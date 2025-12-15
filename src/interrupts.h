#ifndef KEEL_OS_INTERRUPTS_H_
#define KEEL_OS_INTERRUPTS_H_

#include "types.h"
#include "gdt.h"
#include "port.h"
#include "io.h"

struct gate_descriptor {
	u16 handler_address_low_bits;
	u16 gdt_code_segment_selector;
	u8 reserved;
	u8 access;
	u16 handler_address_high_bits;
} __attribute__((packed));

struct interrupt_descriptor_table_pointer {
	u16 size;
	u32 base;
} __attribute__((packed));

static struct gate_descriptor idt[256];

static struct Port pic_master_command = {};
static struct Port pic_master_data = {};
static struct Port pic_slave_command = {};
static struct Port pic_slave_data = {};

void set_interrupt_descriptor_table_entry(u8 interrupt_number, u16 code_segment_selector_offset, void (*handler)(),
										  u8 descriptor_privilege_level, u8 descriptor_type) {
	const u8 IDT_DESC_PRESENT = 0x80;

	idt[interrupt_number].handler_address_low_bits = ((u32)handler) & 0xFFFF;
	idt[interrupt_number].handler_address_high_bits = (((u32)handler) >> 16) & 0xFFFF;
	idt[interrupt_number].gdt_code_segment_selector = code_segment_selector_offset;
	idt[interrupt_number].reserved = 0;
	idt[interrupt_number].access = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privilege_level & 3) << 5);
}

u32 handle_interrupt(u8 interrupt_number, u32 esp) {
	printf("INTERRUPT\n");
	return esp;
}

extern void ignore_interrupt_request();
extern void handle_interrupt_request0x00();
extern void handle_interrupt_request0x01();

void interrupt_manager_initialize() {
	u16 code_segment = global_descriptor_table_code_segment_selector();
	const u8 IDT_INTERRUPT_GATE = 0xE;

	for(u16 i = 0; i < 256; i++) {
		set_interrupt_descriptor_table_entry(i, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);
	}
	set_interrupt_descriptor_table_entry(0x20, code_segment, &handle_interrupt_request0x00, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_table_entry(0x21, code_segment, &handle_interrupt_request0x01, 0, IDT_INTERRUPT_GATE);

	pic_master_command.number = 0x20;
	pic_master_data.number = 0x21;
	pic_slave_command.number = 0xA0;
	pic_slave_data.number = 0xA1;

	port_write_8_slow(pic_master_command, 0x11);
	port_write_8_slow(pic_slave_command, 0x11);

	port_write_8_slow(pic_master_data, 0x20);
	port_write_8_slow(pic_slave_data, 0x28);

	port_write_8_slow(pic_master_data, 0x04);
	port_write_8_slow(pic_slave_data, 0x02);

	port_write_8_slow(pic_master_data, 0x01);
	port_write_8_slow(pic_slave_data, 0x01);

	port_write_8_slow(pic_master_data, 0x00);
	port_write_8_slow(pic_slave_data, 0x00);

	struct interrupt_descriptor_table_pointer idtp;
	idtp.size = 256 * sizeof(struct gate_descriptor) - 1;
	idtp.base = (u32)idt;
	asm volatile("lidt %0" : : "m"(idtp));
}

void interrupt_manager_activate() {
	asm("sti");
}

#endif
