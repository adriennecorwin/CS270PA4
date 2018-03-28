#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define SIZE 255
#define ARGUMENTS 1
#define TOKENSPLIT "\"  \t\r\n\a\"\""
#define TOKENSIZE 255





pid_t pid;
int child_status;


int valide(char **array, char prompt[], int size)
{
	//int count = 0;
	if (strcmp(array[0], "done") == 0)
	{
		//              sscanf(*array, "done %s %s", array[1], array[2]);
		if (isdigit(*array[1]) && array[2] == '\0')
		{
			printf("param = %s\n", array[1]);

			exit(*array[1]);
		}
		else if(!isdigit(*array[1]) && array[2] == '\0')
		{
			printf("Parameter to done must be a non-negative integer.\n");

		}
		else
			printf("too many parameters to done\n");

		for (int k = 0; k < size; k++)
		{
			array[k] = NULL;

		}

		return 1;


	}


	else if (strcmp(array[0], "done\n") == 0)
	{
		return 2;
	}
	/*else if (strcmp(array[0], "") == 0)
	  {

	  return 2;
	  }*/

	if (strcmp(array[0], "setvar") == 0)
	{
		//printf("ooooooooooooo");


		/*      sscanf(*array, "setvar %s %s", array[1], array[2]);

			if (array[1] != '\0' && array[2] != '\0')
			{
			printf("array[1] = %s\n", array[1]);
		//return 4;

		}
		else
		return 4;
		 */


		if (array[3] == '\0' && array[2] != '\0')
		{
			//strcpy(prompt, array[1]);
			//printf("expect 3");
			return 3;
		}
		else
			printf("expected 3 tokens, but got %i tokens\n", size);


		for (int k = 0; k < size; k++)
		{
			array[k] = NULL;

		}

		return 3;
	}

	else if (strcmp(array[0], "setvar\n") == 0)
	{
		//      printf("error\n");
		return 4;
	}

	if (strcmp(array[0], "setprompt") == 0)
	{

		/*  int n = 0;
		    for (n = 0; *array[n]; n++)
		    {

		    }

		    int n = strlen(array);
		    printf("%i\n", n);
		 */
		//sscanf(*array, "setprompt %s %s", array[1], array[2]);

		if (array[2] == '\0')
		{
			strcpy(prompt, array[1]);
			//printf("expect 3");
		}
		else
			printf("expected 2 tokens, but got %i tokens\n", size);


		for (int k = 0; k < size; k++)
		{
			array[k] = NULL;

		}

		/*
		   array[1] = NULL;
		   array[2] = NULL;*/
		/*if (strcmp(array[2], "\0") == 0)
		  {
		  printf("lol\n");
		  }*/
		//printf("yo");
		//printf("too many");

		return 6;
		//              return 5;
	}

	else if (strcmp(array[0], "setprompt\n") == 0)
	{
		//              printf("error2");
		return 6;
	}
	if (strcmp(array[0], "setdir") == 0)
	{
		printf("hi\n");

		if(array[2] == '\0')
		{
			printf("%s\n", array[1]);
			chdir(array[1]);

		}
		/*if (n == 0)
		  {
		  printf("goooooooooooooo\n");
		  }
		 */


		return 7;
		//      }
		/*      else
			{
		//printf("too many");
		return 6;
		}
		 */
}
else if (strcmp(array[0], "setdir\n") == 0)
{
	printf("error\n");
	return 6;
}
if (strcmp(array[0], "showprocs") == 0)
{
	printf("ha\n");
	return 8;
}
else if (strcmp(array[0], "showprocs\n") == 0)
{
	printf("error\n");
	return 8;
}
//**********************************use fork and exec*************************
if (strcmp(array[0], "run") == 0)
{
	printf("run!\n");

	pid = fork();
	if (pid == 0) { // child
		//printf("The child is running. The child is about to run %s %s\n",
		//              cmd, param);

		char *arg[size];
		for (int j = 0; j < size; j++)
		{

			arg[j] = array[j+1];


			if (j == size - 1)
			{
				arg[j] = NULL;
			}

		}

//		printf("%s\n", arg[0]);


		execvp(arg[0],arg);
		//child_status = wait(&child_status);
		//pid_t getpid(void);
		// These lines should not be executed
		printf("The child failed to exec the program.\n");
		exit(1);
	} else { // parent
		//printf("The parent started a child to run %s %s.\n", cmd, param);
		(void) wait(&child_status); // block until child terminates /

		//if (wait(&child_status) >= 0)
		//{
		if (WIFEXITED(child_status) && !WEXITSTATUS(child_status)) {
			printf("Child exited normally.\n");
		} else {
			printf("Child exited abnormally.\n");
		}
		//}
	} // parent







	for (int k = 0; k < size; k++)
	{
		array[k] = NULL;

	}

	return 10;
}

else if (strcmp(array[0], "run\n") == 0)
{
	printf("error!\n");
	return 11;
}
if (strcmp(array[0], "fly") == 0)
{
	printf("fly\n");


 pid = fork();
        if (pid == 0) { // child
                //printf("The child is running. The child is about to run %s %s\n",
                //              cmd, param);

                char *arg[size];
                for (int j = 0; j < size; j++)
                {

                        arg[j] = array[j+1];


                        if (j == size - 1)
                        {
                                arg[j] = NULL;
                        }

                }

                printf("%s\n", arg[0]);


                execvp(arg[0],arg);
                child_status = wait(&child_status);
                //pid_t getpid(void);
                // These lines should not be executed
                printf("The child failed to exec the program.\n");
  
             exit(1);
        } 

else if (pid < 0)
{

printf("error\n");
return -1;

}

else { // parent
                //printf("The parent started a child to run %s %s.\n", cmd, param);
              //  (void) wait(&child_status); // block until child terminates /

                //if (wait(&child_status) >= 0)
                //{
                if (WIFEXITED(child_status) && !WEXITSTATUS(child_status)) {
                        printf("Child exited normally.\n");
                } else {
                        printf("Child exited abnormally.\n");
                }
                //}
        } // parent






	return 12;
}

else if (strcmp(array[0], "fly\n") == 0)
{
	printf("error\n");
	return 13;
}
if (strcmp(array[0], "tovar") == 0)
{

	printf("tovar\n");
	return 14;
}

else if (strcmp(array[0], "tovar\n") == 0)
{

	printf("error\n");
	return 15;
}


return -1;




}

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
	printf("size = %i\n", store_index);
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
		printf("Usage: %s\n", argv[0]);
	}
	else
	{
		strcpy(prompt, "msh");
		while(1)
		{
			printf("%s > ", prompt);

			fgets(user_answer, SIZE, stdin);


/*if (fgets(user_answer, SIZE, stdin) == 0)
{
exit(0);
}
*/

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


			// int store_index = 0;//*********************************

			int size_of_array = 0;
			array = splitLine(user_answer);//**************************
			printf("%s\n", array[0]);
			printf("%s\n", array[1]);


//if (array[2] == '\0')
//{

printf("%s\n", array[2]);
//printf("null");
//}
			//int l =sizeof(array)/sizeof(char**);
			/*for (l = 0; array[l] != NULL; l++)
			  {

			  }
			 */

			for(int i=0; i<strlen(user_answer); i++){

				if (user_answer[i] == ' ' || user_answer[i] == '\n')
				{

					size_of_array++;
				}


			}


for (int j = 0; j <= size_of_array; j++)
{
if ( j == size_of_array)
{
array[j] = NULL;

}

}





			printf("size2 = %i\n", size_of_array);


			int check = valide(array, prompt, size_of_array);
			//count = 0;

			if (check == 2)
			{
				exit(0);
			}

			if (check == 3)
			{
				printf("hiiiiiiiiiii");

			}


			if (check == 6)
			{
				printf("expected 2 tokens, got 1 tokens.\n");


			}
			if (check == 4)
			{

				printf("expected 3 tokens, got 1 tokens.\n");
			}
			if (check == 13)
			{
				printf("error\n");

			}
			if (check == -1)
			{
				//*                             if(array[0] != '\n')
				//                              {
				printf("invalid command %s", array[0]);
				//                              }
				//                              else*/
				//                                      printf("%s > ", prompt);
			}
























			//			printf("%s\n", array[1]);
			/*
			   printf("%s1\n", setvar);
			   int r = strcmp(setvar, array[0]);
			   printf("%i", r);*/
			/*			if ((strcmp(user_answer, "done") == 0))
						{
						done();
						}*/
			/*						if (strcmp(array[0],"setvar") == 0)
									{

									printf("hey");
									}*/

		}
	}
}
