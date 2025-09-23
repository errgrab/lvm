#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

typedef void		u0;
typedef u0*			u0p;

typedef char		chr_t;
typedef chr_t*		chr_p;

typedef bool		u1;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef uint8_t*	u8p;
typedef uint16_t*	u16p;
typedef uint32_t*	u32p;
typedef uint64_t*	u64p;

typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;
typedef int64_t		i64;

typedef int8_t*		i8p;
typedef int16_t*	i16p;
typedef int32_t*	i32p;
typedef int64_t*	i64p;

typedef size_t		usize;
typedef ssize_t		isize;

typedef size_t*		usizep;
typedef ssize_t*	isizep;

#endif // TYPES_H
