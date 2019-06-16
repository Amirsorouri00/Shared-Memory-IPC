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

    for (int j = 0;j<40000;j++)
        for (int i = 0,k=j*1024; i<1024;i++,k++)
            str[k] = dataset[j][i];

    // printf("Data written in memory: NULL and pointer is %s\n", str); 
      
    //detach from shared memory  
    shmdt(str); 
  
    return 0; 
}