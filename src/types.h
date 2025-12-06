#ifndef KEEL_OS_TYPES_H_
#define KEEL_OS_TYPES_H_

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef int i32;
typedef short i16;
typedef signed char i8;

typedef u32 usize;
typedef i32 isize;

#define I8_MIN (-128)
#define I16_MIN (-32767 - 1)
#define I32_MIN (-2147483647 - 1)

#define I8_MAX (127)
#define I16_MAX (32767)
#define I32_MAX (2147483647)

#define U8_MAX (255)
#define U16_MAX (65535)
#define U32_MAX (4294967295U)

#endif
