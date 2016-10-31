#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

pthread_mutex_t esc, lei, prot;
int pilha = 0;

 void *leitor(void *argumento){
	// Não deixa mais leitores entrarem e para proteger o incremento pilha.
	pthread_mutex_lock(&lei);
	pilha++;
	if(pilha == 1) pthread_mutex_lock(&esc);
	pthread_mutex_unlock(&lei);
	
	//Assinalar qual o número da thread.
	int* n = (int*) argumento;
    
	//Acesso ao arquivo compartilhado.
	char nome[] = "teste.txt";
    FILE *fp = fopen(nome,"rt");
    if (!fp) exit(1);
    char linha[50];
	printf("\n Leitores no momento: %d \n", pilha);
	while (fgets(linha, 50, fp)){  
		printf("Leitor#%d: %s \n", n, linha);
	}
    fclose(fp);
	
	// Protege o decremento pilha.
	pthread_mutex_lock(&prot);
	pilha--;
	if( pilha == 0) pthread_mutex_unlock(&esc);
	pthread_mutex_unlock(&prot);
    pthread_exit(NULL);
 }
 
  void *escritor(void *argumento){
	// Impede a entrada de leitores.
	pthread_mutex_lock(&lei);
	printf("Aguardando... \n");
	
	// Aguarda a liberação para a escrita.
	pthread_mutex_lock(&esc);
	
	// Guardar o número da thread.
	int* n = (int*) argumento;
    
	//Acessar o arquivo compartilhado.
	char nome[] = "teste.txt";
    FILE *fp = fopen(nome,"at");
    if (!fp) exit(1);
    char linha[] = "Há um escritor aqui!!!\n";
    fseek(fp, 0L, SEEK_END);
    fprintf(fp,"%s\n",linha);
    fclose(fp);
	
	// Confirmar que a alteração foi feita.
	printf("Escritor #%d alterou o arquivo \n", n);
	
	// Liberar para a próxima thread.
	pthread_mutex_unlock(&esc);
	pthread_mutex_unlock(&lei);
    pthread_exit(NULL);
 }

 int main (int argc, char *argv[]){
    
	int num;
	printf("Indique o número de threads a serem criadas: \n");
	scanf("%d ",&num);
	
	//Crio o vetor de threads.
	pthread_t threads[num];
	
	//Crio o atributo da thread.
	pthread_attr_t attr;
	
	//Inicializar o atributo da thread.
	pthread_attr_init(&attr);
    
	//Faz o atributo ganhar a característica joinable. (Usada pra fazer o programa aguardar o encerramento da thread antes do encerramento do programa.)
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	 
	
	// Inicialização dos semáforos.
	pthread_mutex_init(&lei, NULL);
	pthread_mutex_init(&esc, NULL);

    int rc, tipo;
    long i;
	char arquivo[] = "teste.txt";
	
	//Criar o número de threads determinada pelo usuário.
    for(i = 0; i < num; i++){
        
		// Tipo determina como a thread será escritor ou leitor.
		tipo = rand() % 2;
	    
		//Anúncio da criação da thread.
		printf("Main: criando thread #%ld\n", i);
	    
		//Criação das threads.
		if (tipo == 0){
			rc = pthread_create(&threads[i], &attr, escritor, (void *)i);
		}
		if (tipo == 1){
			rc = pthread_create(&threads[i], &attr, leitor, (void *)i);
        }
		
		//Capturar possíveis erros.
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
        }
    }
	
	// Liberar memória.
	pthread_attr_destroy(&attr);
	
	// Aguardar o encerramento das threads para finalizar o programa.
	for(i=0; i < num; i++) {
       	rc = pthread_join(threads[i], NULL);
      	if (rc) {
         	printf("ERROR; return code from pthread_join() is %d\n", rc);
         	exit(-1);
        }
    }

	return 0;
 }
