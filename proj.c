#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#define NUM_THREADS     5

 void *leitor(void *argumento){
    //le
	printf("Lendo\n");
    pthread_exit(NULL);
 }
 
  void *escritor(void *argumento){
    //escreve
	printf("Escrevendo\n");
    pthread_exit(NULL);
 }

 int main (int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
    int rc;
    long i;
	int j = 0;
	char arquivo[] = "teste.txt";
	
    for(i=0; i<NUM_THREADS; i++){
       printf("In main: creating thread %ld\n", i);
	   if(j == 2){
			rc = pthread_create(&threads[i], NULL, escritor, (void *)i);
			j = 0;
	   }else{
		   rc = pthread_create(&threads[i], NULL, leitor, (void *)i);
		   j++;
	   }
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
    }

	return 0;
 }
