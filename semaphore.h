#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void PV (int id,int mode,int pos) {
    struct sembuf operation; // Une seule opération
    operation.sem_num = 0; // Le sémaphore visé est celui d'indice 0
    operation.sem_op = mode; // Pour faire l'opération P
    operation.sem_flg = 0; // Ou operation.sem_flg = SEM_UNDO
    semop (id, &operation, 1);
}
