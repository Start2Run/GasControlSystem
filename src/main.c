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
#include "./lib/pid-controller.h"

int main(void)
{
    // printf("Enter the sever IP address: "); 
	// scanf("%s", &address);

	// printf("Enter port number: "); 
	// scanf("%d", &port);

    // if(connectClient(address,port)==-1)
	// {
	// 	return -1;
	// }
		
	struct ControlModel model1 = GetGazControlState(30);
	struct ControlModel model2 = GetGazControlState(60);
	struct ControlModel model3 = GetGazControlState(3);

	Ventilation vent = GetGlobalVentilation(model1,model2,model3);
	Aeration aer = GetGlobalAertation(model1,model2,model3);

	struct PID pid;

	pid_init(&pid);

	double val = 20;
    for (int i = 0; i < 100; i++) {
		pid_cal(-40,val,&pid);
        printf("val:% 7.3f inc:% 7.3f\n", val, pid.output);
        val += pid.output;
    }
    return 0;
}