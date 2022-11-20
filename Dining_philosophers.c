

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include<sys/shm.h>

void think(int i);
void eat(int i);
int  random_int(int a, int b);
void P(int id,int position);
void V(int id,int position);
void Test(int i);
void Take_fork(int i);
void Put_down_fork(int i);
void philosopher(int i,int thinkering_times);

  
   //shared mem used with the functions (declaration)
int philo;
int s;
int *sh_state;
int mutex;


  //shared mem used with the functions (declaration)
void main(){
    int a[5]={0,0,0,0,0};
    //shared mem used with the functions 

    philo=semget(IPC_PRIVATE,5,IPC_CREAT|0666);   
    mutex = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    semctl(philo, 0,SETALL,a);
    semctl(mutex, 0,SETVAL,1); 
    //shared mem used with the functions

    
    s= shmget(IPC_PRIVATE,5*sizeof(int), IPC_CREAT|0666);
    sh_state = shmat(s, (void *) 0, 0);

    for (size_t i = 0; i < 5; i++)
    {
        *(sh_state+i)=0;
    }

    int n;
    pid_t parent;
    printf("Give the number of times you want your comrades to think:");
    scanf("%d",&n);
  
   pid_t c;
   parent=getpid();

   
   for (int i = 0; i <5; i++)
   {
       if(getpid()==parent){
           if(fork()==0){
           philosopher(i,n);
       }
       }
       
   }
while(wait(NULL)>0);
}


void think(int i){
    printf("The %dth philosopher STARTS THINKING...\n",i+1);
    sleep(random_int(2,4));
    printf("The %dth philosopher ENDS THINKING...\n",i+1);
}
void eat(int i){
    printf("The %dth philosopher STARTS EATING...\n",i+1);
    sleep(random_int(2,4));
    printf("The %dth philosopher ENDS EATING...\n",i+1);
}

void Take_fork(int i){
    //thinks 0 hungry 1 eats 2
    P(mutex,0);
    *(sh_state+i)=1;
    Test(i);
    V(mutex,0);
    P(philo,i);
}
void Put_down_fork(int i){
    P(mutex,0);
    *(sh_state+i)=0;
    Test((i+1)%5);
    Test((i+4)%5);
    V(mutex,0);
}

void Test(int i){
    if(*(sh_state+i)==1 && *(sh_state+(i+4)%5)!=2 && *(sh_state+(i+1)%5)!=2){
        *(sh_state+i)=2;
        V(philo,i);
    }
}

void philosopher(int i,int thinkering_times){
    for(int j=0;j<thinkering_times;j++){
        think(i);
        Take_fork(i);
        eat(i);
        Put_down_fork(i);
    }
}

int  random_int(int a, int b) {



  return  (rand() % (b - a+1) + a);

}



void P (int id,int position) {
        struct sembuf operation;
        operation.sem_num = position;
        operation.sem_op = -1;
        operation.sem_flg = 0;
        semop (id, &operation, 1);

    }
void V (int id,int position) {
      struct sembuf operation;
      operation.sem_num = position;
      operation.sem_op = 1;
      operation.sem_flg = 0;
      semop (id, &operation, 1);

  }












