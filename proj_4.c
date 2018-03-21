
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARGUMENTS 1
#define TOKENSPLIT " \t\r\n\a"
#define TOKENSIZE 255

char **splitLine(char *user_answer)
{
	char *token;
	token=strtok(user_answer, TOKENSPLIT);
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

char user_answer [200];
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
			scanf("%s", user_answer);
			if ((strcmp(user_answer, "done") == 0))
			{
				done();
			}
			else if ((strcmp(user_answer, "setPrompt") == 0))
			{
			}
			
		}
	}
}
