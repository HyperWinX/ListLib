#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "list.h"

struct list{
	void* ptr;
	size_t elementsize;
	size_t listsz;
	uint32_t elementcount;
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
        if (index < 0 || index > list->elementcount - 1)                                                            return INDEXOUTOFRANGE;
        memcpy(list->ptr + (index * list->elementsize), element, list->elementsize);
}

//Extended functionality

int list_add(struct list* list, void* element){
	if (list->elementcount > 0){
		void* ptr = realloc(list->ptr, list->allocated + list->elementsize);
		if (!ptr) return REALLOCFAILURE;
		list->ptr = ptr;
		list->allocated += list->elementsize;
	}
	memcpy(list->ptr, element, list->elementsize);
	list->elementcount++;
	list->listsz += list->elementsize;
	return NOERR;
}

int list_addrange1(struct list* list, void* array, int count){
	int retcode = 0;
	for (uint32_t i = 0; i < count; i++){
		retcode = list_add(list, array + (list->elementsize * i));
		if (retcode) return retcode;
	}
	return retcode;
}

int list_addrange2(struct list* list, struct list* src){
	int retcode = 0;
	for (uint32_t i = 0; i < src->elementcount; i++){
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

signed int list_contains(struct list* list, void* element){
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!memcmp(list->ptr + (list->elementsize * i), element, list->elementsize)) return 0;
	return ITEMNOTFOUND;
}

signed int list_exists(struct list* list, predicate comparer, void* elementtofind){
	for (uint32_t i = 0; i < list->elementcount; i++){
		int retcode = comparer(list->ptr + (list->elementsize * i), elementtofind);
		if (!retcode) return retcode;
	}
	return ITEMNOTFOUND;
}

signed int list_find(struct list* list, void* elementtofind, predicate comparer, void* item){
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			memcpy(item, list->ptr + (list->elementsize * i), list->elementsize);
			return NOERR;
		}
	return ITEMNOTFOUND;
}

uint32_t list_findindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind){
	uint32_t end = (endindex > list->elementcount - 1 ? list->elementcount : endindex);
	if (startindex > list->elementcount - 1 ||
	    startindex < 0 ||
	    startindex > endindex){
			errno = ARGBADRANGE;
			return -1;
		}
	for(uint32_t i = startindex; i < end; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_findindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind){
	if (startindex > list->elementcount - 1 ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return -1;
		}
	for (uint32_t i = startindex; i < list->elementcount; i++)
		if(!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_findindex3(struct list* list, void* elementtofind, predicate comparer){
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
}

int list_findlast(struct list* list, void* elementtofind, void* item, predicate comparer){
	for (int i = list->elementcount - 1; i >= 0; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			memcpy(item, list->ptr + (list->elementsize * i), list->elementsize);
			return NOERR;
		}
	return ITEMNOTFOUND;
}

uint32_t list_findlastindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind, void* item){
	uint32_t end = (endindex > list->elementcount - 1 ? list->elementcount : endindex);
	if (startindex > list->elementcount - 1 ||
	    startindex < 0 ||
	    startindex > endindex){
			errno = ARGBADRANGE;
			return -1;
		}
	for(uint32_t i = end; i > startindex; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return -1;
}

uint32_t list_findlastindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind, void* item){
	if (startindex > list->elementcount - 1 ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return -1;
		}
	for (uint32_t i = list->elementcount - 1; i > startindex; i--)
		if(!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_findlastindex3(struct list* list, void* elementtofind, predicate comparer){
	for (uint32_t i = list->elementsize - 1; i >= 0; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
} 

void list_foreach(struct list* list, void (*action)(void*)){
	for (int i = 0; i < list->elementcount; i++)
		(*action)(list->ptr + (list->elementsize * i));
}

uint32_t list_indexof1(struct list* list, void* elementtofind){
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_indexof2(struct list* list, void* elementtofind, uint32_t startindex){
	if (startindex > list->elementcount - 1 ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return -1;
		}
	for (uint32_t i = startindex; i < list->elementcount - 1; i++)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_indexof3(struct list* list, void* elementtofind, uint32_t startindex, uint32_t endindex){
	if (startindex > list->elementcount - 1 ||
		startindex < 0 ||
		endindex > list->elementcount - 1 ||
		endindex < 0 ||
		startindex >= endindex){
			errno = ARGBADRANGE;
			return -1;
	}
	for (uint32_t i = startindex; i < endindex; i++){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	}
	errno = ITEMNOTFOUND;
	return 0;
}

int list_insert(struct list* list, void* element, int index){
	if (index < 0 ||
		index >= list->elementcount - 1){
			errno = ARGBADRANGE;
			return -1;
	}
	void* ptr = realloc(list->ptr, list->allocated + list->elementsize);
	if (!ptr) return REALLOCFAILURE;
	list->ptr = ptr;
	list->allocated += list->elementsize;
	for (int i = list->elementcount; i <= index; i--){
		memcpy(list->ptr + (list->elementsize * (i + 1)), list->ptr + (list->elementsize * i), list->elementsize);
	}
	memcpy(list->ptr + (list->elementsize * index), element, list->elementsize);
	return NOERR;
}

uint32_t list_lastindexof1(struct list* list, void* elementtofind){
	for (uint32_t i = list->elementcount - 1; i >= 0; i--)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_lastindexof2(struct list* list, void* elementtofind, int startindex){
	if (startindex > list->elementcount - 1 ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return -1;
		}
	for (uint32_t i = list->elementcount - 1; i >= startindex; i--)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_lastindexof3(struct list* list, void* elementtofind, int startindex, int endindex){
	if (startindex > list->elementcount - 1 ||
		startindex < 0 ||
		endindex > list->elementcount - 1 ||
		endindex < 0 ||
		startindex >= endindex){
			errno = ARGBADRANGE;
			return -1;
	}
	for (uint32_t i = endindex; i >= startindex; i--){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	}
	errno = ITEMNOTFOUND;
	return 0;
}

int list_remove(struct list* list, void* elementtofind){
	for (uint32_t i = 0; i < list->elementcount; i++){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			memset(list->ptr + (list->elementsize * i), 0x00, list->elementsize);
			for (uint32_t j = i + 1; j < list->elementsize - 1; j++)
				memcpy(list->ptr + (list->elementsize * j - 1), list->ptr + (list->elementsize * j), list->elementsize);
			return NOERR;
		}
	}
	return ITEMNOTFOUND;
}

int list_removeall(struct list* list, void* elementtofind, predicate comparer){
	uint32_t removed = 0;
	for (uint32_t i = 0; i < list->elementcount - 1; i++){
		if (!comparer(elementtofind, list->ptr + (list->elementsize * i))){
			list_remove(list, list->ptr + (list->elementsize * i));
			removed++;
		}
	}
	return removed;
}

int list_removeat(struct list* list, uint32_t index){
	if (index > list->elementcount - 1 ||
		index < 0) return INDEXOUTOFRANGE;
	return list_remove(list, list->ptr + (list->elementsize * i));
}