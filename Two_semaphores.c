#include<stdio.h>
#include "semaphore.h"

// This program simply demonstrates a very simple use of semaphores
// in order to better clarify their use 
// Since in this program our main aim for the father to create two direct processes in this way
//         Father Process
//       ________|________
//       |               |    
//     Son_1            Son_2
//And we must ensure that the Second son (Son_2) executes first Then the first son (Son_1)
// then the Father (Father Process)

// Son_2 > Son_1 > Father

// Note that the "Printf" can be replaced with any other piece of code
// And it is just a way of clarifying the understanding of the order  
void msg_father( ){
    printf ("Father is starting \n");
    sleep (3);
    printf ("Father is ending \n\n");
}
void msg_Son_1( ){
    printf ("Son_1 is starting \n");
    sleep(3);
    printf ("Son_1 is ending\n\n");
}
void msg_Son_2( ){
    printf ("Son_2 is starting\n");
    sleep(3);
    printf ("Son_2 is ending \n\n");
}
main( ){
    // The 2 in semget parameters means that we will have two semaphores
    int semid = semget(IPC_PRIVATE,2,IPC_CREAT|0666);
    semctl(semid, 0,SETALL, 0,0);
    int pid;
    pid = fork ( );
    if (pid == 0){
        PV(semid,-1,0);
        msg_Son_1 ( );
        PV(semid,1,1);
    }
    else{
        if (fork()==0){
            msg_Son_2  ( );
            PV(semid,1,0);
        }else{
            PV(semid,-1,1);
            msg_father ( );
            while (wait(NULL)>0);
        }
        
    }
}