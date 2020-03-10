#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <semaphore.h> 
#include "./lib/tcp-client.h"
#include "./lib/state-helper.h"

int main(void)
{
	struct Tcp_Client client_sender;
	CreateTcpClient(&client_sender);

    printf("Enter the sever IP address: "); 
	scanf("%s", &client_sender.address);

	printf("Enter port number: "); 
	scanf("%d", &client_sender.port);

    if(connectClient(&client_sender)==-1)
	{
		return -1;
	}
		
	// struct ControlModel model1 = GetGazControlState(30);
	// struct ControlModel model2 = GetGazControlState(60);
	// struct ControlModel model3 = GetGazControlState(3);

	// Ventilation vent = GetGlobalVentilation(model1,model2,model3);
	// Aeration aer = GetGlobalAertation(model1,model2,model3);

	while (1)
	{
		char* a;
		ReadMessage(&client_sender,&a);
		printf("value is %s", &a);
	}
		
    return 0;
}