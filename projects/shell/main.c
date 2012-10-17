/*
 * project 1 (shell) main.c template 
 *
 * project partner names and other information up here, please
 *
 */

/* you probably won't need any other header files for this project */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>

char ***commandBreak(char *buffer);

int main(int argc, char **argv) 
{
    char *prompt = "hitme> ";
    printf("%s", prompt);
    fflush(stdout);
    // If mode = 0 than in sequential, mode = 1 is parallel
    int mode = 0;
    int i = 0;

    char buffer[1024];
    while (fgets(buffer, 1024, stdin) != NULL) 
    {
        /* process current command line in buffer */
        buffer[strlen(buffer)-1] = ';';
        char ***cmd = commandBreak(buffer);
        // char *cmd[] = { buffer, "-a", NULL };
        pid_t p = fork();
        if (p == 0) 
        {
            /* in child */
            if(mode == 0)
            {
                for(;cmd[i] != NULL; i++)
                {
                    if(cmd[i + 1] == NULL)
                    {
                        exec_command(cmd[i], mode);
                    }
                    pid_t n = fork();
                    if(n == 0)
                    {
                        exec_command(cmd[i], mode);
                    }
                    else if(n > 0)
                    {
                        int rstatus = 0;
                        pid_t childn = wait(&rstatus);
                        assert(n == childn);
                    }
                } 
            }
            else
            {
                for(;cmd[i] != NULL; i++)
                {
                    if(cmd[i + 1] == NULL)
                    {
                        exec_command(cmd[i], mode);
                    }
                    pid_t n = fork();
                    if(n == 0)
                    {
                        exec_command(cmd[i], mode);
                    }
                } 
            }
        } 
        else if (p > 0) 
        {
            /* in parent */
            int rstatus = 0;
            pid_t childp = wait(&rstatus);
            /* for this simple starter code, the only child process we should
               "wait" for is the one we just spun off, so check that we got the
               same process id */ 
            assert(p == childp);
            printf("%i\n", rstatus);
            if(rstatus < 0){exit(0);}
            else if(rstatus > 0)
            {
                if(mode == 0){mode = 1;}
                else{mode = 0;}
            }
            // printf("Parent got carcass of child process %d, return val %d\n", childp, WEXITSTATUS(rstatus));
        } 
        else 
        {
            /* fork had an error; bail out */
            fprintf(stderr, "fork failed: %s\n", strerror(errno));
        }

        printf("%s", prompt);
        fflush(stdout);
    }

    return 0;
}

void exec_command(char **com, int *mode)
{
    if(strcmp(com[0], "exit") == 0)
    {
        exit(0);
    }
    else if(strcmp(com[0], "mode") == 0)
    {
        if(com[1] == NULL)
        {
            if(mode == 0)
            {printf("%s\n", "sequential");}
            else
            {printf("%s\n", "parallel");}
        }
        else if((com[1] == 's' || (strcmp(com[1], 'sequential') == 0)) && mode == 1)
        {exit(1);}
        else if((com[1] == 'p' || (strcmp(com[1], 'parallel') == 0)) && mode == 0)
        {exit(2);}
        else
        {
            if(mode == 0)
            {printf("%s\n", "sequential");}
            else
            {printf("%s\n", "parallel");}
        }
        exit(0);
    }
    else if (execv(com[0], com) < 0) 
    {
        fprintf(stderr, "execv failed: %s\n", strerror(errno));
        // only way we get here is if execv fails.
        exit(-1);
    }
}

char ***commandBreak(char *buffer)
{
    char * helper;
    int h = 0;
    int com = 0;
    int tot_com = 0;
    int i = 0;
    char ***commands;
    char **result;
    for(;i < strlen(buffer);i++)
    {
        if(buffer[i] == ';')
        {tot_com++;}
    }
    i = 0;
    commands = malloc(sizeof(char**) * (tot_com + 1));
    tot_com = 0;
    int previous = 1;
    for(;i < strlen(buffer);i++)
    {
        if(buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t')
        {
            previous = 1;
        }
        else if(buffer[i] == ';')
        {
            commands[tot_com] = malloc(sizeof(char*) * (com + 1));
            tot_com++;
            com = 0;
        }
        else
        {
            if(previous == 1)
            {
                com++;
                previous = 0;
            }
        }
    }
    i = 0;
    int word = 0;
    com = 0;
    int beg = 0;
    int comment = -1;
    result = commands[0];
    for(;i < strlen(buffer);i++)
    {
        if(buffer[i] == '#')
        {
            comment = i;
            buffer[i] = ';';
        }
        if(buffer[i] == ';')
        {
            if(beg == i)
            {
                beg++;
            }
            else
            {
                helper = malloc(sizeof(char) * (i - beg + 2));
                for(;beg < i; beg++)
                {
                    helper[h] = buffer[beg];
                    h++;
                }
                beg++;
                helper[h] = '\0';
                h = 0;
                result[word] = helper;
                word++;
            }
            result[word] = NULL;
            com++;
            result = commands[com];
            word = 0;
        }
        else if(buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t')
        {
            if(beg == i)
            {
                beg++;
            }
            else
            {
                helper = malloc(sizeof(char) * (i - beg + 2));
                for(;beg < i; beg++)
                {
                    helper[h] = buffer[beg];
                    h++;
                }
                beg++;
                helper[h] = '\0';
                h = 0;
                result[word] = helper;
                word++;
            }
        }
        if(comment == i)
        {
            commands[com] = NULL;
            return commands;
        }
    }
    commands[com] = NULL;
    return commands;
}

