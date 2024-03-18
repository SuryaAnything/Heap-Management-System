//
// Created by surya on 3.2.24.
//

#include "cHEAP.h"
#include <stdlib.h>


#define throw \
fprintf(stderr,"\nException at line %d : in file %s\n",__LINE__,__FILE__);
#define raise \
fprintf(stderr,"Error at line %d : in file %s\n",__LINE__,__FILE__);
#define SegmentationError() \
perror("\nSegmentation Fault "); exit(EXIT_FAILURE)
#define NonAccessibleException() \
perror("\nAttempted deallocation of already freed memory block. ")
#define NullPointerException() \
perror("\nRequired Memory block Found NULL ")
#define IllegalMemoryAccessException() \
perror("\nAttempted to free a memory block not belonging to the heap ")

/**
 * @brief Declaration of the fixed-size memory pool.
 * @details This pool is used for memory allocation and deallocation operations.
 */
cHEAP_INTRINSIC_ALIGNMENT static char _pool[cHEAP_VIRTUAL_HEAP_SIZE] = {cHEAP_DEFAULT_MEM_VALUE};


/**
 * @brief Structure representing the free list.
 * @details This structure maintains a linked list of available memory blocks.
 */
struct _hfBlock {
    struct metaData* head;  /**< Pointer to the head of the free list */
    int capacity;          /**< Total capacity of the memory pool */
    bool init;             /**< Flag indicating whether the memory pool has been initialized */
}freeList;

/**
 * @brief Function to create a cHEAP_AllocateNew memory node
 *
 * @param address Starting address of the memory block
 * @param size Size of the memory block
 * @return Pointer to the newly created memory node
 */
/***********************************************************************************************
 * @note This function is intended for INTERNAL use only and is not part of the PUBLIC interface.
 **************************************************************************************************
 */
cHEAP_PUBLIC(struct metaData*) _h_node(void* __restrict__ address, int size) {
    struct metaData* node1 = (struct metaData*)address;
    node1->length = size - cHEAP_MetaDataSize;
    node1->available = true;
    node1->prior = NULL;
    node1->next = NULL;
    node1->heapPtr = address + cHEAP_MetaDataSize;
    return node1;
}

/**
 * @brief Function to initialize the heap
 */
/************************************************************************************************
 * @note This function is intended for INTERNAL use only and is not part of the PUBLIC interface.
 * It is discouraged for users to explicitly call this function.
 * The function is implicitly called when cHEAP_AllocateNew() is called for the first time.
 **************************************************************************************************/
cHEAP_PUBLIC(void) _initializer_() {
    freeList.head = _h_node(_pool, cHEAP_VIRTUAL_HEAP_SIZE);
    freeList.capacity = cHEAP_VIRTUAL_HEAP_SIZE - cHEAP_MetaDataSize;
    freeList.init = cHEAP_InitializerConstant;
}

/**
 * @brief Function to allocate a cHEAP_AllocateNew memory _Block
 *
 * @param _Block Number of blocks to allocate
 * @param T Data consumer size multiplier
 * @return Pointer to the allocated memory _Block
 */
cHEAP_PUBLIC(void*) cHEAP_AllocateNew(_dtype T, int _Block) {

    errno = 0;
    if (freeList.init!=1) { _initializer_(); }

    _Block = _Block * T;

    if (_Block==0) { return NULL; }

    struct metaData* temp = freeList.head;
    if (freeList.capacity > _Block){
        while (temp && ((_Block) > temp->length || !temp->available)) {
            temp = temp->next;
        }

        if (temp) {
            if ((temp->length - _Block) > cHEAP_THRESHOLD_SIZE){
                // split code
                struct metaData *node2 = _h_node(temp->heapPtr + _Block, temp->length - _Block);
                node2->next = temp->next;
                node2->prior = temp;
                temp->next = node2;
                temp->available = false;
                temp->length = _Block;
                freeList.capacity = freeList.capacity - _Block - cHEAP_MetaDataSize;
            }
            temp->consumer = T;
            return temp->heapPtr;
        }
    }
    errno = 12;
    raise SegmentationError();
}

/**
 * @brief Function to deallocate a memory block
 *
 * @param _Memory Pointer to the memory block to deallocate
 */
