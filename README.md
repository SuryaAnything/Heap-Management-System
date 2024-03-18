# Heap Management System

This project implements a heap management system in C, providing functionalities for dynamic memory allocation, deallocation, and memory usage inspection.

## Contents

1. [Introduction](#introduction)
2. [Components](#components)
3. [Memory Management Functions](#memory-management-functions)
4. [Error Handling](#error-handling)
5. [Usage](#usage)
6. [Contributing](#contributing)

## Introduction

The heap management system consists of a header file (`heap.h`) and an implementation file (`cHEAP.c`). It includes functionalities to allocate, deallocate, and inspect memory blocks within a fixed-size memory pool.

## Components

- **Header File (`heap.h`)**:
  - Defines constants, data types, and function prototypes.
  - Contains macros for error handling.

- **Implementation File (`cHEAP.c`)**:
  - Implements memory management functions.
  - Defines a fixed-size memory pool and free list for managing memory blocks.

## Memory Management Functions

1. `_initializer_()`: Initializes the heap.
2. `cHEAP_AllocateNew()`: Allocates memory blocks.
3. `cHEAP_Release()`: Deallocates memory blocks.
4. `cHEAP_InspectFreeSpace()`: Prints information about free memory blocks.
5. `cHEAP_Length()`: Gets the size of an allocated memory block.
6. `cHEAP_IfAllocatable()`: Checks if memory of a given size can be allocated.
7. `cHEAP_Nullify()`: Nullifies memory contents.
8. `cHEAP_GetTotalFreeSpace()`: Gets total free space in the heap.
9. `cHEAP_GetTotalAllocatedSpace()`: Gets total allocated space in the heap.
10. `cHEAP_DeallocateAll()`: Deallocates all memory blocks in the heap.

## Error Handling

The system includes error handling mechanisms using `errno` and custom error messages for scenarios like segmentation faults, non-accessible memory, attempted deallocation of already freed memory, and illegal memory access.

## Usage

To use the heap management system, include the `cHEAP.h` header file in your project and link it with the `cHEAP.c` implementation file. Then, you can call the provided memory management functions as needed.

