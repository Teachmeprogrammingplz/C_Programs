#include<stdio.h>
main(){
	int N,i,s,S;
	printf("donner N le nombre d'entier\n");
	scanf("%d",&N);
	printf("donner les N entier\n");
	for(i=0,S=0;i<N;i++){
	scanf("%d",&s);
	S+=s;
	}
	printf("%d",S);
}
