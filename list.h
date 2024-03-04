#pragma once

#define ARGBADRANGE -2
#define ITEMNOTFOUND -1
#define NOERR 0
#define NULLINITSIZE 1
#define ALLOCFAILURE 2
#define REALLOCFAILURE 3
#define INDEXOUTOFRANGE 4
#define NULLPTR 5

typedef struct list{
	void* ptr;
	size_t elementsize;
	size_t listsz;
	uint32_t elementcount;
	size_t allocated;
} list;

typedef struct indexpair{
	int32_t code;
	uint32_t index;
} indexpair;

typedef int (*predicate)(void*,void*);

list list_init(size_t initial_count, size_t elementsize);
indexpair list_get(struct list* list, void* element, uint32_t index);
indexpair list_set(struct list* list, void* element, uint32_t index);
indexpair list_destroy(struct list* list);
indexpair list_add(struct list* list, void* element);
indexpair list_addrange1(struct list* list, void* array, int count);
indexpair list_addrange2(struct list* list, struct list* src);
indexpair list_clear(struct list* list);
indexpair list_contains(struct list* list, void* element);
indexpair list_exists(struct list* list, predicate comparer, void* elementtofind);
signed int list_find(struct list* list, void* elementtofind, predicate comparer, void* item);
uint32_t list_findindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind);
uint32_t list_findindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind);
uint32_t list_findindex3(struct list* list, predicate comparer, void* elementtofind);
signed int list_findlast(struct list* list, void* elementtofind, predicate comparer, void* item);
uint32_t list_findlastindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind);
uint32_t list_findlastindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind);
uint32_t list_findlastindex3(struct list* list, predicate comparer, void* elementtofind);
int list_foreach(struct list* list, void (*action)(void*));
uint32_t list_indexof1(struct list* list, void* elementtofind);
uint32_t list_indexof2(struct list* list, void* elementtofind, uint32_t startindex);
uint32_t list_indexof3(struct list* list, void* elementtofind, uint32_t startindex, uint32_t endindex);
int list_insert(struct list* list, void* element, uint32_t index);
uint32_t list_lastindexof1(struct list* list, void* elementtofind);
uint32_t list_lastindexof2(struct list* list, void* elementtofind, uint32_t startindex);
uint32_t list_lastindexof3(struct list* list, void* elementtofind, uint32_t startindex, uint32_t endindex);
signed int list_remove(struct list* list, void* elementtofind);
uint32_t list_removeall(struct list* list, void* elementtofind, predicate comparer);
signed int list_removeat(struct list* list, uint32_t index);
int list_reverse(struct list* list);
