#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

#define ELEMENTCNTINDEX() (list->elementcount == 0 ? 0 : list->elementcount - 1)

indexpair indexpair_default = {NOERR, 0};

//Basic actions

errcode_t realloc_if_required(list* listobj, int elements){
	if (listobj->listsz + (listobj->elementsize * elements) <= listobj->allocated) return NOERR;
	void* ptr = realloc(listobj->ptr, listobj->allocated + listobj->elementsize * elements);
	if (!ptr) return REALLOCFAILURE;
	listobj->ptr = ptr;
	listobj->allocated += listobj->elementsize * elements;
	return NOERR;
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
	indexpair ret = indexpair_default;
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
	indexpair ret = indexpair_default;
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
	indexpair ret = indexpair_default;
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
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !element){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	if ((int)realloc_if_required(list, 4)){
			ret.code = REALLOCFAILURE;
			return ret;
	}
	memcpy(list->ptr + (list->elementsize * list->elementcount), element, list->elementsize);
	list->elementcount++;
	list->listsz += list->elementsize;
	ret.code = NOERR;
	return ret;
}

indexpair list_addrange1(struct list* list, void* array, int count){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !array){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (!count){ // Bad count
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	if ((int)realloc_if_required(list, count)){
			ret.code = REALLOCFAILURE;
			return ret;
	}
	memcpy(list->ptr + (list->elementsize * list->elementcount), array, list->elementsize * count);
	list->elementcount += count;
	return ret;
}

indexpair list_addrange2(struct list* list, struct list* src){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !src){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	if ((int)realloc_if_required(list, src->elementcount)){
			ret.code = REALLOCFAILURE;
			return ret;
	}
	memcpy(list->ptr + (list->elementsize * list->elementcount), src->ptr, src->listsz);
	list->elementcount += src->elementcount;
	return ret;
}

indexpair list_clear(struct list* list){
	indexpair ret = indexpair_default;
	if (!list){
		ret.code = NULLPTR;
		return ret;
	}
	list->elementcount = 0;
	list->listsz = 0;
	return ret;
}

indexpair list_contains(struct list* list, void* element){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !element){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!memcmp(list->ptr + (list->elementsize * i), element, list->elementsize)) return ret;
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_exists(struct list* list, predicate comparer, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !comparer || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)) return ret;
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_find(struct list* list, void* elementtofind, predicate comparer, void* item){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind || !comparer || !item){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			memcpy(item, list->ptr + (list->elementsize * i), list->elementsize);
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_findindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !comparer || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (startindex > ELEMENTCNTINDEX() ||
	    	 startindex > endindex){ // Incorrect indexes
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	for(uint32_t i = startindex; i < endindex; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			ret.index = i;
			return ret;
		}
	errno = ITEMNOTFOUND;
	return ret;
}

indexpair list_findindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !comparer || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (startindex > ELEMENTCNTINDEX()){ // Incorrect indexes
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	for (uint32_t i = startindex; i < list->elementcount; i++)
		if(!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			ret.index = i;
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_findindex3(struct list* list, predicate comparer, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !comparer || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			ret.index = i;
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_findlast(struct list* list, void* elementtofind, predicate comparer, void* item){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind || !comparer || !item){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (int i = list->elementcount - 1; i >= 0; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			memcpy(item, list->ptr + (list->elementsize * i), list->elementsize);
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_findlastindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !comparer || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	if (startindex > ELEMENTCNTINDEX() ||
	    startindex > endindex){ // Incorrect indexes
			ret.code = ARGBADRANGE;
			return ret;
	}
	// Main code
	uint32_t end = (endindex > list->elementcount - 1 ? list->elementcount : endindex);
	for (uint32_t i = end; i > startindex; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			ret.index = i;
			return ret;
		}
	return ret;
}

indexpair list_findlastindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !comparer || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	if (startindex > ELEMENTCNTINDEX()){ // Incorrect indexes
			ret.code = ARGBADRANGE;
			return ret;
	}
	// Main code
	for (uint32_t i = ELEMENTCNTINDEX(); i > startindex; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			ret.index = i;
			return ret;
		}
	return ret;
}

indexpair list_findlastindex3(struct list* list, predicate comparer,  void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !comparer || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = list->elementcount - 1; i >= 0; i--)
		if (!comparer(list->ptr + (list->elementsize * i), elementtofind)){
			ret.index = i;
			return ret;
		}
	return ret;
} 

indexpair list_foreach(struct list* list, void (*action)(void*)){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !action){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = 0; i < list->elementcount; i++)
		(*action)(list->ptr + (list->elementsize * i));
	return ret;
}

indexpair list_indexof1(struct list* list, void* elementtofind, uint32_t startindex, uint32_t endindex){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (startindex > ELEMENTCNTINDEX() ||
 		endindex > ELEMENTCNTINDEX() ||
 		startindex >= endindex){ // Incorrect indexes
			ret.code = ARGBADRANGE;
			return ret;
	}
	// Main code
	for (uint32_t i = startindex; i < endindex; i++){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			ret.index = i;
			return ret;
		}
	}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_indexof2(struct list* list, void* elementtofind, uint32_t startindex){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (startindex > ELEMENTCNTINDEX()){ // Incorrect indexes
			ret.code = ARGBADRANGE;
			return ret;
	}
	// Main code
	for (uint32_t i = startindex; i < list->elementcount - 1; i++)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			ret.index = i;
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_indexof3(struct list* list, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = 0; i < list->elementcount; i++)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			ret.index = i;
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_insert(struct list* list, void* element, uint32_t index){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !element){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (index >= ELEMENTCNTINDEX()){ // Incorrect indexes
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	if ((int)realloc_if_required(list, 4)){
			ret.code = REALLOCFAILURE;
			return ret;
	}
	for (uint32_t i = list->elementcount - 1; i >= index; i--)
		memcpy(list->ptr + (list->elementsize * (i + 1)), list->ptr + (list->elementsize * i), list->elementsize);
	memcpy(list->ptr + (list->elementsize * index), element, list->elementsize);
	list->listsz += list->elementsize;
	return ret;
}

indexpair list_lastindexof1(struct list* list, void* elementtofind, uint32_t startindex, uint32_t endindex){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	if (startindex > ELEMENTCNTINDEX() ||
		endindex > ELEMENTCNTINDEX() ||
		startindex >= endindex){ // Incorrect indexes
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	for (uint32_t i = endindex; i >= startindex; i--){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			ret.index = i;
			return ret;
		}
	}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_lastindexof2(struct list* list, void* elementtofind, uint32_t startindex){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	else if (startindex > ELEMENTCNTINDEX()){ // Incorrect indexes
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	for (uint32_t i = list->elementcount - 1; i >= startindex; i--)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			ret.index = i;
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_lastindexof3(struct list* list, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = list->elementcount - 1; i >= 0; i--)
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			ret.index = i;
			return ret;
		}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_remove(struct list* list, void* elementtofind){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = 0; i < list->elementcount; i++){
		if (!memcmp(elementtofind, list->ptr + (list->elementsize * i), list->elementsize)){
			for (uint32_t j = i + 1; j < list->elementcount - 1; j++)
				memcpy(list->ptr + (list->elementsize * (j - 1)), list->ptr + (list->elementsize * j), list->elementsize);
			return ret;
		}
	}
	ret.code = ITEMNOTFOUND;
	return ret;
}

indexpair list_removeall(struct list* list, void* elementtofind, predicate comparer){
	indexpair ret = indexpair_default;
	// Protection
	if (!list || !elementtofind || !comparer){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	// Main code
	for (uint32_t i = 0; i < list->elementcount - 1; i++)
		if (!comparer(elementtofind, list->ptr + (list->elementsize * i)))
			list_removeat(list, i);
	return ret;
}

indexpair list_removeat(struct list* list, uint32_t index){
	indexpair ret = indexpair_default;
	// Protection
	if (!list){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	if (index > ELEMENTCNTINDEX()){ // Incorrect indexes
		ret.code = ARGBADRANGE;
		return ret;
	}
	// Main code
	for (uint32_t i = index + 1; i < list->elementcount; i++)
		memcpy(list->ptr + (list->elementsize * (i - 1)), list->ptr + (list->elementsize * i), list->elementsize);
	return ret;
}

indexpair list_reverse(struct list* list){
	indexpair ret = indexpair_default;
	// Protection
	if (!list){ // Null pointer
		ret.code = NULLPTR;
		return ret;
	}
	if (list->elementcount < 2) return ret;
	uint32_t left = 0, right = 0;
	uint32_t length = list->elementcount;
	char tmpbuf[list->elementsize];
	for (left = 0, right = length - 1; left < right; left++, right--){
		memcpy(tmpbuf, list->ptr + (list->elementsize * left), list->elementsize);
		memcpy(list->ptr + (list->elementsize * left), list->ptr + (list->elementsize * right), list->elementsize);
		memcpy(list->ptr + (list->elementsize * right), tmpbuf, list->elementsize);
	}
	return ret;
}
