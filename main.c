#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>


char* read_line(void);
char** parse(char* input);
void execute(char** args);
int parentLength(char** args);
int externalExe(char** args);

int main(void) {
    //main function for our shell

    printf("Welcome to aSh!! \n\n");

    while (true) {

        //Our shell will have 3 basic steps:-
        // 1. Read the input i.e the readline function which will take the input from the command line
        // 2. parse function which will break the input string into an array of strings
        // 3. execute function which will execute the command based on the return of parse function!!
        printf("~$ ");
        char* input = read_line();
        
        //printf("%s", input);
        
        char** parsed = parse(input);

        // for (int i = 0; i < strlen(parsed); i++) {
        //     printf("%s\n", parsed[i]);
        // }

        execute(parsed);

        free(input);
        free(parsed);
    
    }

    return 0; // worked correctly
}

char* read_line(void) {
    //Function to take input from stdin using getline function of C
    char* buffer;
    size_t bufSize = 100; // declaring maximum number of characters that can be taken as argument to be 100
    size_t line;

    buffer = (char*)malloc(bufSize * sizeof(char));

    if (buffer == NULL) {
        printf("Error: Couldn't allocate memory :( \n");
        exit(1);
    }

    line = getline(&buffer, &bufSize, stdin);

    return buffer;

}

char** parse(char* input) {
    //parsing the input by breaking it up by spaces, ex - "my friend" will be "["my", "friend"]" 
    // an array of strings basically

    int parentArrayBuffer = 100; // initially giving 100 members to the parent array
    char** parentArray = (char**)malloc(parentArrayBuffer * sizeof(char*));
    int parentArrayCounter = 0;

    int currentArrayBuffer = 1024; // initially giving length to each argument as 1024
    char* currentArray = (char*)malloc(currentArrayBuffer*sizeof(char));
    int currentArrayCounter = 0;

    for (int i = 0; i < strlen(input); i++) {

        char character = input[i];

        if (character == ' ') {

            if (parentArrayCounter + 1 > parentArrayBuffer) {
                parentArrayBuffer += 100;
                parentArray = (char**)realloc(parentArray, parentArrayBuffer*sizeof(char*));
            }

            if (currentArrayCounter + 1 > currentArrayBuffer) {
                currentArrayBuffer += 1024;
                currentArray = (char*)realloc(currentArray, currentArrayBuffer*sizeof(char));
            }

            parentArray[parentArrayCounter] = currentArray;
            parentArrayCounter++;

            currentArray = malloc(currentArrayBuffer * sizeof(char));
            currentArrayCounter = 0;
        
        } else if (character == '\n') {

            if (parentArrayCounter + 1 > parentArrayBuffer) {
                parentArrayBuffer += 100;
                parentArray = (char**)realloc(parentArray, parentArrayBuffer*sizeof(char*));
            }

            if (currentArrayCounter + 1 > currentArrayBuffer) {
                currentArrayBuffer += 1024;
                currentArray = (char*)realloc(currentArray, currentArrayBuffer*sizeof(char));
            }

            parentArray[parentArrayCounter++] = currentArray;

            parentArray[parentArrayCounter] = NULL;
            break;

        } else {

            if (currentArrayCounter + 1 > currentArrayBuffer) {
                currentArrayBuffer += 1024;
                currentArray = (char*)realloc(currentArray, currentArrayBuffer*sizeof(char));
            }

            currentArray[currentArrayCounter++] = character;

        }

    }

    return parentArray;

}


void execute(char** args) {

    int numArgs = parentLength(args);
    
    if (strcmp(args[0], "cd") == 0) {

        if (args[1] == NULL) {
            fprintf(stderr, "aSh: argument expected to command cd\n");
        } else if (chdir(args[1]) == -1) {
            printf("Error: %s is not a valid directory. Please enter a valid directory name for cd!!\n", args[1]);
        }
        if (chdir(args[1]) == 0) {
            printf("Changed directory to %s\n", args[1]);
        }//printf("%i\n", parentLength(args));
        //my cd already handles .. and other usable commands!!

        //cd is working properly except the hifen option!!

    } else if (strcmp(args[0], "pwd") == 0) {

        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("Current working directory: %s\n", cwd); //working properly except the hifens

    } else if (strcmp(args[0], "quit") == 0) {

        printf("Quitting....\n");
        exit(0); //works


    } else if (strcmp(args[0], "echo") == 0) {

        for (int i = 1; i < numArgs; i++) {
            
            for (int j = 0; j < strlen(args[i]); j++) {

                if (args[i][j] != '"') {
                    char ch = (char) args[i][j];
                    if (ch == '\\') { printf("\n");j++; }
                    else { printf("%c", ch); }
                }
            }
            printf(" ");
            //printf("%s ", args[i]);

        }
        printf("\n");
        //working properly except the hifens
    } else if (args[0] == NULL) {
        return; // empty command entered
    } else {
        externalExe(args);
    }

}

int parentLength(char** args) {
    char* i = args[0];
    int len = 0;

    while (i != NULL) {
        len++;
        i = args[len];
    }

    return len;
}


int externalExe(char** args) {
    
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0) {
        //child process
        if (strcmp(args[0], "ls") == 0){
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            int statusCode = execvp(cwd, args);
            if (statusCode == -1) {printf("Your command failed\n"); perror("aSh");}
        } else if (strcmp(args[0], "cat") == 0) {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            int statusCode = execvp(cwd, args);
            if (statusCode == -1) {printf("Your command failed\n"); perror("aSh");}
        } else if (strcmp(args[0], "mkdir") == 0) {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            int statusCode = execvp(cwd, args);
            if (statusCode == -1) {printf("Your command failed\n"); perror("aSh");}
        } else {
            int statusCode = execvp(args[0], args);
            if (statusCode == -1) {printf("Your command failed\n"); perror("aSh");}
        }  
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        //error forking
        perror("aSh");
    } else {
        //parent
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}