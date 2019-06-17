#define _GNU_SOURCE
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/wait.h>
#include <string.h>
#include "splicer-benchmarking.h"


int main() 
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65);
	// char** dataset = initializer();
    char* buf =  malloc(sizeof(char*) * SPLICE_SIZE);
    char ch[58] = {};
    random_char_selector(ch);
    time_t t;
    srand((unsigned) time(&t));
    for(int i = 0; i < SPLICE_SIZE; i++){
        buf[i] = ch[(rand() % 58)];
    }



    // shmget returns an identifier in shmid
    int shmid = shmget(key,SPLICE_SIZE,0666|IPC_CREAT);
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(char*)0,0); 

    pid_t   childpid;
	if((childpid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}
	if(childpid == 0)
	{
        clock_t start = clocker(0, "start");
        memcpy(str, buf, SPLICE_SIZE);

        clock_t end = clocker(1, "end");
        double first_result = time_calc(end, start, "here");
        printf("The frequency is eqals to(Mbps): %f .\n", (double)((SPLICE_SIZE)/(first_result*1000000)));
        printf("Data written in memory: NULL and pointer is %d\n", str[500000]); 

    }
    else{
        int child1_status, child2_status;
        waitpid(childpid, &child1_status, 0);
        if (child1_status == 0)  // Verify child process terminated without error. 
        {
            printf("The child process terminated normally.\n");    
        }
        else{printf("The child process terminated with an error!.\n");}
        
        exit(0);
    }

    
    //detach from shared memory  
    // shmdt(str); 
  
    return 0; 
}