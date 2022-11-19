

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// This program does the following (Consumer Producer Problem)
// it creates a file and reads from it "Case by Case" or writes in it  "Case by Case"
// and we have two programs:
// One that writes in the file and one that reads from it and since the file is a shared 
// ressource between two processes then we have to use semaphores
// Reading and writing in the file happens with case structs
struct Case {char value[80];};
// N is the the number of cases in the file (Logically)

#define N 4

void PV (int id,int mode);
int  random_int(int a, int b);
char * random_string(int a, int b);
FILE * create_file( char name[], int n);
struct Case  read(int i, FILE *f);
int write(char s[], int i,FILE *f);

int main(){

    FILE* f=create_file("buffer",N);

    int semid = semget(IPC_PRIVATE,2,IPC_CREAT|0666);
    semctl(semid, 0,SETALL, 0,N);

    int C=0;
    // Making sure that number of messages is greater than the size of the storage(N cases)
    do{
        printf("Give the number of messages to transmit : ");
        scanf("%d",&C);
        printf("\n");
    }while (C<=N);


    pid_t pid;
    pid = fork();
       
    if (pid == 0){

        //Consumer
        int indC=0;
        for (int in = 0; in < C; in++){
            PV(semid,-1);
            struct Case B=read(indC,f);

            PV(semid+1,1);
            printf("Consumer Msg: %s, in index: %d\n",B.value,indC);
            indC=(indC+1)%N;

            sleep(random_int(1,3));
        }
    }
    else{
        //Producer
        int indP=0;
       
        for (int in = 0; in < C; in++){
            
            char B[80];
            sprintf(B,"%s",random_string(5,10));
            sleep(random_int(1,3));

            PV(semid+1,-1);
            write(B,indP,f);
            PV(semid,1);

            printf("Producing Msg: %s, in index: %d\n",B,indP);
            indP=(indP+1)%N;
        }
        while(wait(NULL)>0);
    }
}


int  random_int(int a, int b) {
    return  (rand() % (b - a+1) + a);
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
// Name of the file and the Case (The struct) number in the file.
FILE * create_file( char name[], int n){
    struct Case e;
    int  i;
    FILE *f = fopen(name , "w+");
    strcpy(e.value,"");
    for(i=1;i<=n; i++) 
        fwrite(&e, sizeof(e),1,f);
    return(f);
}
struct Case  read(int i, FILE *f) {
    f=fopen("./buffer","r");
    fseek(f, i*sizeof(struct Case), SEEK_SET); 
    struct Case e;
    fread( &e, sizeof(e), 1, f); 
    fclose(f);
    return e; 
}

// s is the string to put in n° i Case 3
int write(char s[], int i,FILE *f) {
    f=fopen("./buffer","w");
    int r;
    fseek(f, i*sizeof(struct Case), SEEK_SET); 
    struct Case e;
    strcpy(e.value, s);
    r=fwrite( &e, sizeof(e), 1, f);
    fclose(f);
    return(r); // return value 0 or 1 
}
void PV (int id,int mode) {
    //P: mode=-1 ; V: mode1;
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = mode;
    operation.sem_flg = 0; 
    semop (id, &operation, 1);
}