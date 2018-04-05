
Files:

proj4.c
makefile
README.txt

Group Members:

Tabitha Holloway
Adrienne Corwin

Description:

We stored each variable, variable value, background process, both pid and type of background process, and token into a dynamically allocated array that held elements of similar types. For example, a tokens array held the tokens, while a variable value array held the values of the variables the user set. Not only so, but following with the way of the working program works, the program accepts any variable name that starts with a letter, but the variable can have special characters in the middle. Unusual library routines include #include <unistd.h> and #include <wait.h>, which were used for the parent and child processes.

Limitations:

One limitation of our program is that the program can accept comments in the input as long as the comments are not on a line by itself. For example, if the user types something like:
run ls #list everything
setvar #
then the program will run correctly. If the user only types something like:
#hello
then the program will run into a segmentation fault.
Another limitation is that if the user does not type in endquotes, then the token takes everything past the beginning quote until the user inputs endline. Therefore, unmatched quots do not give an error or warning to the user.
Another limitation is that sometimes, when the user runs /bin/sleep after using fly /bin/sleep, the program will not print to the user that the fly /bin/sleep has completed.
Another limitation is that the array in tovar does not have a large enough size to hold the entire output for some of the commands if the output has a over 255 charaacters.
