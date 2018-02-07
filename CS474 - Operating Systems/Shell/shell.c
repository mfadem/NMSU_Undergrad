/**
* Michael Fadem
* Project 2: Simple Shell
* CS 474
* 11/1/16
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_LINE		80 /* 80 chars per line, per command */
#define MAX_HIST		10
char *hist[MAX_HIST];
int ref[MAX_HIST];
int histCount = 1;
static int should_run = 1;

char *input(void){
	
	char *arg = NULL;
	ssize_t buffer = 0;
   //fflush(stdin);
	getline(&arg, &buffer, stdin);
	//printf("%s", arg);
	return arg;

}

char **split(char *userInput){
	
	int tokenSize = MAX_LINE/2 + 1;
	int tokenCount = 0;
	char **tokens = malloc(tokenSize * sizeof(char *));
	char *tokenGrab;
	
	tokenGrab = strtok(userInput, " ");
	
	while(tokenGrab != NULL){
		
		if(tokenCount > tokenSize){
			printf("Tokens exceed allotted amount\n");
			return '\0';
			//exit(EXIT_FAILURE);
		}
		
		tokens[tokenCount] = tokenGrab;
		tokenCount++;
		tokenGrab = strtok(NULL, " ");

	}
	
	tokens[tokenCount] = NULL;
	return tokens;

}

void run(char **args){
	
	pid_t pid, wpid;
	int state;
	
	pid = fork();
	
	char * last;
	int j;

	for(j = 0; args[j] != NULL; j++){
		last = args[j];
	}

	if(pid == 0){ //Child
		if(execvp(args[0], args) == -1)
			perror("osh>");	
		exit(0);
	}
	else if(pid < 0){ //Error
		perror("Fork Failed!\n");
	}
	else{ //Parent
		//wait(NULL);
		do{
		if(!strcmp(last,"&") == 0){
			wpid = waitpid(pid, &state, 0);	
		}
		else{
			wpid = waitpid(pid, &state, WNOHANG);
		}
		}while(!WIFEXITED(state) && !WIFSIGNALED(state));
	}
}

int history(char *hist[], int current){

	int i = current;
	
	do{
		if(hist[i]){
			printf("%d. %s\n", histCount, hist[i]);
			ref[i] = histCount;
			histCount++;
		}
		i = (i+1) % MAX_HIST;
	}
	while(i != current);
	return 0;
}

int main(void){
	
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	char *in;
	char **arg;
	const char* leave = "exit";
	const char* check = "history";
	const char* checkRun = "!!";
	
	int argc = 0;
	int count = 0;
	int i, current = 0;
	
	for(i = 0; i < MAX_HIST; i++){
		hist[i] = NULL;
	}
	
	for(i = 0; i < MAX_HIST; i++){
		ref[i] = 0;
	}
	
	printf("Welcome to Michael Fadem's Simple Shell\n");
	printf("Enter enter your commands below, Enter 'exit' to quit, Enter 'history' to see recent commands\n");

   while (should_run){ 
	  
      printf("osh>");
      fflush(stdout);

		//free(hist[current]);
		in = input();
		strtok(in, "\n");
		current = (current + 1) % MAX_HIST;
		
		int a = in[1] - '0';
		int b = in[2] - '0';
		int ab;
		if(b == 0){
			b = b + 1;
			ab = a + b*10;
			ab--;
		}
		else{
			ab = a + b*10;
		}
		int ex = in[0];
		
		//printf("a = %d\n",a);
		//printf("in[0] = %d\n",ex);
		//printf("b = %d\n",b);
		//printf("ab = %d\n",ab);
		
		if(strcmp(in,leave) == 0){
			printf("Goodbye!\n");
			should_run = 0;
		}

		else if(!strcmp(in, check) == 0 && !strcmp(in, checkRun) == 0 && ex != 33){
			hist[current] = strdup(in);
		}

		else if(strcmp(in,check) == 0){
			current--;
			if(hist[current] != NULL){
				history(hist,current);
				continue;
			}
			else{
				printf("No commands in history.\n");
				continue;
			}
		}

		else if(strcmp(in,checkRun) == 0){
			current--;
			if(hist[current] != NULL){
				in = strdup(hist[current]);
				printf("osh> %s\n", in);
			}
			else{
				printf("No commands in history.\n");
				continue;
			}
		}
		
		if(ex == 33 && a >= 0 && b < 0){
			int r = 0;
			for(i = 0; i < MAX_HIST; i++){
				if(ref[i] == a){
					r = i;
				}
			}
			if(hist[r] != NULL){
				in = strdup(hist[r]);
				printf("osh> %s\n", in);
			}
			else{
				printf("No commands in history.\n");
				continue;
			}
		}
		
		else if(ex == 33 && ab >= 10){
			int r = 0;
			for(i = 0; i < MAX_HIST; i++){
				if(ref[i] == ab){
					r = i;
				}
			}
			if(hist[r] != NULL){
				in = strdup(hist[r]);
				printf("osh> %s\n", in);
			}
			else{
				printf("No commands in history.\n");
			}
				
		}

		arg = split(in);
		if(arg == '\0')
			continue;

		if(should_run != 0 && !strcmp(arg[0],check) == 0 && !strcmp(arg[0],checkRun) == 0){
			run(arg);
		}

   }//end while 
	
	for(i = 0; i < MAX_HIST; i++){
		free(hist[i]);
		hist[i] = NULL;
	}
	
	return 0;
} //end main
