#ifndef __HASH_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

struct __list_node 
{
    char * data;
    struct __list_node *next;
};

typedef struct 
{
    struct __list_node *head;
    pthread_mutex_t mutex;
} list_t;

typedef struct 
{
	struct list_t **arr;
	int hash;
	int len;

} hashtable_t;

// create a new hashtable; parameter is a size hint
hashtable_t *hashtable_new(int);

// free anything allocated by the hashtable library
void hashtable_free(hashtable_t *);

// add a new string to the hashtable
void hashtable_add(hashtable_t *, const char *);

// remove a string from the hashtable; if the string
// doesn't exist in the hashtable, do nothing
void hashtable_remove(hashtable_t *, const char *);

// print the contents of the hashtable
void hashtable_print(hashtable_t *);

void list_clear(list_t *);
void list_add(list_t *, const char *);
int list_remove(list_t *, const char *);
// void list_print(list_t *, FILE *);
void list_print(list_t *);

#endif

