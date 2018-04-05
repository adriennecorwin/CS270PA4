#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wait.h>
#include <unistd.h>

//set constants
#define SIZE 255
#define ARGUMENTS 1
#define DONE "done"
#define SETVAR "setvar"
#define SETPROMPT "setprompt"
#define SETDIR "setdir"
#define SHOWPROCS "showprocs"
#define RUN "run"
#define FLY "fly"
#define TOVAR "tovar"

//finds the position, in the array back_pid, of the finished background process
int finished_pid(int which_pid, pid_t *back_pid, int *num_backPids){
        int find_pos, position;//find_pos is where index of the finished background process is, and position what will be returned
        for (find_pos = 0; find_pos < *num_backPids; find_pos += 1){
                if (back_pid[find_pos] == which_pid){
                        position = find_pos;
                        break;
                }
        }
        return position;
}
//updates the back_pid array
pid_t *update_bp(int position, pid_t *back_pid, int *num_backPids){
        (*num_backPids)--;//once a background process finishes, the function deletes the pid associated with the finished background process
        for (int index = position-1; index < *num_backPids; index += 1){
                back_pid[index] = back_pid[index + 1];
        }
        return back_pid;
}
//updates the back_type array
char **update_bt(int position, char **back_type, int *num_backType){
        (*num_backType)--;//once a background process finishes, the function deletes the type of background process associated with the finished background process
        for (int index = position-1; index < *num_backType; index += 1){
                back_type[index] = back_type[index + 1];
        }
        return back_type;
}

//when called, exits the program with exit status given by user
void done(char *exit_status, int num_tokens, pid_t *back_pid, int num_backPids, int status, char **back_type, int num_backType){
        if (num_tokens==1){ //if user types done with no exit status

	int which_pid;//variable for which background process has finished
	//calls waitpid
	while ((which_pid = waitpid(-1, &status, WNOHANG)) > 0) {
        	int position = finished_pid(which_pid, back_pid, &num_backPids);//finds position of the finished process in the back_pid array
                for (int index = 0; back_type[index] != '\0'; index +=1){//determines the type of the background process that finished
                	if (index == position){
                        	printf("Completed: %s\n", back_type[index]);//prints to user which background process finished
                                break;
                        }
                }
                back_pid = update_bp(position, back_pid, &num_backPids);//updates the array back_pid
                back_type = update_bt(position, back_type, &num_backType);//updates the array back_type
        }
                exit(0);//exits the program with exit status 0
        }
        else{
                exit(*exit_status);//exits the program with exit status equal to the user's input
        }
}

//sets shell prompt to new_prompt
void setprompt(char *new_prompt, char *prompt){
	strcpy(prompt, new_prompt);//copies the new prompt into variable prompt
}

//associates a given variable name with the given value using parallel arrays
void setvar(char **tokens, char *name, char *val, char **var_names, char **var_vals, int **num_vars, int **showTokens){
	if(strcmp(name, "showTokens")==0){ //if the user is setting the showToken variable, set the corresponding showTokens variable from main
		**showTokens=atoi(val); 
	}
	int reassign_index=0;
	//check if the user is reassigning a variable value
	while (reassign_index<**num_vars && strcmp(var_names[reassign_index], name)!=0){
		reassign_index+=1;
	} //finding if the variable name they are setting was already set previously
	if(reassign_index!=**num_vars){ //if it was, reassign the variable value instead of making a new variable
		for(int val_index=0; val_index<SIZE; val_index+=1){
			var_vals[reassign_index][val_index]=val[val_index];
		}
	}
	else{	//if they are not reassigning, make a vew variable and add it to the end of the parrallel arrays
    	   	**num_vars+=1; //as long as you are not reassigning you are adding 1 var to the array
        	for(int name_index=0; name_index<strlen(name); name_index+=1){ //add variable name given by user to variable name array
                	var_names[**num_vars-1][name_index]=name[name_index];
        	}
        	for(int val_index=0; val_index<strlen(val); val_index+=1){ //add variable value given to corresponding index in variable value array
                	var_vals[**num_vars-1][val_index]=val[val_index];
        	}
	}
	if (strcmp(tokens[1], "PATH") == 0)//if the PATH variable was changed, then reset the variable to its new value
	{
		setenv(var_names[0],var_vals[0], 1);
	}

}

