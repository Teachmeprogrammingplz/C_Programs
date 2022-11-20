#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
  

// This Program Simulates a number of swimmers that come for a swim
// And so they have to take a "basket" and look for "cabinet"
// When inside the "cabinet" they will change their clothes and put them in
// The "basket" they acquired and when done they leave the "cabinet"
// Then they go to swim and come back looking for a free "cabinet"
// Once in they change their clothes and exit the "cabinet" and put down
// The basket for someone else to use

void PV (int id,int mode);
int random_int(int a, int b);

int main()
{

    pid_t f;
    pid_t g;

    int cabinet;
    int swimmer;
    int basket;

    printf("Enter the number of Cabinets :");
    scanf("%d", &cabinet);
    printf("\n");

    printf("Enter the number of swimmers:");
    scanf("%d", &swimmer);
    printf("\n");

    printf("Enter the number of baskets:");
    scanf("%d", &basket);
    printf("\n");

    pid_t parent = getpid();

    // semaphores declaration
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    int a[2] = {basket,cabinet};
  
    semctl(semid, 0, SETALL, a);
    // semaphores declaration*/

    int son = -1;

    pid_t r;
    int shared_index = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
    int *IndC = shmat(shared_index, (void *) 0, 0);
    for (int i = 0; i < swimmer; i++)
    {
        r = fork();
        if (r == 0)
        {
            son = (i+1);
            *IndC+=1;
            break;
        }
    }

    while(*IndC<swimmer){
        if(son==swimmer){
            *IndC+=1;
            if(*IndC==(swimmer)){
            printf("Start!\n");
        }
        }
        
    }
    if (son != -1)
    {   
        // {basket,cabinet};

        PV(semid, -1);
        // printf("Swimmer: %d, took a basket and is looking for a cabinet...\n", son);
        PV(semid+1,-1);
        // printf("Swimmer: %d, cabinet taken\n", son);

        sleep(random_int(1, 3));

        // printf("Swimmer: %d, is out to swim\n", son);
        PV(semid+1,1);
        // printf("The swimmer %d is swimming\n", son);

        sleep(random_int(1, 3));

        // printf("The swimmer %d done, cabine looking...\n", son);
        PV(semid+1,-1);

        sleep(random_int(1, 3));

        printf("The swimmer %d is done!\n", son);
        PV(semid,1);
        PV(semid+1,1);

    }
    else
    {
        while (wait(NULL) > 0);
    }
}

int random_int(int a, int b)
{
    return (rand() % (b - a + 1) + a);
}


void PV (int id,int mode) {
    //P: mode=-1 ; V: mode1;
    struct sembuf operation; 
    operation.sem_num = 0; 
    operation.sem_op = mode; 
    operation.sem_flg = 0; 
    semop (id, &operation, 1);
}