#ifndef KEEL_OS_IO_H_
#define KEEL_OS_IO_H_

#include "types.h"

#define COLS 80
#define ROWS 25

static unsigned short *video_mem = (unsigned short *)0xb8000;
static u8 x = 0, y = 0;

static inline void scroll_up() {
	for(u8 local_y = 0; local_y < ROWS - 1; local_y++) {
		for(u8 local_x = 0; local_x < COLS; local_x++) {
			video_mem[COLS * local_y + local_x] = video_mem[COLS * (local_y + 1) + local_x];
		}
	}
	for(u8 local_x = 0; local_x < COLS; local_x++) {
		video_mem[COLS * (ROWS - 1) + local_x] = (video_mem[COLS * (ROWS - 1) + local_x] & 0xFF00) | ' ';
	}
	y--;
}

static inline void putchar(const char c) {
	if(c == '\n') {
		for(usize local_x = x; local_x < COLS; local_x++) {
			putchar(' ');
		}
		y++;
		x = 0;
		if(y >= ROWS) { scroll_up(); }
		return;
	}

	if(x >= COLS) {
		y++;
		x = 0;
	}
	if(y >= ROWS) { scroll_up(); }
	video_mem[COLS * y + x] = (video_mem[COLS * y + x] & 0xFF00) | c;
	x++;
}

static inline void printf(const char *str) {
	for(int i = 0; str[i] != '\0'; i++) {
		putchar(str[i]);
	}
}

static inline void outb(u16 port, u8 val) {
	asm volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline u8 inb(u16 port) {
	u8 ret;
	asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}

#define DECLARE_PRINT_U(w)                                                                                             \
	static inline void print_u##w(u##w n) {                                                                            \
		if(n == 0) {                                                                                                   \
			putchar('0');                                                                                              \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		char buf[w];                                                                                                   \
		u8 i = 0;                                                                                                      \
                                                                                                                       \
		while(n) {                                                                                                     \
			u8 d = n % 10;                                                                                             \
			buf[i++] = d + '0';                                                                                        \
			n /= 10;                                                                                                   \
		}                                                                                                              \
                                                                                                                       \
		for(i8 j = i - 1; j >= 0; j--) {                                                                               \
			putchar(buf[j]);                                                                                           \
		}                                                                                                              \
	}

#define DECLARE_PRINT_I(w)                                                                                             \
	static inline void print_i##w(i##w n) {                                                                            \
		if(n < 0) {                                                                                                    \
			putchar('-');                                                                                              \
			print_u##w(-n);                                                                                            \
		}                                                                                                              \
	}

DECLARE_PRINT_U(8)
DECLARE_PRINT_U(16)
DECLARE_PRINT_U(32)

DECLARE_PRINT_I(8)
DECLARE_PRINT_I(16)
DECLARE_PRINT_I(32)

#endif