//changes current directory name to newdir--given by user
void setdir(char *newdir){
	int check; //check that changing the directory to new dir is valid
	check = chdir(newdir);
	if(check!=0){
		printf("File directory does not exist\n");
	}
}
//if the user runs run, then it will run the child process in the foreground
void run(char **tokens, char **var_names, char **var_vals){
	setenv(var_names[0], var_vals[0], 0);//set environment for child
	getenv("PATH");//get environment for child
	pid_t pid; //variable for parent id
	int child_status; //status of child process
	pid = fork(); //creates a child
        if(pid<0){ //fork error
                fprintf(stderr, "Fork failed\n");
        } 
        if (pid == 0) { // child
        	char *command_array[SIZE]; //array that holds everything from the input except the token run
        		for (int j = 0; j < SIZE; j++){ //store all tokens (except run) into command_array
        		command_array[j] = tokens[j+1];
        		if (j == SIZE - 1){
        			command_array[j] = NULL; //sets last element to null
        		}
        	}//end for loop
        	execvp(command_array[0],command_array); //runs execvp
        	// These lines should not be executed
        	printf("The child failed to exec the program.\n");
        	exit(1); 
        }//child 
	else { // parent
        	(void) wait(&child_status); // block until child terminates /
        	if (WIFEXITED(child_status) && !WEXITSTATUS(child_status)) {
        		//child exited normally
        	} 
		else {
        		printf("Child exited abnormally.\n");//child did not exit normally
        	}
        } // parent

}
//if user runs fly, then it will run a child process in the background
int fly(char **tokens, char **var_names, char **var_vals, pid_t *back_pid, int **num_backPids, int** status,  char **back_type, int **num_backType){
	setenv(var_names[0], var_vals[0], 0);//set environment for child
        getenv("PATH");//get environment for child
        pid_t pid;  //variable for parent id
        pid = fork();//creates a child
	if(pid<0){ //fork error 
                fprintf(stderr, "Fork failed\n");
        }
        if (pid == 0) { // child
                char *command_array[SIZE];//array that holds everything from the input except the token fly
                for (int j = 0; j < SIZE; j++){ //store all tokens (except fly) into command_array
                        command_array[j] = tokens[j+1];
                        if (j == SIZE - 1){
                                command_array[j] = NULL; //sets last element to null 
                        }
                }
                execvp(command_array[0],command_array); //runs execvp 
                fflush(stdout);
                // These lines should not be executed
                printf("The child failed to exec the program.\n");
                exit(1);
        }//child
        else{ // parent
 		back_type[**num_backType] = tokens[1]; //stores the type of background processe into the array back_type
                (**num_backType)++;//increases the number of background processe type by one

                back_pid[**num_backPids] = pid; //stores the parent id of the background processes into an array back_pid
                (**num_backPids)++;//increases the number of background process pid by one
                return **status; //returns status of the child
        } // parent
}
//if user runs showprocs, then the program will list the current background processes that are running
void showprocs(char **tokens, pid_t *back_pid, int **num_backPids, char **back_type, int **num_backType){
        for (int index = 0; index < **num_backType; index += 1){
                printf("%s\n",back_type[index]); //prints the current background processes running
        }
        if (**num_backPids == 0){ //if there are no background processes currently running
                printf("No Background Processes.\n");
        }
}
//if the user runs tovar, then sets a variable, that the user input, equal to the standard output (stdout)
void tovar(char **tokens, char **var_names, char **var_vals, int **num_vars){
	signal(SIGPIPE, SIG_IGN);//captures signal
	setenv(var_names[0], var_vals[0], 0);//set environment for child
        getenv("PATH");//get environment for child
	pid_t pid; //variable for parent id
        int child_status;//variable for child status
	int fd[2]; //file descriptor for reading from stdout and writing into a variable
int size = SIZE * 2;
	char output[size];//array that holds what the variable will store
	pipe(fd);//calls pipe funtion
	if(pipe(fd)==-1){ //pipe error
		fprintf(stderr, "Pipe failed\n");
	}
        pid = fork();//creates a child
	if(pid<0){ //fork error
		fprintf(stderr, "Fork failed\n");
	}
        if (pid == 0) { // child
                char *command_array[SIZE];//array that holds everything from the input except the token tovar and the variable that was part of input
                for (int j = 0; j < SIZE; j++){
                        command_array[j] = tokens[j+2];
                        if (j == SIZE - 1){
                                command_array[j] = NULL;//sets last element to null
                        }
                }
		dup2(fd[1], 1);//duplicates the file descriptor for write
		close(fd[0]);//closes file descriptor for reading
                execvp(command_array[0],command_array);//runs execvp
                // These lines should not be executed
                printf("The child failed to exec the program.\n");
                exit(1);
        }
        else { // parent
	int count =read(fd[0], output, sizeof(output));//reads from file descriptor 0
		int find_var=0;//used if a variable is being reassigned
		while(find_var<**num_vars && strcmp(var_names[find_var], tokens[1])!=0){
			find_var+=1;//increments variable by 1 if the name of the variable is not similar to an existing variable in the array var_names
		}
		if(find_var!=**num_vars){ //if the variable is not being reassigned
			for(int set_index =0; set_index<sizeof(output); set_index+=1){
				var_vals[find_var][set_index]=output[set_index];//inserts value of the variable into array var_vals
			}
		}	
		else{//reassign the variable
			**num_vars+=1;
 			for(int set_index =0; set_index<sizeof(output); set_index+=1){
				var_names[**num_vars-1][set_index]=tokens[1][set_index];//sets name of the variable in var_names
			}
			for(int set_index =0; set_index<sizeof(output); set_index+=1){
                		var_vals[**num_vars-1][set_index]=output[set_index];//sets value of variable in var_vals
        		}
		}	
        	(void) wait(&child_status); // block until child terminates 

        	if (WIFEXITED(child_status) && !WEXITSTATUS(child_status)) {
 			//child exited normally
        	}
        	else {
                	printf("Child exited abnormally.\n");//child did not exit normally
        	}	
	} // parent
}

