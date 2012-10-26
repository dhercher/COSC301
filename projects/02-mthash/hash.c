#include "hash.h"


// create a new hashtable; parameter is a size hint
hashtable_t *hashtable_new(int sizehint) 
{
	if(sizehint == NULL || sizehint < 1)
		return NULL;
	hashtable_t *tab = (hashtable_t*)malloc(sizeof(hashtable_t));
	if(sizehint <= 1000)
		tab->hash = 1009;
	else if(sizehint == 9929)
		tab->hash = 8861;
	else
		tab->hash = 9929;
	tab->len = sizehint;
	tab->arr = malloc(sizeof(list_t*) * (sizehint + 1));
	tab->arr[sizehint] = NULL;
	int i = 0;
	for (; i < sizehint; i++)
	{
		list_t *list = (list_t*)malloc(sizeof(list_t));
		pthread_mutex_init(&list->mutex, NULL);
		list->head = NULL;
		tab->arr[i] = list;
	}
    return tab;
}

// free anything allocated by the hashtable library
void hashtable_free(hashtable_t *hashtable) 
{
	int i = 0;
	list_t *lst = hashtable->arr[i];
	while(lst != NULL)
	{
		list_clear(lst);
		i++;
		lst = hashtable->arr[i];
	}
}

// add a new string to the hashtable
void hashtable_add(hashtable_t *hashtable, const char *s) 
{
	int h = hashtable->hash;
	int i = 0;
	int sum = 0;
	for(;i < strlen(s);i++)
	{
		sum = sum + (int)s[i];
	}
	sum = sum * hashtable->hash;
	sum = sum % hashtable->len;
	list_add(hashtable->arr[sum], s);
}

// remove a string from the hashtable; if the string
// doesn't exist in the hashtable, do nothing
void hashtable_remove(hashtable_t *hashtable, const char *s) 
{
	int h = hashtable->hash;
	int i = 0;
	int sum = 0;
	for(;i < strlen(s);i++)
	{
		sum = sum + (int)s[i];
	}
	sum = sum * hashtable->hash;
	sum = sum % hashtable->len;
	list_remove(hashtable->arr[sum], s);
}

// print the contents of the hashtable
void hashtable_print(hashtable_t *hashtable) 
{
	printf("*** List Contents Begin ***\n");
	int i = 0;
	list_t *lst = hashtable->arr[i];
	while(lst != NULL)
	{
		list_print(lst);
		i++;
		lst = hashtable->arr[i];
	}
	printf("\n*** List Contents End ***\n");
}

void list_clear(list_t *list) 
{
    pthread_mutex_lock(&list->mutex);
    struct __list_node *tmp = list->head;
    while (tmp) 
    {
        struct __list_node *tmp2 = tmp->next;
        free(tmp);
        tmp = tmp2;
    }
    list->head = NULL;
    pthread_mutex_unlock(&list->mutex);
}

void list_add(list_t *list, const char * val) 
{
    pthread_mutex_lock(&list->mutex);
    struct __list_node *new_node = (struct __list_node *)malloc (sizeof(struct __list_node));
    if (!new_node) {
        fprintf(stderr, "No memory while attempting to create a new list node!\n");
        pthread_mutex_unlock(&list->mutex);
        abort();
    }
    new_node->data = malloc(sizeof(char) * (strlen(val) + 1));
	strcpy(new_node->data, val);
    new_node->next = NULL;

    struct __list_node *tmp = list->head;

    /* special case: list is currently empty */
    if(tmp == NULL) 
    {
    	list->head = new_node;
    }
    else 
    {
        while (tmp->next) 
        {
        	tmp = tmp->next; 
        }
        tmp->next = new_node;
    }
    pthread_mutex_unlock(&list->mutex);
}

int list_remove(list_t *list, const char * target) 
{
    pthread_mutex_lock(&list->mutex);
    int removed = 0;
    /* short cut: is the list empty? */
    if (list->head == NULL)
    {
        pthread_mutex_unlock(&list->mutex);
        return removed;
    }

    /* check for removing items at the head */
    struct __list_node *dead = NULL;
    struct __list_node *tmp = list->head;
    while (tmp != NULL && tmp->data == target) {
        dead = tmp;
        tmp = tmp->next;
        free(dead);
        removed += 1;
    }
    list->head = tmp;

    /* if we removed anything or the data at the head is greater than
       the target, we're done (since the list is sorted */
    if (removed > 0 || target < tmp->data) {
        pthread_mutex_unlock(&list->mutex);
        return removed;
    }

    /* find the target to destroy (if it exists). 
       keep track of previous node using dead.  */
    dead = tmp;
    while (dead != NULL && dead->data != target) {
        tmp = dead;
        dead = dead->next;
    }

    /* if there's something to destroy... */
    if (dead != NULL) { 

        // dead is node we want to remove, tmp is
        // node previous to dead
        while (dead != NULL && dead->data == target) {
            tmp->next = dead->next;
            free(dead);
            dead = tmp->next;    
            removed += 1;
        }
    }
    pthread_mutex_unlock(&list->mutex);
    return removed;
}

// void list_print(list_t *list, FILE *f) 
void list_print(list_t *list) 
{
	pthread_mutex_lock(&list->mutex);
    struct __list_node *tmp = list->head;
    while (tmp) {
    	printf("%s, ", tmp->data);
        // fprintf(f, "%s\n", tmp->data);
        tmp = tmp->next;
    }
    pthread_mutex_unlock(&list->mutex);
}
