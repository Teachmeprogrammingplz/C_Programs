#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define N 5
typedef struct{
    char value[15];
}message;

void PV (int id,int, int mode);
void read(int,int);
void write(int,int);
int  random_int(int a, int b);
char * random_string(int a, int b);
message* Tampon;

int main(){
    int mutexLec = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    semctl(mutexLec, 0,SETVAL, 1);

    int mutexRed = semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    semctl(mutexRed, 0,SETVAL, 1);

    int semTampon = semget(IPC_PRIVATE,2,IPC_CREAT|0666);
    semctl(semTampon, 0,SETVAL, 0);
    semctl(semTampon, 1,SETVAL, N);
    
    int segid = shmget(IPC_PRIVATE, N*sizeof(message), IPC_CREAT|0666);
    Tampon = shmat(segid, (void *) 0, 0);

    int lecid = shmget(IPC_PRIVATE, sizeof(message), IPC_CREAT|0666);
    int* indLec = shmat(lecid, (void *) 0, 0);
    *indLec=0;

    int redid = shmget(IPC_PRIVATE, sizeof(message), IPC_CREAT|0666);
    int* indRed = shmat(redid, (void *) 0, 0);
    *indLec=0;
    
    int nbFils=0;
    do{
        printf("Give the number of Consumers/Producers (>=1)\nn=");
        scanf("%d",&nbFils);
    }while(nbFils<1);

    int Son=-1;
    int mainPid=getpid();

    for (int fils = 0; fils < 2*nbFils; fils++){
        if(getpid()==mainPid){
            if(fork()==0){
                Son=fils;
            }
        }
    }
    if (Son!=-1 && Son%2){
        for (int i = 0; i < 3; i++){
            PV(semTampon,0,-1);
            PV(mutexLec,0,-1);
            read(Son/2,*indLec);
            *indLec=(*indLec+1)%N;
            PV(mutexLec,0,1);
            PV(semTampon,1,1);
        }
    }else if(Son!=-1){
        for (int i = 0; i < 3; i++){
        PV(semTampon,1,-1);
        PV(mutexRed,0,-1);
        write(Son/2,*indRed);
        *indRed=(*indRed+1)%N;
        PV(mutexRed,0,1);
        PV(semTampon,0,1);
        }
    }else while (wait(NULL)>0);
}

int  random_int(int a, int b) {
    return (rand() % (b - a+1) + a);
}

char * random_string(int a, int b){
    srand(time(NULL));
    int L=  (rand() % (b - a+1) + a);
    int i;
    char *r;
    r=(char *)malloc(L+1); 
    for (i=0; i<L;i++){
        r[i] = random_int('A','Z');
    }
    r[L]='\0';
    return r;
}

void write(int id, int ind){
    char B[20];
    sprintf(B,"%s",random_string(5,10));
    printf("Producer: %d, ind %d, I produce message :%s\n",id,ind,B);
    strcpy(Tampon[ind].value,B);
    sleep(random_int(1,3));
    printf("Producer: %d, I finished producing the message\n",id);
}

void read(int id,int ind){
    printf("Consumer: %d, reading the message\n",id);
    sleep(random_int(1,3));
    printf("Consumer: %d, ind %d, I'm done reading the message :%s\n",id,ind,Tampon[ind].value);
}

void PV (int id,int pos, int mode){
    struct sembuf operation;
    operation.sem_num = pos;
    operation.sem_op = mode;
    operation.sem_flg = 0;
    semop (id, &operation, 1);
}