//if user enters a ^ infront of one of their tokens, look for a matching variable name and replace ^token with its value
char **replace_var(char **tokens, char **var_names, char **var_vals, int num_tokens, int num_vars, int *carrot_indexes){
	int var_found_num=0, replace_index=0;
	char **vars_found = (char**)malloc(SIZE*sizeof(char*)); //initialize 2d array to hold each variable that had a carrot in front of it
	for (int i=0; i<SIZE; i++){
                vars_found[i]=(char*)malloc(SIZE*sizeof(char));
        }
	for(int carrot_index=0; carrot_index<num_tokens; carrot_index+=1){//go through carrot array and find which indexes (which correspond to token indexes) had carrots
		if(carrot_indexes[carrot_index]==1){
			for(int var_index=1; var_index<(strlen(tokens[carrot_index])); var_index+=1){ //start at index 1 to copy the variable name without the carrot into var_found array
				vars_found[var_found_num][var_index-1]=tokens[carrot_index][var_index];
			}
			var_found_num+=1; //count number of variable names with carrots you are storing in vars_found
		}
	}
//	if(var_found_num>0){ //double check that there was in fact a variable with a carrot (first check was in main)
		for(int var_found_index=0; var_found_index<var_found_num; var_found_index+=1){ //for every variable with a carrot you found
			int match_index=0;
			while(match_index<num_vars && strcmp(var_names[match_index], vars_found[var_found_index])!=0){ //find the index of that matching variable name in the var_names array
				match_index+=1;
			}
			if(match_index!=num_vars){ //if the variable name was found in the var_names array
				while(replace_index<num_tokens && carrot_indexes[replace_index]!=1){
					replace_index+=1;
				}//find the index of the token you need to replace
				for(int token_index=0; token_index<SIZE; token_index+=1){ //replace the token with the variable value
					tokens[replace_index][token_index]=var_vals[match_index][token_index];
				}
				replace_index+=1; 
			}
			else{ //if no variable name already set was found
				printf("This variable name does not exist\n");
			}
		}
	return tokens;
}

