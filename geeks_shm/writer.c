#define _GNU_SOURCE
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "splicer-benchmarking.h"


int main() 
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65);
    // printf("12");
	char** dataset = initializer();
    // shmget returns an identifier in shmid
    int shmid = shmget(key,1024*40000,0666|IPC_CREAT);
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
    clock_t start = clocker(0, "start");
    for (int j = 0;j<40000;j++)
        for (int i = 0,k=j*1024; i<1024;i++,k++)
            str[k] = dataset[j][i];
    // str = "hellllllllllllllllllllllllllllllllll\0";
    clock_t end = clocker(1, "end");
    double first_result = time_calc(end, start, "here");
	printf("The frequency is eqals to(Mbps): %f .\n", (double)((40000*1024)/(first_result*1000000)));
    

    printf("Data written in memory: NULL and pointer is %d\n", str[2048]); 
      
    //detach from shared memory  
    shmdt(str); 
  
    return 0; 
}