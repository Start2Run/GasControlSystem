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
#include "./lib/tcp-client.h"
#include "./lib/state-helper.h"

int gas1Value, gas2Value, gas3Value = 0;
pthread_mutex_t lockGasValues;
sem_t timerLockControlCalculSemaphore;
struct Tcp_Client client_listener, client_sender;

void* TcpListen()
{
  while (1)
	{
		char *array[3];
		ReadMessageSuccesful(&client_listener,array);
		int i = 0;
		for (i = 0; i < 3; ++i) 
        {
			if(array[i]!=NULL)
			if(strlen(array[i])>3)
			{
				char gaseIdentifiant[3]; 
				char stringValue[3];
				bool isLG1=strcmp(gaseIdentifiant,"LG1",3)!=0;
				bool isLG2=strcmp(gaseIdentifiant,"LG2",3)!=0;
				bool isLG3=strcmp(gaseIdentifiant,"LG3",3)!=0;

				if(getSubString(array[i],gaseIdentifiant,0,2)==-1)
				{
					continue;
				}
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
					pthread_mutex_lock(&lockGasValues);
					gas1Value = val;
					pthread_mutex_unlock(&lockGasValues);
					//printf("LG1 value is: %d \n",val);
				}
				else if(isLG2)
				{
					pthread_mutex_lock(&lockGasValues);
					gas2Value = val;
					pthread_mutex_unlock(&lockGasValues);
				}
				else if(isLG3)
				{
					pthread_mutex_lock(&lockGasValues);
					gas3Value = val;
					pthread_mutex_unlock(&lockGasValues);
				}
				else
				{
					continue;
				}			
			}
		}
	}
}

void* ControlCalcul()
{
	while (1)
	{
	sem_wait(&timerLockControlCalculSemaphore);
	pthread_mutex_lock(&lockGasValues);
	struct ControlModel model1 = GetGazControlState(gas1Value);
	pthread_mutex_unlock(&lockGasValues);
	pthread_mutex_lock(&lockGasValues);
	struct ControlModel model2 = GetGazControlState(gas2Value);
	pthread_mutex_unlock(&lockGasValues);
	pthread_mutex_lock(&lockGasValues);
	struct ControlModel model3 = GetGazControlState(gas3Value);
	pthread_mutex_unlock(&lockGasValues);

	Ventilation vent = GetGlobalVentilation(model1,model2,model3);
	Aeration aer = GetGlobalAertation(model1,model2,model3);
	printf("The ventilation is %d \n", (int)vent);
	printf("The aeration is %d \n", (int)aer);
	}
}

void* Timer()
{
    while(1)
    {
        sem_post(&timerLockControlCalculSemaphore);
        sleep(1);
    }
}

int main(void)
{
	InitiliseTcp();

	pthread_t t1,t2,t3;
    if(pthread_mutex_init(&lockGasValues, NULL) != 0 )
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
	//CreateTcpClient(&client_sender);
    printf("Enter the sever IP address: "); 
	//scanf("%s", &client_sender.address);

	printf("Enter port number: "); 
	//scanf("%d", &client_sender.port);

	strcpy(client_listener.address,"192.168.86.159");
	client_listener.port=1231;

	// strcpy(client_sender.address,"192.168.86.159");
	// client_sender.port=1232;
    
	if(connectClient(&client_listener)==-1)
	{
		return -1;
	}
	// if(connectClient(&client_sender)==-1)
	// {
	// 	return -1;
	// }
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