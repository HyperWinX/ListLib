#include <stdint.h>
#include <stdlib.h>
#include "list.h"

struct list{
	void* ptr;
	size_t elementsize;
	size_t listsz;
	size_t elementcount;
	size_t allocated;
};

int list_init(struct list* list, size_t initial_size, size_t elementsize){
	if (!initial_size) return NULLINITSIZE;
	void* ptr = calloc(elementsize, 1);
	if (!ptr) return ALLOCFAILURE;
	list->ptr = ptr;
	list->elementsize = elementsize;
	list->listsz = 0;
	list->elementcount = 0;
	list->allocated = elementsize;
	return NOERR;
}

//Basic actions

int list_get(struct list* list, void* element, int index){
        if (index < 0 || index > list->elementcount - 1)
                return INDEXOUTOFRANGE;
        memcpy(element, list->ptr + (index * list->elementsize), list->elementsize);
}                                                 
int list_set(struct list* list, void* element, int index){
        if (index < 0 || index > list->elementcoun
t - 1)                                                            return INDEXOUTOFRANGE;
        memcpy(list->ptr + (index * list-
>elementsize), element, list->elementsize);       }
