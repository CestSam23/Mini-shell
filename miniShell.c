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
		} else if (args[0] != NULL){
			execute(args);
		}
		
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
	}
}

void cdCustom(char *path){
	if(chdir(path)!=0){
		perror("");
	}
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
			if (status == 0)
				printf("");
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
			if(status ==0)
				printf("");
			else
				printf("No se que hiciste mal Sam\n");
	}
}