cHEAP_PUBLIC(void) cHEAP_Release(void* _Memory) {
    errno = 0;
    if (_Memory==NULL) {
        errno = EINVAL;
        throw NullPointerException();
    }

    struct metaData* temp = freeList.head;

    while(temp && temp->heapPtr != _Memory) {
        temp = temp->next;
    }

    if (temp==NULL) {
        errno = 1;
        throw IllegalMemoryAccessException();
        return;
    }

    if (temp->available) {
        errno = 1;
        throw NonAccessibleException();
        return;
    }

    temp->available = true;
    freeList.capacity += temp->length + cHEAP_MetaDataSize;
    if (temp->next!=NULL) {
        if (temp->next->available) {
            temp->length += temp->next->length + cHEAP_MetaDataSize;
            temp->next = temp->next->next;
            if (temp->next!=NULL) {
                temp->next->prior = temp;
            }
        }
    }

    if (temp->prior != NULL) {
        if (temp->prior->available) {
            temp->prior->length += temp->length + cHEAP_MetaDataSize;
            temp->prior->next = temp->next;
            if (temp->next!=NULL) {
                temp->next->prior = temp->prior;
            }
        }
    }
    _Memory = NULL;
}

/**
 * @brief Function to print the free memory blocks in the heap
 */
cHEAP_PUBLIC(void) cHEAP_InspectFreeSpace() {
    if (freeList.init!=1) { _initializer_(); }
    struct metaData* temp = freeList.head;
    printf("\n\n");
    printf("+----------------+------+\n");
    printf("|     Address    | Size |\n");

    while (temp!=NULL) {
        if  (temp->available) {
            printf("+----------------+------+\n");
            printf("|%p| %d  |\n", &temp->heapPtr, temp->length);
        }
        temp = temp->next;
    }
    printf("+----------------+------+\n");
}

/**
 * @brief Function to get the size of an allocated memory block
 *
 * @param _Memory Pointer to the allocated memory block
 * @return Size of the allocated memory block
 */
cHEAP_PUBLIC(int) cHEAP_Length(void* _Memory) {
    if (_Memory == NULL) {
        errno = EINVAL;
        throw NullPointerException();
    }
    struct metaData* temp = freeList.head;
    while (temp && temp->heapPtr != _Memory) {
        temp = temp->next;
    }
    if (temp!=NULL && !temp->available) {
        return temp->length / temp->consumer;
    }
    return 0;
}

/**
 * @brief Function to check if a memory block of a given size can be allocated
 *
 * @param size Size of the memory block to check
 * @return True if allocatable, False otherwise
 */
cHEAP_PUBLIC(bool) cHEAP_IfAllocatable(int size) {
    struct metaData* temp = freeList.head;
    while (temp && (temp->length < size || !temp->available)) {
        temp = temp->next;
    }
    if (temp) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Function to cHEAP_Nullify memory contents.
 * @param _Memory Pointer to the memory block to be nullified.
 * @details This function sets the contents of the specified memory block to a default value.
 */
cHEAP_PUBLIC(void) cHEAP_Nullify(void* _Memory) {
    struct metaData* temp = freeList.head;
    while (temp && temp->heapPtr != _Memory) {
        temp = temp->next;
    }
    int value = cHEAP_DEFAULT_MEM_VALUE;
    if (temp != NULL && !temp->available) {
        for (int iterator = 0; iterator < temp->length; ++iterator) {
            memcpy(&((char*)_Memory)[iterator], &value, temp->consumer);
        }
    }
}

/**
 * @brief Function to get the total amount of free space available in the heap.
 *
 * @return Total free space in the heap.
 */
cHEAP_PUBLIC(int) cHEAP_GetTotalFreeSpace() {
    return freeList.capacity;
}

/**
 * @brief Function to get the total amount of space currently allocated in the heap.
 *
 * @return Total allocated space in the heap.
 */
cHEAP_PUBLIC(int) cHEAP_GetTotalAllocatedSpace() {
    struct metaData *temp = freeList.head;
    int size = 0;
    while (temp) {
        if (!temp->available)
        {
            size += temp->length;
        }
        temp = temp->next;
    }
    return size;
}

/**
 * @brief Function to deallocate all memory blocks in the heap.
 */
cHEAP_PUBLIC(void) cHEAP_DeallocateAll() {
    struct metaData *temp = freeList.head;
    while (temp) {
        if (!temp->available) {
            cHEAP_Release(temp->heapPtr);
        }
        temp = temp->next;
    }
}

