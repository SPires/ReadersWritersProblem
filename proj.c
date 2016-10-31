#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#define NUM_THREADS     10

pthread_mutex_t esc, lei;
int pilha = 0;

 void *leitor(void *argumento){
	pthread_mutex_lock(&lei);
	pilha++;
	if(pilha == 1) pthread_mutex_lock(&esc);
	pthread_mutex_unlock(&lei);
	int* n = (int*) argumento;
    char nome[] = "teste.txt";
    FILE *fp = fopen(nome,"rt");
    if (!fp) exit(1);
    char linha[50];
	printf("\nLeitores no momento: %d\n", pilha);
    while (fgets(linha, 50, fp)){  
		printf("Leitor#%d: %s\n", n, linha);
	}
    fclose(fp);
	pilha--;
	if( pilha == 0) pthread_mutex_unlock(&esc);
    pthread_exit(NULL);
 }
 
  void *escritor(void *argumento){
	pthread_mutex_lock(&lei);
	printf("Esperando acabar as leituras\n");
	pthread_mutex_lock(&esc);
	int* n = (int*) argumento;
    char nome[] = "teste.txt";
    FILE *fp = fopen(nome,"at");
    if (!fp) exit(1);
    char linha[] = "Há um escritor aqui!!!\n";
    fseek(fp, 0L, SEEK_END);
    fprintf(fp,"%s\n",linha);
    fclose(fp);
	printf("Escritor #%d alterou o arquivo \n", n);
	pthread_mutex_unlock(&esc);
	pthread_mutex_unlock(&lei);
    pthread_exit(NULL);
 }

 int main (int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	 
	
	pthread_mutex_init(&lei, NULL);
	pthread_mutex_init(&esc, NULL);
	
	int num;
	printf("Indique o número de threads a serem criadas: \n");
	scanf("%d\n",&num);
	
	
    int rc, tipo;
    long i;
	char arquivo[] = "teste.txt";
	
	//Criar o número de threads determinada pelo usuário.
    for(i = 0; i < NUM_THREADS; i++){
        
		// Tipo determina como a thread será escritor ou leitor.
		tipo = rand() % 2;
	    
		//Anúncio da criação da thread.
		printf("Main: criando thread #%ld\n", i);
	    
		//Criação das threads.
		if (tipo == 0)
			rc = pthread_create(&threads[i], NULL, escritor, (void *)i);
		if (tipo == 1)
			rc = pthread_create(&threads[i], NULL, leitor, (void *)i);
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
