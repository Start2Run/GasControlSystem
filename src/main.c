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
 
int main(void)
{
    printf("Enter the sever IP address: "); 
	scanf("%s", &address);

	printf("Enter port number: "); 
	scanf("%d", &port);

    if(connectClient(address,port)==-1)
	{
		return -1;
	}
  
    return 0;
}