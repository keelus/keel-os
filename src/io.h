#ifndef KEEL_OS_IO_H_
#define KEEL_OS_IO_H_

#include "types.h"

#define COLS 80
#define ROWS 25

extern unsigned short *video_mem;
extern uint8_t x, y;

static inline void putchar(const char c) {
	if(c == '\n') {
		uint8_t cur_y = y;
		while(y == cur_y) {
			putchar(' ');
		}
		return;
	}

	video_mem[COLS * y + x] = (video_mem[COLS * y + x] & 0xFF00) | c;

	x++;
	if(x > COLS) {
		y++;
		x = 0;
	}
}

static inline void printf(const char *str) {
	for(int i = 0; str[i] != '\0'; i++) {
		putchar(str[i]);
	}
}

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}

#define DECLARE_PRINT_UINT(w)                                                  \
	static inline void print_uint##w(uint##w##_t n) {                          \
		if(n == 0) {                                                           \
			putchar('0');                                                      \
			return;                                                            \
		}                                                                      \
                                                                               \
		char buf[w];                                                           \
		uint8_t i = 0;                                                         \
                                                                               \
		while(n) {                                                             \
			uint8_t d = n % 10;                                                \
			buf[i++] = d + '0';                                                \
			n /= 10;                                                           \
		}                                                                      \
                                                                               \
		for(int8_t j = i - 1; j >= 0; j--) {                                   \
			putchar(buf[j]);                                                   \
		}                                                                      \
	}

#define DECLARE_PRINT_INT(w)                                                   \
	static inline void print_int##w(int##w##_t n) {                            \
		if(n < 0) {                                                            \
			putchar('-');                                                      \
			print_uint##w(-n);                                                 \
		}                                                                      \
	}

DECLARE_PRINT_UINT(8)
DECLARE_PRINT_UINT(16)
DECLARE_PRINT_UINT(32)

DECLARE_PRINT_INT(8)
DECLARE_PRINT_INT(16)
DECLARE_PRINT_INT(32)

#endif
