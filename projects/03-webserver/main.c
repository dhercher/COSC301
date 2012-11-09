#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "network.h"


// global variable; can't be avoided because
// of asynchronous signal interaction
int still_running = TRUE;
void signal_handler(int sig) 
{
    still_running = FALSE;
}


void usage(const char *progname) 
{
    fprintf(stderr, "usage: %s [-p port] [-t numthreads]\n", progname);
    fprintf(stderr, "\tport number defaults to 3000 if not specified.\n");
    fprintf(stderr, "\tnumber of threads is 1 by default.\n");
    exit(0);
}

void quit(void *varg)
{
    char buffer[1024];
    while(fgets(buffer, 1024, stdin) != NULL)
    {
        buffer[strlen(buffer)-1] = '\0';
        if(buffer[0] == 'q')
        {
            printf("%s\n", "stop running");
            still_running = FALSE;
            return;
        }
    }
    return;
}

void runserver(int numthreads, unsigned short serverport) 
{
    //////////////////////////////////////////////////

    // create your pool of threads here

    //////////////////////////////////////////////////
    struct thread_d threads[numthreads + 1];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    int i;
    for(i = 0; i < numthreads; i++)
    {
        threads[i].buf = malloc(sizeof(char) * 1024);
        threads[i].buf_size = 1024;
        threads[i].mutex = &mutex;
        threads[i].thread = NULL;
    }
    i = 0;
    int main_socket = prepare_server_socket(serverport);
    if (main_socket < 0) {
        exit(-1);
    }
    signal(SIGINT, signal_handler);

    struct sockaddr_in client_address;
    socklen_t addr_len;

    fprintf(stderr, "Server listening on port %d.  Going into request loop.\n", serverport);
    pthread_create(&threads[numthreads].thread, NULL, quit, 0);
    while (still_running) 
    {

        struct pollfd pfd = {main_socket, POLLIN};
        int prv = poll(&pfd, 1, 10000);

        if (prv == 0) 
        {
            continue;
        } 
        else if (prv < 0) 
        {
            PRINT_ERROR("poll");
            still_running = FALSE;
            continue;
        }
        
        addr_len = sizeof(client_address);
        memset(&client_address, 0, addr_len);

        int new_sock = accept(main_socket, (struct sockaddr *)&client_address, &addr_len);
        if (new_sock > 0) 
        {
            
            fprintf(stderr, "Got connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

           ////////////////////////////////////////////////////////
           /* You got a new connection.  Hand the connection off
            * to one of the threads in the pool to process the
            * request.
            *
            * Don't forget to close the socket (in the worker thread)
            * when you're done.
            */
           ////////////////////////////////////////////////////////
            printf("%d\n", new_sock);
            for(i = 0; i < numthreads; i++)
            {
                printf("%d\n", threads[i].thread);
                if(threads[i].thread == 0)
                {
                    printf("%d\n", i);
                    threads[i].sock = new_sock;
                    threads[i].client = client_address;
                    pthread_create(&threads[i].thread, NULL, worker, (void*)&threads[i]);
                    continue;
                }
            }
        }
    }
    for(i = 0; i < numthreads; i++)
    {
        free(threads[i].buf);
    }
    pthread_mutex_destroy(&mutex);
    fprintf(stderr, "Server shutting down.\n");
    close(main_socket);
}

void worker(void *varg)
{
    struct thread_d *args = (struct thread_d *)varg;
    int suc;
    suc = getrequest(args->sock, args->buf, args->buf_size);
    if(suc == -1)
    {
        // FAIL
        close(args->sock);
        args->thread = NULL;
        return;
    }
    else
    {
        // File found, send out
        printf("%s\n", args->buf);
    }
    pthread_mutex_lock(&(args->mutex));
    suc = senddata(args->sock, args->buf, args->buf_size);
    pthread_mutex_unlock(&(args->mutex));
    if(suc == -1)
    {
        // FAIL
        close(args->sock);
        args->thread = NULL;
        return;
    }
    else
    {
        // File was sent
        printf("%s\n", "File Sent");
    }
    int wl = open("weblog.txt", O_APPEND);
    printf("%d\n", wl);
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    char *log_load = malloc(sizeof(char) * 1024);
    sprintf(log_load, "%s:%d %s \"GET %s\" 200 %d\n\0", inet_ntoa((args->client).sin_addr), ntohs((args->client).sin_port), asctime(localtime(&ltime)), args->buf, suc);
    printf("%s\n", log_load);
    write(wl, log_load, 1024);
    close(wl);
    close(args->sock);
    args->thread = NULL;
    return;
}


int main(int argc, char **argv) {
    unsigned short port = 3000;
    int num_threads = 1;

    int c;
    while (-1 != (c = getopt(argc, argv, "hp:"))) 
    {
        switch(c) 
        {
            case 'p':
                port = atoi(optarg);
                if (port < 1024) {
                    usage(argv[0]);
                }
                break;

            case 't':
                num_threads = atoi(optarg);
                if (num_threads < 1) 
                {
                    usage(argv[0]);
                }
                break;
            case 'h':
            default:
                usage(argv[0]);
                break;
        }
    }

    runserver(num_threads, port);
    
    fprintf(stderr, "Server done.\n");
    exit(0);
}
