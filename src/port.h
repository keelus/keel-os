#ifndef KEEL_OS_PORT_H_
#define KEEL_OS_PORT_H_

#include "types.h"

static inline void port_write_8(const u16 port, u8 data) {
	asm volatile("outb %b0, %w1" : : "a"(data), "Nd"(port) : "memory");
}

static inline void port_write_8_slow(const u16 port, u8 data) {
	asm volatile("outb %b0, %w1\njmp 1f\n1: jmp 1f\n1:" : : "a"(data), "Nd"(port) : "memory");
}

static inline u8 port_read_8(const u16 port) {
	u8 ret;
	asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}

static inline void port_write_16(const u16 port, u16 data) {
	asm volatile("outw %w0, %w1" : : "a"(data), "Nd"(port) : "memory");
}

static inline u16 port_read_16(const u16 port) {
	u16 ret;
	asm volatile("inw %w1, %w0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}

static inline void port_write_32(const u16 port, u32 data) {
	asm volatile("outl %0, %w1" : : "a"(data), "Nd"(port) : "memory");
}

static inline u32 port_read_32(const u16 port) {
	u32 ret;
	asm volatile("inl %w1, %0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}

#endif
