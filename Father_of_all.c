#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

// This program creates The following Structure 
//          Main Process
//   ____________|___________
//   |      |       |       |    
//  Son_1  Son_2   Son_3   Son_4 ............

int main(){

    pid_t Son_pid,My_pid,Father_pid,Main_pid;
    int n;
    
    printf("Enter The number of processes you wish to create :");
    scanf("%d",&n);
    printf("\n");

    // The Process Id of the program 
    Main_pid=getpid();

    for (int i = 1; i < n; i++){
        // Only The Main Process can create Sons
        if(getpid()==Main_pid){
            Son_pid=fork();
        }
        if (Son_pid==-1){
            printf("Forking Error, Can't create a Son\n");
        }
        else{
            if (Son_pid==0){

                My_pid=getpid();
                Father_pid=getppid();
                printf("I'm a Son with an ID of:  %d\n",My_pid);
                printf("My Father is the Process of ID %d\n\n",Father_pid);
                // This Break is to Stop the Sons from executing unecessary for loop steps
                // Since we only want to check their idendity 
                break;
            }
        }
    }
}
