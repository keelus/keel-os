#ifndef KEEL_OS_GDT_H_
#define KEEL_OS_GDT_H_

#include "types.h"

struct segment_descriptor {
	u16 limit_lo;
	u16 base_lo;
	u8 base_hi;
	u8 access_byte;
	u8 flags_limit_hi;
	u8 base_vhi;
} __attribute__((packed));

static inline struct segment_descriptor segment_descriptor_create(u32 base, u32 limit, u8 access_byte) {
	struct segment_descriptor sd = {0};

	if(limit <= 65536) {
		sd.flags_limit_hi = 0x40;
	} else {
		// TODO: Read this
		if((limit & 0xFFF) != 0xFFF) {
			limit = (limit >> 12) - 1;
		} else {
			limit >>= 12;
		}

		sd.flags_limit_hi = 0xC0;
	}

	sd.limit_lo = limit & 0xFFFF;
	sd.flags_limit_hi |= (limit >> 16) & 0xF;

	sd.base_lo = base & 0xFFFF;
	sd.base_hi = (base >> 16) & 0xFF;
	sd.base_vhi = (base >> 24) & 0xFF;

	sd.access_byte = access_byte;

	return sd;
}

static inline u32 segment_descriptor_base(const struct segment_descriptor *sd) {
	u32 base = sd->base_lo;
	base |= sd->base_hi << 16;
	base |= sd->base_vhi << 24;
	return base;
}

static inline u32 segment_descriptor_limit(const struct segment_descriptor *sd) {
	u32 limit = sd->limit_lo;
	limit |= (sd->flags_limit_hi & 0xF) << 16;

	if((sd->flags_limit_hi & 0xC0) == 0xC0) { return (limit << 12) | 0xFFF; }
	return limit;
}

struct global_descriptor_table {
	struct segment_descriptor null_segment_selector;
	struct segment_descriptor unused_segment_selector;
	struct segment_descriptor code_segment_selector;
	struct segment_descriptor data_segment_selector;
} __attribute__((packed));

static struct global_descriptor_table gdt = {0};

static inline void global_descriptor_table_initialize() {
	gdt = (struct global_descriptor_table){
		.null_segment_selector = segment_descriptor_create(0, 0, 0),
		.unused_segment_selector = segment_descriptor_create(0, 0, 0),
		.code_segment_selector = segment_descriptor_create(0, 6 * 1024 * 1024, 0x9A),
		.data_segment_selector = segment_descriptor_create(0, 6 * 1024 * 1024, 0x92),
	};

	u32 i[2];
	i[0] = (u32)&gdt;
	i[1] = sizeof(struct global_descriptor_table) << 16;

	asm volatile("lgdt (%0)" : : "p"(((u8 *)i) + 2));
}

static inline u16 global_descriptor_table_code_segment_selector() {
	return (u32)&gdt.code_segment_selector - (u32)&gdt;
}

static inline u16 global_descriptor_table_data_segment_selector() {
	return (u32)&gdt.data_segment_selector - (u32)&gdt;
}

typedef unsigned int u32;

#endif
