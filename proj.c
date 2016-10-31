#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#define NUM_THREADS     5

 void *leitor(void *argumento){
	 int* n = (int*) argumento;
    char nome[] = "teste.txt";
    FILE *fp = fopen(nome,"rt");
    if (!fp) exit(1);
    char linha[50];
    while (fgets(linha, 50, fp)){  
		printf("Leitor#%d: ", n);
		printf("%s\n",linha);
	}
    fclose(fp);
    pthread_exit(NULL);
 }
 
  void *escritor(void *argumento){
    char nome[] = "teste.txt";
    FILE *fp = fopen(nome,"at");
    if (!fp) exit(1);
    char linha[] = "Oi, estou escrevendo!";
    fseek(fp, 0L, SEEK_END);
    fprintf(fp,"%s\n",linha);
    fclose(fp);
    pthread_exit(NULL);
 }

 int main (int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	 

    int rc;
    long i;
	int j = 0;
	char arquivo[] = "teste.txt";
	
    for(i=0; i<NUM_THREADS; i++){
       printf("Main: criando thread #%ld\n", i);
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
	
	pthread_attr_destroy(&attr);
	
	for(i=0; i<NUM_THREADS; i++) {
       		rc = pthread_join(threads[i], NULL);
      		if (rc) {
         		 printf("ERROR; return code from pthread_join() is %d\n", rc);
         		 exit(-1);
         	 }
       }

	return 0;
 }
