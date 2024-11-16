#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAX_LINE 80
//Module to read from the user
void read_line(char *line);
//Module to separte tokens
void separate_tokens(char *line, char **args);
//Returns current directory
void currentDirectory();
//Going to home
void cdHome();
//Going to custom
void cdCustom(char *path);

//Executing commands
void execute(char **args);
void executePrograms(char **args);
int numberOfPrompts = 0;
pid_t pidProgram = 0;

void writeCounter();
void endOfProgram();
void printPrompts();

void killProgram();



int main(){
	char prompt[MAX_LINE];
	char *args[MAX_LINE];
	cdHome();

	while(1){
		read_line(prompt);
		if(strlen(prompt)!=0)
			separate_tokens(prompt, args);


		if(strcmp(args[0],"salir") ==0){
			break;
		} else if (strcmp(args[0],"cd")==0){
			if(args[1] == NULL){
				cdHome();
			} else {
				cdCustom(args[1]);
			}
		} else if (args[0] != NULL && args[0][0] == '.'){
			executePrograms(args);
		} else if (args[0] != NULL){
			execute(args);
		}

	}


	//Atexit
	if(atexit(printPrompts)!=0){
		perror("Atexit");
		exit(1);
	}
	if(atexit(writeCounter)!=0){
		perror("Atexit");
		exit(1);
	}
	if(atexit(endOfProgram) != 0){
		perror("Atexit");
		exit(1);
	}

	
}


void read_line(char *line){
	currentDirectory();
	printf(">");
	fgets(line,MAX_LINE, stdin);
	if(strlen(line)!=0)
		line[strlen(line)-1] = '\0'; //Delete new line
}

void separate_tokens(char *line, char **args){
	int i=0;
	args[i] = strtok(line, " ");
	while(args[i] != NULL){
		i++;
		args[i] = strtok(NULL, " ");
	}
}

void cdHome(){
	if(chdir(getenv("HOME"))!=0){
		perror("Error changing");
	}	numberOfPrompts++;

}

void cdCustom(char *path){
	if(chdir(path)!=0){
		perror("");
	}	numberOfPrompts++;
}

void currentDirectory(){
	char *arguments[3] = {"pwd","-P",NULL};
	int status;
	pid_t pid = fork();

	switch(pid){
		case -1: //Fallo
			perror("Error en fork");
			exit(-1);
		case 0: //En hijo
			execvp(arguments[0],arguments);
			perror("Error in exec");
			exit(-1);
		default:
			while(wait(&status)!=pid);
			if (status == 0){
				printf("");
			}
			else 
				printf("Error en el hijo\n");
	}
}

void execute(char **args){
	int status;
	pid_t pid = fork();

	switch(pid){
		case -1: //Fallo
			perror("Error en Fork");
			exit(-1);
		case 0:  //En Hijo
			execvp(args[0],args);
			perror("Error in exec");
			exit(-1);
		default:
			while(wait(&status)!=pid);
			if(status ==0){
				numberOfPrompts++;
				printf("");
			}
	}
}

void executePrograms(char **args){
	int status;
	pid_t pid = pidProgram = fork();

	switch(pid){
		case -1: //Fallo
			perror("Error en Fork");
			exit(-1);
		case 0:  //En Hijo
			execvp(args[0],args);
			perror("Error in exec");
			exit(-1);
		default:
			signal(2,killProgram);
			while(wait(&status)!=pid);
			if(WIFEXITED(status)){
				printf("\nTAREA %d TERMINADA CON ESTADO %d\n",pid,WEXITSTATUS(status));
				signal(2,SIG_DFL);

			} else if ( WIFSIGNALED(status)){
				printf("\nTAREA %d INTERRUMPIDA POR LA SEÑAL %d\n",pid,WTERMSIG(status));
				signal(2,SIG_DFL);
			}
			numberOfPrompts++;
	}
}

void killProgram(){
	kill(pidProgram,2);
}

void writeCounter(){
	FILE *fPtr;

	if((fPtr=fopen("Counter.txt","w"))==NULL){
		printf("Error abriendo archivo\n");
	} else {
		fprintf(fPtr,"%d",numberOfPrompts);
		fclose(fPtr);
	}
}

void endOfProgram(){
	printf("MiniShell Finalizado (:\n");
}

void printPrompts(){
	printf("Prompts used: %d\n",numberOfPrompts);
}
