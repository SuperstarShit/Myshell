
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>



#define MAX_SIZE 100


typedef struct 
{
    char *stack_com[MAX_SIZE];
    int top;
} stack;

void initializer(stack *ptr)
{
    ptr->top = -1;
    return;
}

bool isempty(stack *ptr)
{
    if(ptr->top == -1) return true;

    return false;
}

bool isfull(stack *ptr)
{
    if(ptr->top >= MAX_SIZE-1) return true;

    return false;
}

void push(stack *ptr , char command[])
{
    if(isfull(ptr)) 
    {
        printf("Stack Overflow\n");
        return;
    }
    char *val = malloc(strlen(command)+1);
    strcpy(val, command);
    ptr->stack_com[++ptr->top] = val;
    printf("Successfully pushed: ");
    printf("%s\n", command);

    return;
}

void pop(stack *ptr)
{
    if(isempty(ptr))
    {
        printf("Stack underflow\n");
        return;
    }
    char *tmp = ptr->stack_com[ptr->top];
    ptr->top--;
    printf("Successfully poppped: ");
    printf("%s", tmp);

    free(tmp);

    return;
}

void peek(stack *ptr)
{
    printf("%s", ptr->stack_com[ptr->top]);

    return;
}




int isChar_space(char value);

struct commandList
{
    int index;
    char operator;
};

int main()
{

    stack history;
    initializer(&history);
    char *str = malloc(100);

    while (1)
    {
        printf("Myshell> ");

        fgets(str, 100, stdin);

        if (strncmp(str, "fk u", 4) == 0) break;
            
        else
        {
            push(&history, str);

// parser is here now
              char *commands[10];

                char *p = str;
                int word_count = 0;
                commands[word_count++] = p;

                while (*p != '\0')
                {
                    if (*p == '\n')
                    {
                        *p = '\0';
                        break;
                    }
                    p++;
                }

                p = str;

                while (word_count < 10 && *p != '\0')
                {
                    if (*p == ' ')
                    {
                        *p = '\0';
                        p++;

                        if (*p != '\0')
                        {
                            commands[word_count++] = p;
                        }
                    }
                    else
                        p++;
                }
                commands[word_count] = NULL;



            pid_t worker = fork();

            if (worker < 0)
            {
                printf("Child creation failed\n");
                return 1;
            }

            if (worker == 0)
            {
               
              

              
/*
                for (int i = 0; commands[i] != NULL; i++)
                {
                    if (commands[i] != NULL && strcmp(commands[i], ">") == 0)
                    {
                      
                       int fd = open(commands[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                       
                      if(fd == -1)
                      {
                        perror("open: ");
                        exit(2);
                      }

                      if(dup2(fd, STDOUT_FILENO) == -1)
                      {
                        perror("dup2: ");
                        exit(3);
                      }



                       commands[i] = NULL;
                       close(fd);

                       commands[i+1] = NULL;
                       
                    //    break;

                    }
                    else if(strcmp(commands[i], "<") == 0)
                    {
                        int fd = open(commands[i+1], O_RDONLY);

                        if(fd == -1)
                        {
                            perror("open: ");
                            exit(4);
                        }

                        if(dup2(fd, STDIN_FILENO) == -1)
                        {
                            perror("dup2: ");
                            exit(5);
                        }

                        commands[i] = NULL;
                        close(fd);
                        // break;

                    }
                    
                }

                */



    
    struct commandList strcommand[5] = {0};

    // char *check = "cat < index > output";
    int arrind = 0;

    for(int i = 0; commands[i] != NULL; i++)
    {
        if(isChar_space(commands[i][0]) || strlen(commands[i]) > 1) continue;
        else
        {
            strcommand[arrind].operator = commands[i][0];
            strcommand[arrind].index = i;
            arrind++;
        }
    }

    // for(int i = 0;i < 2;i++)
    // {
    //     printf("%c at index %i\n", checking[i].command, checking[i].index);

    // }
    

    
    for(int i = 0; strcommand[i].operator != '\0'; i++)
    {
        if(strcommand[i].operator == '<')
        {
            int fd = open(commands[strcommand[i].index+1], O_RDONLY);

            if(fd == -1)
            {
                perror("open: ");
                exit(3);
            }

            if(dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2: ");
                exit(4);
            }

            close(fd);

        }

        else if(strcommand[i].operator == '>')
        {

          int fd = open(commands[strcommand[i].index+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                       
          if(fd == -1)
            {
              perror("open: ");
              exit(2);
            }

            if(dup2(fd, STDOUT_FILENO) == -1)
                {
                perror("dup2: ");
                exit(3);
                }

                close(fd);

        }

        else if(strcommand[i].operator == '|')
        {
            int pipeline[2];

            if(pipe(pipeline) == -1)
            {
                perror("pipe: ");
                exit(8);
            } 
           
            pid_t grandchild = fork();

            if(grandchild < 0) 
            {
                perror("fork: ");
                exit(7);
            }

            if(grandchild == 0) // for left side of pipe
            {
                // read from commands[index-1] and write into pipeline[1]
                // to do this : fix the stdout of this process into pipeline 1 and run exec
                
                commands[strcommand[i].index] = NULL;

                
                if(dup2(pipeline[1], STDOUT_FILENO) == -1) 
                {
                    perror("dup2: ");
                    exit(9);
                }

                close(pipeline[1]);
                
                execvp(commands[0], commands);
            }

            
            // int statusG;
            // waitpid(grandchild, &statusG, 0);
            // if(WIFEXITED(statusG) != 1)
            // {
            //     printf("Grandchild fucked it up\n");
            //     exit(10);
            // }

            // now the child parent will read from the pipeline and write into right side 
            
            close(pipeline[1]);

            dup2(pipeline[0], STDIN_FILENO);
            close(pipeline[0]);
            execvp(commands[strcommand[i].index+1], &commands[strcommand[i].index+1]);
        }
    }
    

    if(strcommand[0].operator != '\0')
    {
        // printf("nulling: %s\n",commands[strcommand[0].index]); for debugging
 commands[strcommand[0].index] = NULL;
    }

    
                // printf("This is the first command %s\n", commands[0]); // fix this, the first command is null in ./name --> fixed

                execvp(commands[0], commands);
                perror("execvp");
                return 6;
            }

            else
            {
                int status;
                waitpid(worker, &status, 0);
                printf("The child exited with status: %i\n", WEXITSTATUS(status));
            }
        }
    }
    return 0;
}






int isChar_space(char value)
{
    int alpha = (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');

    return (value == ' ') || alpha;
}