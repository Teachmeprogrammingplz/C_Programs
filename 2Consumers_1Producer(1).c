#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define N 4
struct Case {
    char value[80];
};
void PV (int id,int mode);
int  random_int(int a, int b);
char * random_string(int a, int b);
int size_of(char *str);

int main(){
    int semid = semget(IPC_PRIVATE,3,IPC_CREAT|0666);
    semctl(semid, 0,SETALL, 0,N,1);

    int shared_space = shmget(IPC_PRIVATE, N*sizeof(struct Case), IPC_CREAT|0666);
    int *shared_space_pointer=shmat(shared_space, (void *) 0, 0);

    int shared_index = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
    int *IndC = shmat(shared_index, (void *) 0, 0);
    int C=0;
    
    do{
        printf("Give the number of messages to transmit : ");
        scanf("%d",&C);
    } while (C<=N);
    int pid;
    pid = fork ( );
    if (pid == 0){
        if(fork()==0){
            for (int in = 0; in < C; in++){
                PV(semid+2,-1);
                PV(semid,-1);
                //Actual Consumption
                //Actual Reading
                int c=0;
                char B[12];
                while(*(shared_space_pointer+80*(*IndC)+c)!='\0'){
                    B[c]=*(shared_space_pointer+80*(*IndC)+c);
                    c++;
                }

                PV(semid+1,1);
                // Showing the consumption start
                printf("Consumer 1 Msg: %s, in index: %d\n",B,*IndC);
                // Showing the consumption end

                *IndC=(*IndC+1)%N;
                
                PV(semid+2,1);
                sleep(random_int(1,3));
            }
        }else{
            for (int in = 0; in < C; in++){
                PV(semid+2,-1);
                PV(semid,-1);
                //Actual Consumption
                //Actual Reading
                int c=0;
                char B[12];
                while(*(shared_space_pointer+80*(*IndC)+c)!='\0'){
                    B[c]=*(shared_space_pointer+80*(*IndC)+c);
                    c++;
                }

                PV(semid+1,1);
                // Showing the consumption start
                printf("Consumer 2 Msg: %s, in index: %d\n",B,*IndC);
                // Showing the consumption end

                *IndC=(*IndC+1)%N;
                
                PV(semid+2,1);
                sleep(random_int(1,3));
            }
        }
    }
    else{
        int IndP=0;
        for (int in = 0; in < C; in++){

            //Actual Production 
            char* str_pointer;
            str_pointer=random_string(5,10);
           
            PV(semid+1,-1);
            // the limit of the loop is included in order to add also the '\0'
            // Writing the production
            for(int i=0;i<=size_of(str_pointer);i++){
                 *(shared_space_pointer+IndP*80+i)=*(str_pointer+i);
        
            }
            PV(semid,1);
            // Showing the Production start
            char B[12]; 
            int c=0;
            while(*(shared_space_pointer+80*(IndP)+c)!='\0'){
                B[c]=*(shared_space_pointer+80*(IndP)+c);
                    c++;
            }
            printf("Producer Msg: %s, in index: %d\n",B,IndP);
            sleep(random_int(1,3));
            //Showing the Production end
            IndP=(IndP+1)%N;
        }
        while(wait(NULL)>0);
    }
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
void PV (int id,int mode) {
    //P: mode=-1 ; V: mode1;
    struct sembuf operation; // Une seule opération
    operation.sem_num = 0; // Le sémaphore visé est celui d'indice 0
    operation.sem_op = mode; // Pour faire l'opération P
    operation.sem_flg = 0; // Ou operation.sem_flg = SEM_UNDO
    semop (id, &operation, 1);
}

int size_of(char *str){
    int c=0;
    while(str[c]!='\0'){
    c++;
    }
    return c;
}