//break user input into tokens
char **scan(char user_answer[]){
	char **token_storage = (char**)malloc(SIZE*sizeof(char*)); //initialize 2D array that store tokens
	for (int i=0; i<SIZE; i++){
		token_storage[i]=(char*)malloc(SIZE*sizeof(char));
	}
	int store_index = 0; //indexes token_storage
	int token_index = 0; //indexes token
	int open_quote=0, end_quote=0, open_space=0, end_space=0; //initialize indicators
	for(int input_index=0; input_index<strlen(user_answer); input_index+=1){ //iterate through user input
		if (user_answer[input_index]=='#'){ //ignore any user input after # (comment)
			break;
		}//end if
		else if (user_answer[input_index]=='"'){ //if there is a double quote
			open_quote=input_index; //open quote is at input_index
			while(user_answer[input_index+1]!='"' && user_answer[input_index+1]!='\n'){//input_index<strlen(user_answer)){ //finds next double quote
				input_index+=1;
			}
			end_quote=input_index+1;
			input_index+=1; //increment so that for loop will continue scanning after second double quote
			token_index=0;
			for(int quotes_index=open_quote+1; quotes_index<end_quote; quotes_index+=1){ //grab characters inside quotes and store as token in array of tokens
				token_storage[store_index][token_index]=user_answer[quotes_index];
				token_index+=1;
			}
			store_index+=1; //indicate token_storage has one token in it so next token found goes in next space
		}//end else if

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
	token_storage[store_index]='\0';
	return token_storage;
}//scanner()

//analyze line for correctness
void parse(char **tokens, int num_tokens, int *num_vars, int *showTokens, char **var_names, char **var_vals, char* prompt, pid_t *back_pid, int *num_backPids, int *status, char **back_type, int *num_backType){	
	if(strcmp(tokens[0], DONE) == 0){
		if((num_tokens==1)||(num_tokens==2 && isdigit(*tokens[1]))){
			done(tokens[1], num_tokens, back_pid, *num_backPids, *status, back_type, *num_backType);
		}
		else if(num_tokens>2){
			printf("Too many parameters to done\n");
		}
		else{
			printf("Parameter to done must be a non-negative integer.\n");
		}
	}//end if
	else if (strcmp(tokens[0], SETPROMPT) == 0){
		if(num_tokens==2){
			setprompt(tokens[1], prompt);
		}
		else{
			printf("Expected 2 tokens but got %i tokens\n", num_tokens);
		}
	}//end else if
	else if (strcmp(tokens[0], SETVAR) == 0){
                if(num_tokens==3 && isalpha(tokens[1][0])){
                        setvar(tokens, tokens[1], tokens[2], var_names, var_vals, &num_vars, &showTokens);
                }
                else if(!isalpha(tokens[1][0])){
                        printf("Not valid variable name\n");
                }
                else{
                        printf("Expected 3 tokens but got %i tokens\n", num_tokens);
                }
        }//end else if
        else if (strcmp(tokens[0], SETDIR) == 0){
                if(num_tokens==2 && (tokens[1][0] == '/' || isalpha(tokens[1][0]) || tokens[1][0] == '.' || tokens[1][0] == '^')){
                        setdir(tokens[1]);
                }
                else if (num_tokens!=2){
                        printf("Expected 2 tokens but got %i tokens\n", num_tokens);
                }
                else{
                        printf("Not valid directory name\n");
                }
        }//end else if
	else if (strcmp(tokens[0], RUN) == 0){
		if (num_tokens!=1){
			run(tokens,var_names, var_vals);
		}
		else{
			printf("Command run must take at lease one parameter\n");
		}
	}//end else if 
	else if (strcmp(tokens[0], FLY) == 0){
		fly(tokens, var_names, var_vals, back_pid, &num_backPids, &status, back_type, &num_backType);
	}//end else if
	else if (strcmp(tokens[0], SHOWPROCS) == 0){
		if(num_tokens==1){
			showprocs(tokens, back_pid, &num_backPids, back_type, &num_backType);
		}
		else{
			printf("Expected 1 tokens but got %i tokens\n", num_tokens);
		}
	}//end else if
	else if (strcmp(tokens[0], TOVAR) == 0){
		if(num_tokens>=3){
			tovar(tokens, var_names, var_vals, &num_vars);
		}
		else{
			printf("Insufficient parameters\n");
		}
	}//end else if
	else{
		printf("Invalid command %s\n", tokens[0]);
	}//end if else statement
}//parse()

//prints out tokens from user input 
void print_tokens(char **tokens, int num_tokens){
	for(int token_index=0; token_index<num_tokens; token_index+=1){
                        printf("Token: %s\n", tokens[token_index]);
        }
}

//finds the number of tokens 
int get_num_tokens(char **tokens){
	int num_tokens=0;
	for(int size_index=0; tokens[size_index]!='\0'; size_index+=1){ //count number of tokens from user input
		num_tokens+=1;
	}
	return num_tokens;
}

//finds indices of tokens with carrots and stores them in carrot_indexes
void find_carrot(char **tokens, int num_tokens, int *carrot_indexes, int *sum){
	if(num_tokens>0){
		for(int carrot_index=0; carrot_index<num_tokens; carrot_index+=1){
                	if(tokens[carrot_index][0]=='^'){
                        	carrot_indexes[carrot_index]+=1;
                                *sum+=1;
                        }
                }
	}
}
		
int main(int argc, const char *argv[]) {
	char **tokens; //holds tokens from user input
	pid_t *back_pid = malloc(SIZE*sizeof(pid_t));//holds the pid of the background processes
	char **back_type = (char**)malloc(SIZE*sizeof(char*));//holds type of background processes
	int num_backType = 0, num_backPids = 0, status = 0;//holds number of type of background processes, holds number of pid of the background processes, and holds status of the child background processes
	char *user_answer = malloc(sizeof(char)*SIZE); //holds the user input
	char *prompt = malloc(sizeof(char)*SIZE); //holds the shell prompt
	char **var_names = (char**)malloc(SIZE*sizeof(char*)); //initialize 2D array to store variable names
        for (int i=0; i<SIZE; i++){
                var_names[i]=(char*)malloc(SIZE*sizeof(char));
        }
	char **var_vals = (char**)malloc(SIZE*sizeof(char*)); //initialize 2D array to store variable values
        for (int i=0; i<SIZE; i++){
                var_vals[i]=(char*)malloc(SIZE*sizeof(char));
        }

	int showTokens=0, num_tokens=0, num_vars=1;
	strcpy(var_names[0], "PATH");	//set PATH variable default
	strcpy(var_vals[0], "/bin:/usr/bin");
	//if the argument count is not equal to ARGUMENTS, or the value of 2
	if (argc == 2 || argc > 3){
                //prints to the user how to execute the program
                printf("Usage: %s\n", argv[0]);
        }
        else if (argc == 1 || (argc == 3 && strcmp(argv[1], "<") == 0)){//if user runs the program correctly or is feeding in a file to read
		strcpy(prompt, "msh > ");
		while(1){
                        int which_pid;
                        while ((which_pid = waitpid(-1, &status, WNOHANG)) > 0) {
                                int position = finished_pid(which_pid, back_pid, &num_backPids);
                                for (int index = 0; back_type[index] != '\0'; index +=1){
                                        if (index == position){
                                                printf("Completed: %s\n", back_type[index]);
                                                break;
                                        }
                                }
                                back_pid = update_bp(position, back_pid, &num_backPids);
                                back_type = update_bt(position, back_type, &num_backType);
                        }

                        printf("%s", prompt);
			if (fgets(user_answer, SIZE, stdin) == '\0'){ //if user enters ctl-D, exit with status 0
				printf("\n");
				exit(0);
			}
			else{ //else continue running the shell
				tokens = scan(user_answer); //tokenize user answer
				num_tokens=0; //reset number of tokens to zero everytime you have a new user input
				num_tokens=get_num_tokens(tokens); //find number of tokens
				int *carrot_indexes=malloc(num_tokens); //indicates which tokens contain ^
				int sum=0;//number of tokens with ^
				find_carrot(tokens, num_tokens, carrot_indexes, &sum); //finds which tokens contain ^ and stores them in carrot_indexes
				if(sum!=0){//if any of the tokens have a ^
                                        tokens=replace_var(tokens, var_names, var_vals, num_tokens, num_vars, carrot_indexes); //replace variable name with corresponging value
				}
				//parse tokens for valid input and call appropriate functions

				parse(tokens, num_tokens, &num_vars, &showTokens, var_names, var_vals, prompt, back_pid, &num_backPids, &status, back_type, &num_backType);
				if(showTokens==1){ //if user sets showTokens to 1 print tokens
					print_tokens(tokens, num_tokens);
				}
			}
		}
	}
}
