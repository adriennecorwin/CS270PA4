
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARGUMENTS 1



















int main(int argc, const char *argv[]) {

char user_answer [200];
int exit_status;
char c;
        //if the argument count is not equal to ARGUMENTS, or the value of 2
        if (argc != ARGUMENTS)
        {
                //prints to the user how to execute the program
                printf("Usage: ./prog_4\n");

        }

	else
	{
		while(1)
		{
			printf("msh > ");
			scanf("%s %i", user_answer, &exit_status);


if((c = getchar()) == EOF )
{

exit(0);

}


			else if (strcmp(user_answer, "done") == 0)
			{
				printf("%i", exit_status);
				exit(exit_status);
			}
		}
	}
}
