#define ARGBADRANGE -2
#define ITEMNOTFOUND -1
#define NOERR 0
#define NULLINITSIZE 1
#define ALLOCFAILURE 2
#define REALLOCFAILURE 3
#define INDEXOUTOFRANGE 4
#define NULLPTR 5

struct list{
	void* ptr;
	size_t elementsize;
	size_t listsz;
	uint32_t elementcount;
	size_t allocated;
};

typedef int (*predicate)(void*,void*);

int list_init(struct list* list, size_t initial_count, size_t elementsize);
int list_get(struct list* list, void* element, int index);
int list_set(struct list* list, void* element, int index);
int list_destroy(struct list* list);
int list_add(struct list* list, void* element);
int list_addrange1(struct list* list, void* array, int count);
int list_addrange2(struct list* list, struct list* src);
int list_clear(struct list* list);
signed int list_contains(struct list* list, void* element);
signed int list_exists(struct list* list, predicate comparer, void* elementtofind);
signed int list_find(struct list* list, void* elementtofind, predicate comparer, void* item);
uint32_t list_findindex1(struct list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind);
uint32_t list_findindex2(struct list* list, uint32_t startindex, predicate comparer, void* elementtofind);
uint32_t list_findindex3(struct list* list, predicate comparer, void* elementtofind);
signed int list_findlast(struct list* list, void* elementtofind, void* item, predicate comparer);
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