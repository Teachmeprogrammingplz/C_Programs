#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

// This program creates The following Structure (n=3) 

//             Main Process
//                   |
//                 Son_1
//                   |
//                 Son_2
//                   |
//                 Son_3
//                   


int main(){
    pid_t Son_pid,My_pid,Father_pid;
    int n;

    printf("n=");
    scanf("%d",&n);
    printf("\n");

    for (int i = 0; i < n; i++){
        Son_pid=fork();

        if (Son_pid==-1){
            printf("Forking Error, Can't create a Son");
        }else{
            if (Son_pid==0){

                My_pid=getpid();
                Father_pid=getppid();
                printf("I'm the process %d, My Father is %d\n\n",My_pid,Father_pid);
              
            }
            else{
                // This break statment stops the process from creating further sons
                // and that way it will have a unique son
                break;
            }
        }
    }
    while(wait(NULL)>0);
}
