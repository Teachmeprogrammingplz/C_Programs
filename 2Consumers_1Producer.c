#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define N 4
struct Case {
    char value[80];
};
static int* IndP;
void PV (int id,int mode);
int  random_int(int a, int b);
char * random_string(int a, int b);
FILE * create_file( char name[], int n);
struct Case  read(int i, FILE *f);
int write(char s[], int i,FILE *f);
int size_of(char *str);



int main(){
    // Shared Memory 
    int *shared_space = mmap(NULL, N*sizeof(struct Case), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // Shared Producer index variable
    int *IndC = mmap(NULL, sizeof *IndC, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    int semid = semget(IPC_PRIVATE,3,IPC_CREAT|0666);
    semctl(semid, 0,SETALL, 0,N,1);

    int C=0;
    // Making sure that number of messages is greater than the size of the storage(N cases)
    do{
        printf("Give the number of messages to transmit : ");
        scanf("%d",&C);
        printf("\n");
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
                while(*(shared_space+80*(*IndC)+c)!='\0'){
                    B[c]=*(shared_space+80*(*IndC)+c);
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
                while(*(shared_space+80*(*IndC)+c)!='\0'){
                    B[c]=*(shared_space+80*(*IndC)+c);
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
                 *(shared_space+IndP*80+i)=*(str_pointer+i);
        
            }
            PV(semid,1);
            // Showing the Production start
            char B[12]; 
            int c=0;
            while(*(shared_space+80*(IndP)+c)!='\0'){
                B[c]=*(shared_space+80*(IndP)+c);
                    c++;
            }
            printf("Producer Msg: %s, in index: %d\n",B,IndP);
            sleep(random_int(1,3));
            //Showing the Production end
            IndP=(IndP+1)%N;
        }
        // while(wait(NULL)>0);
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
    struct sembuf operation; 
    operation.sem_num = 0; 
    operation.sem_op = mode; 
    operation.sem_flg = 0; 
    semop (id, &operation, 1);
}


int size_of(char *str){
    int c=0;
    while(str[c]!='\0'){
    c++;
    }
    return c;
}
