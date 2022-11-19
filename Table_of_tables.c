#include<stdio.h>
#include<stdlib.h>
int main(){
    int *p[3],n;
    for(int i=0;i<3;i++){
        printf("n=");
        scanf("%d",&n);
        p[i]=(int*)malloc((n+1)*sizeof(int));
        *p[i]=n;
        for(int j=1;j<=n;j++){
            printf("V%d=",j);
            scanf("%d",p[i]+j);
        }
    }
    for(int i=0;i<3;i++){
        n=*p[i];
        for(int j=1;j<=n;j++){
            printf("%d,",*(p[i]+j));
        }
        printf("\n");
    }
}