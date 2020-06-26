#ifndef TYPES_H
#define TYPES_H
# include <assert.h> 

#define internal static 
#define local_persist static 
#define global_variable static


#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define Pi32 3.14159265359f

typedef float R32;
typedef double R64;
typedef int8_t Z8;
typedef int16_t Z16;
typedef int32_t Z32;
typedef int64_t Z64;
typedef Z32  bool32;

//Natural Numbers start at 0.  Fuck you, don't @me
typedef uint8_t N8;
typedef uint16_t N16;
typedef uint32_t N32;
typedef uint64_t N64;

#endif