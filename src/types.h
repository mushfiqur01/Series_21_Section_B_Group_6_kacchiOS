#ifndef TYPES_H
#define TYPES_H

/* Standard Fixed-Width Unsigned Integers */
typedef unsigned char      uint8_t;   /* 1 Byte  */
typedef unsigned short     uint16_t;  /* 2 Bytes */
typedef unsigned int       uint32_t;  /* 4 Bytes */
typedef unsigned long long uint64_t;  /* 8 Bytes */

/* Standard Fixed-Width Signed Integers */
typedef signed char        int8_t;    /* 1 Byte  */
typedef signed short       int16_t;   /* 2 Bytes */
typedef signed int         int32_t;   /* 4 Bytes */
typedef signed long long   int64_t;   /* 8 Bytes */

/* Pointer-sized types for 32-bit x86 architecture */
typedef uint32_t           uintptr_t; /* Unsigned int large enough for a pointer */
typedef int32_t            intptr_t;  /* Signed int large enough for a pointer */
typedef uint32_t           size_t;    /* Type for object sizes */
typedef int32_t            ssize_t;   /* Type for sizes with error codes (-1) */

/* Boolean support (since we don't have stdbool.h) */
typedef enum {
    false = 0,
    true  = 1
} bool;

/* Common Macros */
#ifndef NULL
#define NULL ((void*)0)
#endif

/* PID type for process management consistency */
typedef int32_t            pid_t;

#endif