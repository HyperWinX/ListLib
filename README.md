<div align="center">
    <h1>ListLib</h1>
    <p>New level dynamic array library.</p>
    <p>
        <a href="https://github.com/HyperWinX/CTeatify/graphs/contributors">
            <img src="https://img.shields.io/github/contributors/HyperWinX/ListLib" alt="contributors"/>
        </a>
        <a href="https://github.com/HyperWinX/ListLib/commits/master">
            <img src="https://img.shields.io/github/last-commit/HyperWinX/ListLib" alt="last commit"/>
        </a>
        <a href="https://github.com/HyperWinX/ListLib/network/members">
            <img src="https://img.shields.io/github/forks/HyperWinX/ListLib" alt="forks"/>
        </a>
        <a href="https://github.com/HyperWinX/ListLib/stargazers">
            <img src="https://img.shields.io/github/stars/HyperWinX/ListLib" alt="contributors"/>
        </a>
        <a href="https://github.com/HyperWinX/ListLib/issues">
            <img src="https://img.shields.io/github/issues/HyperWinX/ListLib" alt="contributors"/>
        </a>
    </p>
</div>
<br/>

# Contents
- [About project](#about-listlib)
- [Installation](#installation)
- [Usage](#usage)
    - [Indexpair struct](#indexpair-struct)
    - [Predicate](#predicate)
    - [Initialize array](#initialization)
    - [Get value](#get-value)
    - [Set value](#set-value)
    - [Destroy](#destroy-array)
    - [Add element](#add-element)
    - [Add array](#add-array)
    - [Add another dynamic array](#add-another-dynamic-array)
    - [Clear](#clear-array)
    - [Contains element](#check-if-array-contains-element)
    - [Element exists](#check-element-existance)
    - [Find with predicate](#find-element-with-predicate)
    - [Find index within range using predicate](#find-element-index-within-specified-range-using-predicate)
    - [Find index with start index using predicate](#find-element-index-with-specified-start-index-using-predicate)
    - [Find index using predicate](#find-element-index-using-predicate)
    - [Find last element index within specified range using predicate](#find-last-index-of-element-within-specified-range-using-predicate)
    - [Find last element index with specified start index using predicate](#find-last-index-of-element-with-start-index-and-using-predicate)
    - [Do something to every list element](#perform-action-on-each-element-in-the-array)
    - [Find element index within specified range](#get-index-of-element-within-specified-range)
    - [Find element index with specified starting index](#get-index-of-element-with-start-index)
    - [Find element index](#get-index-of-element)
    - [Insert element](#insert-element-at-specific-index)
    - [Find last element index within specified range](#get-last-index-of-element-within-specified-range)
    - [Find last element index with specified starting index](#get-index-of-element-with-start-index)
    - [Find last element index](#get-index-of-element)
    - [Remove element](#remove-element-from-the-list)
    - [Remove elements using predicate](#remove-all-occurrences-of-element-using-predicate)
    - [Remove element at index](#remove-element-at-specific-index)
    - [Reverse list](#reverse-the-order-of-elements-in-the-list)

## About ListLib
This is a small dynamic array implementation with a lot functionality. Written in pure C. You can use any object as list element, even custom structs!

## Installation
1. Download [header](list.h) and [.c source file](list.c) (if you want to compile it directly into project) or download .so file from releases.
2. Include header into your project.
3. Just use!

## Usage

### Indexpair struct
Library has indexpair struct, defined with typedef. It contains index, if function should return index, and exit code. Using that, you can check if function exited successfully.
To access values, do this:
```c
int restored = 0;
indexpair ret = list_get(&listobj, &restored, 2);
int return_code = ret.code; // You got retcode
int index = ret.index; // Index. Returned only from special functions
```

### Predicate
There is typedef'd predicate type with signature
```c
int (*predicate)(void*,void*)
```
This is a type for special functions, which require predicate. Using it, you can find values even when you are using custom structs as elements!

### Initialization
Signature:
```c
list list_init(int initial_count, int elementsize);
```
Array initialization:
```c
list listobj = list_init(1, sizeof(int));
```
In this example, you allocated array with size 1 and element size equal to sizeof int.

### Get value
Signature:  
```c
indexpair list_get(list* listobj, void* element, uint32_t index);
```
Get value from array:
```c
int restored = 0;
list_get(&listobj, &restored, 2);
```

### Set value
Signature:
```c
indexpair list_set(list* listobj, void* element, uint32_t index);
```
Set array element value:
```c
int value = 5;
list_set(&listobj, &value, 2);
```

### Destroy array
Signature:
```c
indexpair list_destroy(list* listobj);
```
Destroy array:
```c
list_destroy(&listobj);
```

### Add element
Signature:
```c
indexpair list_add(list* listobj, void* element);
```
Append element to array and allocate more memory if needed:
```c
int element = 5;
list_add(&listobj, &element);
```

### Add array
Signature:
```c
indexpair list_addrange1(list* listobj, void* arr, int count);
```
Append whole array and allocate more memory if needed:
```c
int arr[3] = {5, 6, 1};
list_addrange1(&listobj, &arr, sizeof(arr) / sizeof(int));
```

### Add another dynamic array
Signature:
```c
indexpair list_addrange2(list* listobj, list* src);
```
Append another dynamic array:
```c
list_addrange2(&listobj, &another);
```

### Clear array
Signature:
```c
indexpair list_clear(list* listobj);
```
Zero element count but do nothing with allocated memory:
```c
list_clear(&listobj);
```

### Check if array contains element
Signature:
```c
indexpair list_contains(list* listobj, void* element);
```
Check if array contains element:
```c
int element = 5;
list_contains(&listobj, &element);
```

### Check element existance
Signature:
```c
indexpair list_exists(list* listobj, predicate comparer, void* element);
```
Check if element exists using predicate passed from user:
```c
custom_type element = {0};
list_exists(&listobj, comparer, &element);
```

### Find element with predicate
Signature:
```c
indexpair list_find(list* list, void* elementtofind, predicate comparer, void* item);
```
Find element with predicate:
```c
custom_type elementtofind = {0};
custom_type restored;
list_find(&listobj, &elementtofind, comparer, &restored);
```

### Find element index within specified range using predicate
Signature:
```c
indexpair list_findindex1(list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind);
```
Find element index within specified range using predicate:
```c
custom_type elementtofind = {0};
list_findindex1(&listobj, 0, 10, comparer, &elementtofind);
```

### Find element index with specified start index using predicate
Signature:
```c
indexpair list_findindex2(list* list, uint32_t startindex, predicate comparer, void* elementtofind);
```
Find element index with specified start index and custom comparer function:
```c
custom_type elementtofind = {0};
list_findindex2(&listobj, 5, comparer, &elementtofind);
```

### Find element index using predicate
Signature:
```c
indexpair list_find_index3(list* list, predicate comparer, void* elementtofind);
```
Find element index using predicate:
```c
custom_type elementtofind = {0};
list_findindex3(&listobj, comparer, &element_to_find);
```

### Find last occurrence of element using predicate
Signature:
```c
indexpair list_findlast(list* list, void* elementtofind, predicate comparer, void* item);
```
Find last occurrence of element using predicate:
```c
custom_type elementtofind = {0};
custom_type restored;
list_findlast(&listobj, &elementtofind, comparer, &restored);
```

### Find last index of element within specified range using predicate
Signature:
```c
indexpair list_findlastindex1(list* list, uint32_t startindex, uint32_t endindex, predicate comparer, void* elementtofind);
```
Find last occurrence of element within specified range using predicate:
```c
custom_type elementtofind = {0};
listfindlastindex1(&listobj, 0, 10, comparer, &elementtofind);
```

### Find last index of element with start index and using predicate
Signature:
```c
indexpair list_findlastindex2(list* list, uint32_t startindex, predicate comparer, void* elementtofind);
```
Find last occurrence of element with specified start index and predicate:
```c
custom_type elementtofind = {0};
list_findlastindex2(&listobj, 5, comparer, &elementtofind);
```

### Find last index of element with predicate
Signature:
```c
indexpair list_findlastindex3(list* list, predicate comparer, void* elementtofind);
```
Find last occurrence of element using predicate:
```c
custom_type elementtofind = {0};
list_findlastindex3(&listobj, comparer, &elementtofind);
```

### Perform action on each element in the array
Signature:
```c
indexpair list_foreach(list* list, void (*action)(void*));
```
Perform action on each element in the array:
```c
list_foreach(&listobj, custom_action);
```

### Get index of element within specified range
Signature:
```c
indexpair list_indexof1(list* list, void* elementtofind, uint32_t startindex, uint32_t endindex);
```
Get index of element within specified range:
```c
custom_type elementtofind = {0};
list_indexof1(&listobj, &elementtofind, 0, 10);
```

### Get index of element with start index
Signature:
```c
indexpair list_indexof2(list* list, void* elementtofind, uint32_t startindex);
```
Get index of element with specified start index:
```c
custom_type elementtofind = {0};
list_indexof2(&listobj, &elementtofind, 5);
```

### Get index of element
Signature:
```c
indexpair list_indexof3(list* list, void* elementtofind);
```
Get index of element starting from beginning:
```c
custom_type elementtofind = {0};
list_indexof3(&listobj, &elementtofind);
```

### Insert element at specific index
Signature:
```c
indexpair listinsert(struct list* list, void* element, uint32t index);
```
Insert element at specific index:
```c
custom_type newelement = {0};
list_insert(&listobj, &newelement, 5);
```

### Get last index of element within specified range
Signature:
```c
indexpair list_lastindexof1(list* list, void* elementtofind, uint32_t startindex, uint32_t endindex);
```
Get last index of element within specified range:
```c
custom_type elementtofind = {0};
list_lastindexof1(&listobj, &elementtofind, 0, 10);
```

### Get last index of element with start index
Signature:
```c
indexpair list_lastindexof2(list* list, void* elementtofind, uint32_t startindex);
```
Get last index of element with specified start index:
```c
custom_type elementtofind = {0};
list_lastindexof2(&listobj, &elementtofind, 5);
```

### Get last index of element
Signature:
```c
indexpair list_lastindexof3(list* list, void* elementtofind);
```
Get last index of element:
```c
custom_type elementtofind = {0};
list_lastindexof3(&listobj, &elementtofind);
```

### Remove element from the list
Signature:
```c
indexpair list_remove(list* list, void* elementtofind);
```
Remove element from the list:
```c
custom_type element_to_remove = {0};
list_remove(&listobj, &element_to_remove);
```

### Remove all occurrences of element using predicate
Signature:
```c
indexpair list_removeall(list* list, void* elementtofind, predicate comparer);
```
Remove all occurrences of element using predicate:
```c
custom_type element_to_remove = {0};
list_removeall(&listobj, &element_to_remove, comparer);
```

### Remove element at specific index
Signature:
```c
indexpair list_removeat(list* list, uint32_t index);
```
Remove element at specific index:
```c
list_removeat(&listobj, 5);
```

### Reverse the order of elements in the list
Signature:
```c
indexpair list_reverse(list* list);
```
Reverse the order of elements in the list:
```c
list_reverse(&listobj);
```