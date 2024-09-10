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
char *current_directory();

int main(){


	while(1){
		
	}
}



void read_line(char *line){
	printf("%s>",current_directory());
	fgets(line,MAX_LINE, stdin);
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

char *current_directory(){
	return "  ";
}

