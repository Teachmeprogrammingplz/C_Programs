#include<stdio.h>
#include<stdlib.h>
int main(){
    int *p,N;
    printf("N=");
    scanf("%d",&N);
    p=(int*)malloc((N+1)*sizeof(int));
    for(int i=0;i<N;i++){
        printf("n%d=",i+1);
        scanf("%d",p+i);
    }
    for(int i=0;i<N/2;i++){
        *(p+N)=*(p+i);
        *(p+i)=*(p+N-1-i);
        *(p+N-1-i)=*(p+N);
    }
    for(int i=0;i<N;i++){
        printf("%d,",*(p+i));
    }
}