#include<sys/wait.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

// This program creates The following Structure (n=3) 

//             Main Process
//       ___________|___________
//       |          |           |    
//     Son_1       Son_2      Son_3 ............
//    ___|___       |             
//   |       |    Son_21
// Son_11  Son_12
//   |
// Son_111

// Number of processes is 2**n because if we suppose that we have n=5 
// then for n=6 each process will have an extended for loop by 1 iteration
// thus each process will create a new son that is not a father
// that way we can see the number of the processes for n=6 as the number of processes
// in n=5 and with every process having an additional son
// and that means that the number of processes doubles each time
// and since for n=1 we have two processes, then the number of processes will be 2**n
// NB: Notice that this formula zorks for n=0 as well

int main() {
    int n,v=0;
    
    pid_t Son_pid,My_pid,Father_pid;

    printf("n=");
    scanf("%d",&n);
    printf("\n");

    for (int i = 0; i < n; i++){
        Son_pid=fork();

        if(Son_pid==-1){
            printf("Forking Error, Can't create a Son\n");
            continue;
            }

    }

    if(v==0){
        My_pid=getpid();
        Father_pid=getppid();
        v=1;
    }

    printf("I'm the process %d, My Father is %d\n\n",My_pid,Father_pid);

    // Every Process waits for his children before finishing
    // This Avoids Having zombie processes 
    while(wait(NULL)>-1);
    return 0;
}