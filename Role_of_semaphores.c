#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void PV(int id, int mode);

int main()
{
    printf("To Execute the code without the Semaphores press 0 Otherwise press 1:");
    
    int choice;
    scanf("%d", &choice);

    if (choice == 1){

        int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
        semctl(semid, 0, SETVAL, -1);
        int pid;
        pid = fork();
        
        if (pid == 0){
            for (char i = 'A'; i <= 'Z'; i++){
                printf("%c\n", i);
                sleep(1);
            }
            PV(semid, 1);
        }
        else
        {

            PV(semid, -1);
            if (fork() == 0){
                for (int i = 1; i <= 30; i++){
                    printf("%d\n", i);
                    sleep(1);
                }
            }
            while (wait(NULL) > 0);
        }
    }
    //Choice 0
    else{
        int pid;
        pid = fork ( );
        if (pid == 0){
            for (char i = 'A'; i <= 'Z'; i++){
                printf("%c\n",i);
                sleep(1);
            }  
        }
    else{
        if (fork()==0){
            for (int i = 1; i <= 30; i++){
                printf("%d\n",i);
                sleep(1);
            }
        }
        while (wait(NULL)>0);
    }

    }
}

void PV(int id, int mode)
{
    // P: mode=-1 ; V: mode1;
    struct sembuf operation; // Une seule opération
    operation.sem_num = 0;   // Le sémaphore visé est celui d'indice 0
    operation.sem_op = mode; // Pour faire l'opération P
    operation.sem_flg = 0;   // Ou operation.sem_flg = SEM_UNDO
    semop(id, &operation, 1);
}