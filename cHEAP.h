//
// Created by surya on 3.2.24.
//

#ifndef HEAP_MANAGEMENT_HEAP_H
#define HEAP_MANAGEMENT_HEAP_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define cHEAP_VIRTUAL_HEAP_SIZE   0x400 //1024
#define cHEAP_THRESHOLD_SIZE      40
#define cHEAP_DEFAULT_MEM_VALUE   0
#define META_DATA           __attribute__((section(".metadata")))
#define cHEAP_INTRINSIC_ALIGNMENT __attribute__((aligned(16)))
#define cHEAP_STDCALL __stdcall
#define cHEAP_CDECL __cdecl

#define cHEAP_InitializerConstant   (1)
#ifdef _WIN64
#define cHEAP_PUBLIC(type)   __declspec(dllexport) type cHEAP_STDCALL
#else
#define cHEAP_PUBLIC(type)   __attribute__((visibility("default"))) type cHEAP_CDECL
#endif

/**
 * @brief Enumeration defining data types and their corresponding sizes.
 * @details The enumeration provides sizes for various data types used in memory allocation.
 */
typedef enum {
    VOID = 1,
    CHARACTER = 1,
    INT32 = 4,
    FLOAT32 = 4,
    LONG = 8,
    DOUBLE = 8,
    STRING_BUF = 16,
}_dtype;

/**
 * @brief Structure representing memory block information.
 * @details This structure stores metadata for memory blocks, including pointers, availability, length, and consumer.
 */
struct metaData {
    void* heapPtr;          /**< Pointer to the heap block */
    struct metaData* next;  /**< Pointer to the next memory block */
    struct metaData* prior;  /**< Pointer to the previous memory block */
    bool available;        /**< Flag indicating whether the memory block is available */
    int length;            /**< Length of the memory block */
    _dtype consumer;       /**< Data consumer of the memory block */
}; META_DATA

#define cHEAP_MetaDataSize   sizeof(struct metaData)

#ifndef CHEAP_INIT
#define CHEAP_INIT

/**     < Function Declaration>     */

cHEAP_PUBLIC(struct metaData*) _h_node(void* __restrict__ address, int size);

cHEAP_PUBLIC(void) _initializer_();
cHEAP_PUBLIC(void*) cHEAP_AllocateNew(_dtype T, int _Block);
cHEAP_PUBLIC(void) cHEAP_Release(void* _Memory);
cHEAP_PUBLIC(void) cHEAP_Nullify(void* _Memory);
cHEAP_PUBLIC(bool) cHEAP_IfAllocatable(int size);
cHEAP_PUBLIC(int) cHEAP_Length(void* _Memory);
cHEAP_PUBLIC(void) cHEAP_InspectFreeSpace();


cHEAP_PUBLIC(int) cHEAP_GetTotalFreeSpace();
cHEAP_PUBLIC(int) cHEAP_GetTotalAllocatedSpace();
cHEAP_PUBLIC(void) cHEAP_DeallocateAll();

#endif // CHEAP_INIT


#endif //HEAP_MANAGEMENT_HEAP_H

