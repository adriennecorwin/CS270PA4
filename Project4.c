#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255
#define ARGUMENTS 1
#define TOKENSIZE 255
#define DONE "done"
#define SETVAR "setvar"
#define SETPROMPT "setprompt"
#define SETDIR "setdir"
#define SHOWPROCS "showprocs"
#define RUN "run"
#define FLY "fly"
#define TOVAR "tovar"

int show_tokens = 1;
//break user input into tokens
char **scan(char user_answer[])
{       //SIZE????
	char **token_storage = (char**)malloc(SIZE*sizeof(char*)); //initialize 2D array that store tokens
	for (int i=0; i<SIZE; i++){
		token_storage[i]=(char*)malloc(SIZE*sizeof(char));
	}

	int store_index = 0; //indexes token_storage
	int token_index = 0; //indexes token
	int open_quote=0, end_quote=0, open_space=0, end_space=0; //initialize ...
	for(int input_index=0; input_index<strlen(user_answer); input_index+=1){ //iterate through user input
		if (user_answer[input_index]=='#'){ //ignore any user input after # (comment)
			break;
		}
		else if (user_answer[input_index]=='"'){ //if there is a double quote
			open_quote=input_index; //open quote is at input_index
			while(user_answer[input_index+1]!='"' && user_answer[input_index+1]!='\n'){//input_index<strlen(user_answer)){ //finds next double quote
				input_index+=1;
			}
			end_quote=input_index+1;
			//                      if(user_answer[end_quote]=='\n'){
			//                              printf("Error: unmatched double quote\n");
			//                              break;
			//                      }
			printf("%i", end_quote);
			input_index+=1; //increment so that for loop will continue scanning after second double quote
			token_index=0;
			for(int quotes_index=open_quote+1; quotes_index<end_quote; quotes_index+=1){ //grab characters inside quotes and store as token in array of tokens
				token_storage[store_index][token_index]=user_answer[quotes_index];
				token_index+=1;
			}
			store_index+=1; //indicate token_storage has one token in it so next token found goes in next space
			}//end if

			//if there is a space or newline (don't tokenize space between two endquotes)
			else if((user_answer[input_index]==' ' && user_answer[input_index-1]!='"' && user_answer[input_index-1]!=' ') || (user_answer[input_index]=='\n' && user_answer[input_index-1]!='"' && user_answer[input_index-1]!=' ')){
				end_space=input_index; //end space is at i
				while(user_answer[input_index-1]!=' ' && input_index-1>=0){ //find where the previous space was or the beginning or user input
					input_index--;
				}
				open_space=input_index-1;
				input_index=end_space; //so for loop will continue scanning after the endspace
				token_index = 0;
				for(int space_index=open_space+1; space_index<end_space; space_index+=1){ //grab characters between two spaces and store as token in array of tokens
					token_storage[store_index][token_index]=user_answer[space_index];
					token_index+=1;
				}
				store_index+=1;
			}//end else if
		}//end for loop
		if (show_tokens==1){
			for(int token_num=0; token_num<=store_index; token_num+=1){
				printf("%s\n", token_storage[token_num]);
			}
		}
		return token_storage;
	}//scanner()

	//analyze line for correctness
	void parse(char **tokens)
	{	
		int num_tokens=0;
		for(int size_index=0; tokens[size_index]=='\0'; size_index+=1){
			num_tokens+=1;
		}
		if (strcmp(tokens[0], DONE) == 0)
		{
			if((num_tokens==1) || (num_tokens==2 && isdigit(*tokens[1]))){
				done(tokens[1]);
			}
			else if(num_tokens>2)	
				printf("Too many parameters to done\n");
		}
		else{
			printf("Parameter to done must be a non-negative integer.\n");
		}

		else if (strcmp(tokens[0], SETPROMPT) == 0){
			if(num_tokens==2){
				set_prompt(tokens[1]);
			}
			else{
				printf("Expected 2 tokens but got %i tokens\n", num_tokens);
			}
			else if (strcmp(tokens[0], RUN) == 0){
				run(tokens);
			}
			else if (strcmp(tokens[0], FLY) == 0){
				fly(tokens);
			}
			else if (strcmp(tokens[0], SETVAR) == 0){
				if(num_tokens==3 && isalpha(*tokens[1][0])){
					setvar(tokens[0], tokens[1]);
				}
				else if(!isalpha(*tokens[1][0])){
					printf("Not valid variable name\n")
				}
				else{
					printf("Expected 3 tokens but got %i tokens\n", num_tokens);
				}
			}
			else if (strcmp(tokens[0], SETDIR) == 0){
				if(num_tokens==2 && (tokens[1][0] == '/' || isalpha(*tokens[1][0]) || tokens[1][0] == '.' || tokens[1][0] == '^')){
					setdir(tokens[1]);
				}
				else if (num_tokens!=2){
					printf("Expected 2 tokens but got %i tokens\n", num_tokens);
				}
				else{
					printf("Not valid directory name\n");
				}
			}
			else if (strcmp(tokens[0], SHOWPROCS) == 0){
				if(num_tokens==1){
					showprocs();
				}
				else{
					printf("Expected 1 tokens but got %i tokens\n", num_tokens);
				}
			}
			else if (strcmp(tokens[0], TOVAR) == 0){
				if(num_tokens>=3){
					tovar(tokens);
				}
				else{
					printf("Insufficient parameters\n");
				}
			}
			else{
				printf("Invalid command %s\n", tokens[0]);
			}
		}
	}
}

int main(int argc, const char *argv[]) {
	char **tokens;
	char *user_answer = malloc(sizeof(char)*SIZE);
	char *prompt = malloc(sizeof(char)*SIZE);
	//if the argument count is not equal to ARGUMENTS, or the value of 2
	if (argc != ARGUMENTS)
	{
		//prints to the user how to execute the program
		printf("Usage: %s\n", argv[0]);
	}
	else
	{
		strcpy(prompt, "msh");
		while(1)
		{
			printf("%s > ", prompt);

			fgets(user_answer, SIZE, stdin);
			int size_of_array = 0;
			tokens = splitLine(user_answer);//**************************
			parse(tokens);
		}
	}
}
