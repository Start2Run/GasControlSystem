#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <semaphore.h> 
#include <arpa/inet.h>
#include "./lib/tcp-client.h"
#include "./lib/state-helper.h"

int gas1Value, gas2Value, gas3Value = 0;
pthread_mutex_t lockGasValues,lockSendMessageMutex;
sem_t timerLockControlCalculSemaphore;
struct Tcp_Client client_listener, client_sender;

void* TcpListen()
{
  while (1)
	{
		char *array[3];
		ReadMessageSuccesful(&client_listener,array);
		int i = 0;
		int gas1,gas2,gas3 = 1;
		for (i = 0; i < 3; ++i) 
        {
			if(array[i]!=NULL)
			if(strlen(array[i])>3)
			{
				char gaseIdentifiant[3]; 
				char stringValue[3];
				
				if(getSubString(array[i],gaseIdentifiant,0,2)==-1)
				{
					continue;
				}
				bool isLG1=strcmp(gaseIdentifiant,"LG1",3) == 0;
				bool isLG2=strcmp(gaseIdentifiant,"LG2",3) == 0;
				bool isLG3=strcmp(gaseIdentifiant,"LG3",3) == 0;

				if(!isLG1 && !isLG2 && !isLG3)
				{
					continue;
				}			
				if(getSubString(array[i],stringValue,3,strlen(array[i]))==-1)
				{
					continue;
				}

				int val = strtol(stringValue, NULL, 10);
				
				if(isLG1)
				{
					gas1 = val;
				}
				else if(isLG2)
				{
					gas2 = val;
				}
				else if(isLG3)
				{
					gas3 = val;
				}
				else
				{
					continue;
				}			
			}
		}
		pthread_mutex_lock(&lockGasValues);
			gas1Value = gas1;
			gas2Value = gas2;
			gas3Value = gas3;
		pthread_mutex_unlock(&lockGasValues);
	}
}

void* ControlCalcul()
{
	while (1)
	{
	sem_wait(&timerLockControlCalculSemaphore);
	pthread_mutex_lock(&lockGasValues);
	struct ControlModel model1 = GetGazControlState(gas1Value);
	struct ControlModel model2 = GetGazControlState(gas2Value);
	struct ControlModel model3 = GetGazControlState(gas3Value);

	pthread_mutex_unlock(&lockGasValues);

	Ventilation vent = GetGlobalVentilation(model1,model2,model3);
	Aeration aer = GetGlobalAertation(model1,model2,model3);
	SendControlCommand(aer, vent, model1, model2, model3);
	}
}

void* Timer()
{
    while(1)
    {
        sem_post(&timerLockControlCalculSemaphore);
        usleep(500);
    }
}

int main(void)
{
	InitiliseTcp();

	pthread_t t1,t2,t3;
    if(pthread_mutex_init(&lockGasValues, NULL) != 0 || pthread_mutex_init(&lockSendMessageMutex, NULL) != 0)
    {
       printf("Mutex initialization failed.\n");
       return 1;
    }
	sem_init(&timerLockControlCalculSemaphore, 0, 1);

	pthread_create(&t1, NULL, TcpListen, NULL);
	pthread_create(&t2, NULL, Timer, NULL);
	pthread_create(&t3, NULL, ControlCalcul, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

    return 0;
}

void InitiliseTcp()
{
	CreateTcpClient(&client_listener);
	CreateTcpClient(&client_sender);

    printf("Enter the sever IP address: "); 
	scanf("%s", &client_sender.address);
	
	strcpy(client_listener.address, client_sender.address);

	printf("Enter the listener port: "); 
	scanf("%d", &client_listener.port);

	printf("Enter the sender port: "); 
	scanf("%d", &client_sender.port);

    
	if(connectClient(&client_listener)==-1)
	{
		return -1;
	}
	if(connectClient(&client_sender)==-1)
	{
		return -1;
	}
}

/*Function definition*/
int  getSubString(char *source, char *target,int from, int to)
{
	int length=0;
	int i=0,j=0;
	
	//get length
	while(source[i++]!='\0')
		length++;
	
	if(from<0 || from>length){
		printf("Invalid \'from\' index\n");
		return 1;
	}
	if(to>length){
		printf("Invalid \'to\' index\n");
		return 1;
	}	
	
	for(i=from,j=0;i<=to;i++,j++){
		target[j]=source[i];
	}
	
	//assign NULL at the end of string
	target[j]='\0'; 
	
	return 0;	
}

void SendControlCommand(Aeration aer, Ventilation vent, struct ControlModel gas1Model, struct ControlModel gas2Model, struct ControlModel gas3Model)
{
	// Build message

    char message[100]={0x0};

	strcat(message, "AG1");
	strcat(message, AlarmStrings[(int)gas1Model.alarm]);
	strcat(message,"\n");
	strcat(message, "AG2");
	strcat(message, AlarmStrings[(int)gas2Model.alarm]);
	strcat(message,"\n");
	strcat(message, "AG3");
	strcat(message, AlarmStrings[(int)gas3Model.alarm]);
	strcat(message,"\n");
	strcat(message, AerationStrings[(int)aer]);
 	strcat(message,"\n");
	strcat(message, VentilationStrings[(int)vent]);
	strcat(message,"\n");
	strcat(message, GazInjectionStrings[(int)gas1Model.GasInjection]);
	strcat(message, "1");
 	strcat(message,"\n");
	strcat(message, GazInjectionStrings[(int)gas2Model.GasInjection]);
	strcat(message, "2");
	strcat(message,"\n");
	strcat(message, GazInjectionStrings[(int)gas3Model.GasInjection]);
	strcat(message, "3");
	strcat(message,"\n");

	pthread_mutex_lock(&lockSendMessageMutex);
       sendMessage(message, &client_sender);
    pthread_mutex_unlock(&lockSendMessageMutex);
	//Clear message
    memset(message, 0, strlen(message));
}