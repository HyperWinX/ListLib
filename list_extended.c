#include <stdint.h>
#include <stdlib.h>
#include "list_extended.h"

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

//Extended functionality

int list_add(struct list* list, void* element){
	if (list->elementcount > 0){
		ptr = realloc(list->ptr, list->allocated + list->elementsize);
		if (!ptr) return REALLOCFAILURE;
		list->ptr = ptr;
		list->allocated += list->elementsize;
	}
	memcpy(list->ptr, element, list->elementsize);
	list->elementcount++;
	list->listsz += list->elementsize;
	return NOERR;
}

int list_addrange(struct list* list, void* array, int count){
	int retcode = 0;
	for (int i = 0; i < count; i++){
		retcode = list_add(list, array + (list->elementsize * i));
		if (retcode) return retcode;
	}
	return retcode;
}

int list_addrange(struct list* list, struct list* src){
	int retcode = 0;
	for (int i = 0; i < src->elementcount; i++){
		retcode = list_add(list, src->ptr + (list->elementsize * i));
		if (retcode) return retcode;
	}
	return retcode;
}

int list_clear(struct list* list){
	void* ptr = realloc(list->ptr, list->elementsize);
	if (!ptr) return REALLOCFAILURE;
	list->ptr = ptr;
	list->elementcount = 0;
	list->listsz = 0;
	list->allocated = list->elementsize;
}

int list_contains(struct list* list, void* element){
	for (int i = 0; i < list->elementcount; i++)
		if (!memcmp(list->ptr + (list->elementsize * i), element, list->elementsize)) return 0;
	return 1;
}

int list_find(struct list* list, void* elementtofind, int (*comparer)(void*,void*)){
	for (int i = 0; i < list->elementcount; i++)
		if (!(*comparer)(list->ptr + (list->elementsize * i), elementtofind)){
			memcpy(item, list->ptr + (list->elementsize * i), list->elementsize);
			return NOERR;
		}
	return ITEMNOTFOUND;
}

int list_findindex(struct list* list, int startindex, int endindex, int (*comparer)(void*,void*), void* elementtofind){
	int end = (endindex > list->elementcount - 1 ? list->elementcount : endindex);
	if (startindex > list->elementcount - 1 ||
	    startindex < 0 ||
	    startindex > endindex) return ARGBADRANGE;
	for(int i = startindex; i < end; i++)
		if (!(*comparer)(list->ptr + (list->elementsize * i), elementtofind)) return i;
	return ITEMNOTFOUND;
}

int list_findindex(struct list* list, int startindex, int (*comparer)(void*,void*), void* elementtofind){
	if (startindex > list->elementcount - 1 ||
	    startindex < 0) return ARGBADRANGE;
	for (int i = startindex; i < list->elementcount; i++)
		if(!(*comparer)(list->ptr + (list->elementsize * i), elementtofind)) return i;
	return ITEMNOTFOUND;
}
