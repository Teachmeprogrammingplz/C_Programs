#include<stdio.h>
#include<malloc.h>
int main(){
    int N;
    printf("N=");
    scanf("%d",&N);
    char *P[N];
    // fflush(stdin);
    for(int i=0;i<N;i++){
      *(P+i)=(char *)malloc(20);
      scanf("%s",P[i]);
    }
    for(int i=0;i<N;i++){
        printf("%s\n",P[i]);
    }
}