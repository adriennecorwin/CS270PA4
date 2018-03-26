
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255
#define ARGUMENTS 1
#define TOKENSPLIT "\"  \t\r\n\a\"\""
#define TOKENSIZE 255

char **splitLine(char user_answer[])
{
	char* token;
	char **token_storage; //array that store tokens
	int quote_count=0;
//	int* positions[SIZE];
	//tokenizing *********************************
//	int size = sizeof(user_answer);
//	printf("size: %i", size);
	for (int j=0; user_answer[j]!=NULL; j+=1){
//		positions[j]=0;
		if (user_answer[j] == '"'){
			quote_count+=1;
		//	positions[j]=1;
		}
		printf("count: %i\n", quote_count);
	}
	if (user_answer[0] == '"'){
		printf("HELLO");
		token = strtok(user_answer, "\"");
		for(int i = 0; token != NULL; i += 1)
		{
			printf("token: %s\n", token);
			token_storage[i] = token;
			token = strtok(NULL, TOKENSPLIT);
			printf("token_storage: %s\n", token_storage[i]);
		}
	}
	else{
		int j=0;
		while (quote_count!=2){
			if (user_answer[j] == '"'){
				quote_count+=1;
			}
			j++;
		}
	/*	char* ans1;
		strncpy(ans1, user_answer, j);
		printf("%i\n", j);
		for (int i=0; i<j; i++){
			ans1[i]=user_answer[i];
		}
		printf("substring: %s\n", ans1);*/
		token = strtok(user_answer, TOKENSPLIT);
		for(int i = 0; token != NULL; i += 1){
			printf("token: %s\n", token);
			token_storage[i] = token;
			token = strtok(NULL, "\"");
			printf("token_storage: %s\n", token_storage[i]);
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

void setPrompt(char * newPrompt)
{

}




int main(int argc, const char *argv[]) {
	char setvar [] = "Setvar";

	char **array;
	char user_answer[SIZE];
	int exit_status;
	char c;
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
			array = NULL;
//			printf("%s\n", array[0]);
			/*
			   printf("%s1\n", setvar);
			   int r = strcmp(setvar, array[0]);
			   printf("%i", r);*/
			/*			if ((strcmp(user_answer, "done") == 0))
						{
						done();
						}*/
			if (strcmp(user_answer,"setvar") == 0)
			{

				printf("hey");
			}

		}
	}
}
