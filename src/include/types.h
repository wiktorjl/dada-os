/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */
#ifndef types_h
#define types_h

typedef enum { FALSE=0, TRUE } bool_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;

typedef char                sint8_t;
typedef short               sint16_t;
typedef int                 sint32_t;
typedef long long           sint64_t;
typedef unsigned long long  uint64_t;
typedef uint32_t            addr_t;
typedef uint32_t            offset_t;
typedef uint64_t            loffset_t;
typedef uint32_t            time_t;
typedef uint32_t            size_t;
typedef sint32_t            pid_t;
typedef uint8_t             ret_t;
typedef uint8_t             byte_t;

#endif
