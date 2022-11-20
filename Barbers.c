#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include<time.h>

void PV (int id,int mode);
void Cut_hair(int);
void End (int );
void Start(int );
int number_of_barbers,number_of_clients,barber_sem;

int main(){
    int pid,i;
    printf("Enter the numbers of barbers:\n");
    scanf("%d",&number_of_barbers);

    do{
        printf("Enter the number of clients:\n");
        scanf("%d",&number_of_clients);
    }while(number_of_clients<number_of_barbers);

    barber_sem= semget(IPC_PRIVATE,1, IPC_CREAT|0666);
    semctl (barber_sem, 0, SETVAL, number_of_barbers);

    for(i=0;i<number_of_clients;i++){
        pid=fork();
        if(pid==0){
            break;
        }
    }
    if(pid==0){
        Cut_hair(i+1);
        }
    else{
        while(wait(NULL)>-1);
    }
}


void Start(int NumCli){
    printf("The client %d enters the salon and is waiting for a barber\n",NumCli);
    PV(barber_sem,-1);
    printf("The client %d is taken charge of by a barber\n",NumCli);
}
void End (int NumCli){
    PV(barber_sem,1);
    printf("The client %d finishes cutting hair and is exiting the salon\n",NumCli);
}
void Cut_hair(int NumCli){
    srand(time(NULL));  
    int x=rand() % (4 - 1+1) + 1;
    Start(NumCli);
    printf("The client %d's being cut right now \n",NumCli);
    sleep(x);
    End(NumCli);
}



void PV (int id,int mode) {
    //P: mode=-1 ; V: mode1;
    struct sembuf operation; 
    operation.sem_num = 0; 
    operation.sem_op = mode; 
    operation.sem_flg = 0; 
    semop (id, &operation, 1);
}

