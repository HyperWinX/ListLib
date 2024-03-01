#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "list.h"

#define ELEMENTCNTINDEX() (list->elementcount == 0 ? 0 : list->elementcount - 1)

//Basic actions

void realloc_if_required(list* listobj, int elements){
	if (listobj->listsz != listobj->allocated) return;
	void* ptr = realloc(listobj->ptr, listobj->allocated + listobj->elementsize * elements);
	if (!ptr) return;
	listobj->ptr = ptr;
	listobj->allocated += listobj->elementsize * elements;
}

struct list list_init(size_t initial_count, size_t elementsize){
	struct list listobj = {
		0,
		elementsize,
		0, 0,
		elementsize * initial_count
	};
	// Protection
	if (!initial_count) return listobj;
	if (!elementsize) return listobj;
	// Main body
	listobj.ptr = calloc(elementsize * initial_count, 1);
	return listobj;
}

indexpair list_get(struct list* list, void* element, uint32_t index){
	indexpair ret;
	// Protection
	if (!list || !element){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (index > ELEMENTCNTINDEX()){ // Index bigger than list size
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	memcpy(element, list->ptr + (index * list->elementsize), list->elementsize);
	ret.code = NOERR;
	return ret;
}                                                 
indexpair list_set(struct list* list, void* element, uint32_t index){
	indexpair ret;
	// Protection
	if (!list || !element){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (index > ELEMENTCNTINDEX()){ // Index bigger than list size
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	memcpy(list->ptr + (index * list->elementsize), element, list->elementsize);
	ret.code = NOERR;
	return ret;
}

indexpair list_destroy(struct list* list){
	indexpair ret;
	// Protection
	if (!list){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	if (list->ptr) free(list->ptr);
	memset(list, 0x00, sizeof(struct list));
	ret.code = NOERR;
	return ret;
}

//Extended functionality

indexpair list_add(struct list* list, void* element){
	indexpair ret;
	// Protection
	if (!list || !element){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	realloc_if_required(list, 4);
	memcpy(list->ptr + (list->elementsize * list->elementcount), element, list->elementsize);
	list->elementcount++;
	list->listsz += list->elementsize;
	ret.code = NOERR;
	return ret;
}

indexpair list_addrange1(struct list* list, void* array, int count){
	indexpair ret;
	// Protection
	if (!list || !array){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (!count){ // Bad count
		ret.code = ARGBADRANGE;
		return ret;
	}
	int retcode = 0;
	if (list->allocated < (list->elementcount + count) * list->elementsize){
		void* newptr = realloc(list->ptr, (list->elementcount + count) * list->elementsize);
		if (!newptr) return REALLOCFAILURE;
		list->ptr = newptr;
		list->allocated = (list->elementcount + count) * list->elementsize;
	}
	memcpy(list->ptr + (list->elementsize * list->elementcount), array, list->elementsize * count);
	list->elementcount += count;
	return retcode;
}

int list_addrange2(struct list* list, struct list* src){
	if (!list) return NULLPTR;
	if (!src) return NULLPTR;
	int retcode = 0;
	if (list->allocated - list->listsz < src->listsz){
		void* newptr = realloc(list->ptr, list->listsz + src->listsz);
		if (!newptr) return REALLOCFAILURE;
		list->ptr = newptr;
		list->allocated = list->listsz + src->listsz;
	}
	memcpy(list->ptr + (list->elementsize * list->elementcount), src->ptr, src->listsz);
	list->elementcount += src->elementcount;
	return retcode;
}

int list_clear(struct list* list){
	if (!list) return NULLPTR;
	list->elementcount = 0;
	list->listsz = 0;
	return NOERR;
}

signed int list_contains(struct list* list, void* element){
	if (!list) return NULLPTR;
	if (!element) return NULLPTR;
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!memcmp(list->ptr + (list->elementsize * i), element, list->elementsize)) return 0;
	return ITEMNOTFOUND;
}

signed int list_exists(struct list* list, predicate comparer, void* elementtofind){
	if (!list) return NULLPTR;
	if (!comparer) return NULLPTR;
	if (!elementtofind) return NULLPTR;
	for (uint32_t i = 0; i < list->elementcount; i++){
		int retcode = comparer(list->ptr + (list->elementsize * i), elementtofind);
		if (!retcode) return retcode;
	}
	return ITEMNOTFOUND;
}

signed int list_find(struct list* list, void* elementtofind, predicate comparer, void* item){
	if (!list) return NULLPTR;
	if (!elementtofind) return NULLPTR;
	if (!comparer) return NULLPTR;
	if (!item) return NULLPTR;
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			memcpy(item, list->ptr + (list->elementsize * i), list->elementsize);
			return NOERR;
		}
	return ITEMNOTFOUND;
}

uint32_t list_findindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind){
	if (!list) goto nullptr;
	if (!comparer) goto nullptr;
	if (!elementtofind) goto nullptr;
	if (startindex > ELEMENTCNTINDEX() ||
	    startindex < 0 ||
	    startindex > endindex){
			errno = ARGBADRANGE;
			return 0;
	}
	for(uint32_t i = startindex; i < endindex; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
nullptr:
	errno = NULLPTR;
	return 0;
}

uint32_t list_findindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind){
	if (!list) goto nullptr;
	if (!comparer) goto nullptr;
	if (!elementtofind) goto nullptr;
	if (startindex > ELEMENTCNTINDEX() ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return 0;
		}
	for (uint32_t i = startindex; i < list->elementcount; i++)
		if(!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
nullptr:
	errno = NULLPTR;
	return 0;
}

uint32_t list_findindex3(struct list* list, predicate comparer, void* elementtofind){
	if (!list) goto nullptr;
	if (!comparer) goto nullptr;
	if (!elementtofind) goto nullptr;
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
nullptr:
	errno = NULLPTR;
	return 0;
}

signed int list_findlast(struct list* list, void* elementtofind, predicate comparer, void* item){
	if (!list) return NULLPTR;
	for (int i = list->elementcount - 1; i >= 0; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			memcpy(item, list->ptr + (list->elementsize * i), list->elementsize);
			return NOERR;
		}
	return ITEMNOTFOUND;
}

uint32_t list_findlastindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind){
	// Bad arguments protection
	if (!list || !comparer || !elementtofind) goto nullptr;
	if (startindex > ELEMENTCNTINDEX() ||
	    startindex < 0 ||
	    startindex > endindex) goto argbadrange;
	// Main function body
	uint32_t end = (endindex > list->elementcount - 1 ? list->elementcount : endindex);
	for(uint32_t i = end; i > startindex; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	// Error handlers
	errno = ITEMNOTFOUND;
	return 0;
nullptr:
	errno = NULLPTR;
	return 0;
argbadrange:
	errno = ARGBADRANGE;
	return 0;
}

uint32_t list_findlastindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind){
	if (!list) goto nullptr;
	if (!comparer) goto nullptr;
	if (!elementtofind) goto nullptr;
	if (startindex > ELEMENTCNTINDEX() ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return 0;
	}
	for (uint32_t i = ELEMENTCNTINDEX(); i > startindex; i--)
		if(!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
nullptr:
	errno = NULLPTR;
	return 0;
}

uint32_t list_findlastindex3(struct list* list, predicate comparer,  void* elementtofind){
	if (!list) goto nullptr;
	if (!comparer) goto nullptr;
	if (!elementtofind) goto nullptr;
	for (uint32_t i = list->elementcount - 1; i >= 0; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return i;
	errno = ITEMNOTFOUND;
	return 0;
nullptr:
	errno = NULLPTR;
	return 0;
} 

int list_foreach(struct list* list, void (*action)(void*)){
	if (!list) goto nullptr;
	if (!action) goto nullptr;
	for (int i = 0; i < list->elementcount; i++)
		(*action)(list->ptr + (list->elementsize * i));
	return NOERR;
nullptr:
	errno = NULLPTR;
	return 0;
}

uint32_t list_indexof1(struct list* list, void* elementtofind){
	if (!list) goto nullptr;
	if (!elementtofind) goto nullptr;
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
nullptr:
	errno = NULLPTR;
	return 0;
}

uint32_t list_indexof2(struct list* list, void* elementtofind, uint32_t startindex){
	if (!list || !elementtofind){
		errno = NULLPTR;
		return 0;
	}
	if (startindex > ELEMENTCNTINDEX() ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return 0;
		}
	for (uint32_t i = startindex; i < list->elementcount - 1; i++)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_indexof3(struct list* list, void* elementtofind, uint32_t startindex, uint32_t endindex){
	if (!list || !elementtofind){
		errno = NULLPTR;
		return 0;
	}
	if (startindex > ELEMENTCNTINDEX() ||
		startindex < 0 ||
		endindex > ELEMENTCNTINDEX() ||
		endindex < 0 ||
		startindex >= endindex){
			errno = ARGBADRANGE;
			return 0;
	}
	for (uint32_t i = startindex; i < endindex; i++){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	}
	errno = ITEMNOTFOUND;
	return 0;
}

int list_insert(struct list* list, void* element, uint32_t index){
	if (!list) return NULLPTR;
	if (!element) return NULLPTR;
	if (index < 0 ||
		index >= ELEMENTCNTINDEX()){
			errno = ARGBADRANGE;
			return 0;
	}
	if (list->allocated == list->listsz){
		void* ptr = realloc(list->ptr, list->allocated + list->elementsize * 4);
		if (!ptr) return REALLOCFAILURE;
		list->ptr = ptr;
		list->allocated += list->elementsize * 4;
	}
	for (uint32_t i = list->elementcount - 1; i >= index; i--)
		memcpy(list->ptr + (list->elementsize * (i + 1)), list->ptr + (list->elementsize * i), list->elementsize);
	memcpy(list->ptr + (list->elementsize * index), element, list->elementsize);
	list->listsz += list->elementsize;
	return NOERR;
}

uint32_t list_lastindexof1(struct list* list, void* elementtofind){
	if (!list || !elementtofind){
		errno = NULLPTR;
		return 0;
	}
	for (uint32_t i = list->elementcount - 1; i >= 0; i--)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_lastindexof2(struct list* list, void* elementtofind, uint32_t startindex){
	if (!list || !elementtofind){
		errno = NULLPTR;
		return 0;
	}
	if (startindex > ELEMENTCNTINDEX() ||
	    startindex < 0){
			errno = ARGBADRANGE;
			return 0;
		}
	for (uint32_t i = list->elementcount - 1; i >= startindex; i--)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	errno = ITEMNOTFOUND;
	return 0;
}

uint32_t list_lastindexof3(struct list* list, void* elementtofind, uint32_t startindex, uint32_t endindex){
	if (!list || !elementtofind){
		errno = NULLPTR;
		return 0;
	}
	if (startindex > ELEMENTCNTINDEX() ||
		startindex < 0 ||
		endindex > ELEMENTCNTINDEX() ||
		endindex < 0 ||
		startindex >= endindex){
			errno = ARGBADRANGE;
			return 0;
	}
	for (uint32_t i = endindex; i >= startindex; i--){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize))
			return i;
	}
	errno = ITEMNOTFOUND;
	return 0;
}

signed int list_remove(struct list* list, void* elementtofind){
	if (!list || !elementtofind){
		errno = NULLPTR;
		return 0;
	}
	for (uint32_t i = 0; i < list->elementcount; i++){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			memset(list->ptr + (list->elementsize * i), 0x00, list->elementsize);
			
			for (uint32_t j = i + 1; j < list->elementsize - 1; j++)
				memcpy(list->ptr + (list->elementsize * (j - 1)), list->ptr + (list->elementsize * j), list->elementsize);
			return NOERR;
		}
	}
	return ITEMNOTFOUND;
}

uint32_t list_removeall(struct list* list, void* elementtofind, predicate comparer){
	if (!list || !elementtofind || !comparer){
		errno = NULLPTR;
		return 0;
	}
	uint32_t removed = 0;
	for (uint32_t i = 0; i < list->elementcount - 1; i++){
		if (!comparer(elementtofind, list->ptr + (list->elementsize * i))){
			list_remove(list, list->ptr + (list->elementsize * i));
			removed++;
		}
	}
	return removed;
}

signed int list_removeat(struct list* list, uint32_t index){
	if (!list){
		errno = NULLPTR;
		return 0;
	}
	if (index > ELEMENTCNTINDEX() ||
		index < 0){
			errno = ARGBADRANGE;
			return 0;
		}
	for (uint32_t i = index + 1; i < list->elementcount; i++)
		memcpy(list->ptr + (list->elementsize * (i - 1)), list->ptr + (list->elementsize * i), list->elementsize);
	return NOERR;
}

int list_reverse(struct list* list){
	if (!list){
		errno = NULLPTR;
		return 0;
	}
	if (list->elementcount < 2) return 0;
	uint32_t left = 0, right = 0;
	uint32_t length = list->elementcount;
	char tmpbuf[list->elementsize];
	for (left = 0, right = length - 1; left < right; left++, right--){
		memcpy(tmpbuf, list->ptr + (list->elementsize * left), list->elementsize);
		memcpy(list->ptr + (list->elementsize * left), list->ptr + (list->elementsize * right), list->elementsize);
		memcpy(list->ptr + (list->elementsize * right), tmpbuf, list->elementsize);
	}
	return NOERR;
}
