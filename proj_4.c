
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255
#define ARGUMENTS 1
#define TOKENSPLIT "\"  \t\r\n\a\"\""
#define TOKENSIZE 255

char **splitLine(char user_answer[])
{
	char **token_storage = (char**)malloc(SIZE*sizeof(char*)); //array that store tokens
	for (int i=0; i<SIZE; i++){
		token_storage[i]=(char*)malloc(SIZE*sizeof(char));
	}

	int store_index = 0;
	int token_index = 0;
	char token[SIZE];
	int open_quote=0, end_quote=0, open_space=0, end_space=0;
	for(int i=0; i<strlen(user_answer); i++){
		if (user_answer[i]=='"'){
			for (int i=0; i<SIZE; i++){
				token[i]='\0';
			}
			open_quote=i;
			printf("openquote: %i\n", open_quote);
			while(user_answer[i+1]!='"' && i<strlen(user_answer)){
				i++;
			}
			end_quote=i+1;
			i++;
			printf("q: %i\n", end_quote);
			token_index=0;
			for(int j=open_quote+1; j<end_quote; j++){
				token[token_index]=user_answer[j];
				token_index++;
			}
			printf("s: %i\n", store_index);
				for (int k = 0; k<strlen(token); k++){
					token_storage[store_index][k]=token[k];
				}
//			token_storage[store_index]=token;
			printf("token: %s\n", token_storage[store_index]);
			store_index++;
		}
		else if((user_answer[i]==' ' && user_answer[i-1]!='"') || (user_answer[i]=='\n' && user_answer[i-1]!='"')){
			for (int i=0; i<SIZE; i++){
				token[i]='\0';
			}
			printf("si: %i\n", store_index);
			end_space=i;
			while(user_answer[i-1]!=' ' && i-1>=0){
				i--;
			}
			open_space=i-1;
			printf("openspace: %i\n", open_space);
			i=end_space;
			token_index = 0;
			for(int j=open_space+1; j<end_space; j++){
				printf("j: %i\n", j);
				token[token_index]=user_answer[j];
				token_index++;
			}
			printf("s: %i\n", store_index);
		//	token_storage[store_index]=token;
				for (int k=0; k<strlen(token); k++){
					token_storage[store_index][k]=token[k];
				}
			printf("token: %s\n", token_storage[store_index]);
		//	printf("token: %s\n", token_storage[0]);
			store_index++;
			printf("endspace: %i\n", end_space);
		}
	}

return token_storage;
}





void done()
{
	int exit_status;
	scanf("done %i",&exit_status);
	printf("%i", exit_status);
	exit(exit_status);
}




int main(int argc, const char *argv[]) {
	char **array;
	char user_answer[SIZE];
	char prompt [200];
	//if the argument count is not equal to ARGUMENTS, or the value of 2
	if (argc != ARGUMENTS)
	{
		//prints to the user how to execute the program
		printf("Usage: ./prog_4\n");
	}
	else
	{
		strcpy(prompt, "msh");
		while(1)
		{
			printf("%s > ", prompt);

			fgets(user_answer, SIZE, stdin);




			/*			for (int k = 0; k < sizeof(user_answer); k++)
						{

						if (user_answer[k] == '\"')
						{
						k++;

						while(user_answer[k] != '\"')
						{
						printf("%s", user_answer[k]);
						k++;

						}


						}

						else {
						array = splitLine(user_answer);
						}


						}

			 */




			array = splitLine(user_answer);//**************************
			printf("%s\n", array[0]);
//			printf("%s\n", array[1]);
			/*
			   printf("%s1\n", setvar);
			   int r = strcmp(setvar, array[0]);
			   printf("%i", r);*/
			/*			if ((strcmp(user_answer, "done") == 0))
						{
						done();
						}*/
						if (strcmp(array[0],"setvar") == 0)
						{

						printf("hey");
						}

		}
	}
}
