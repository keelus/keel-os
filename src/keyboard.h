#ifndef KEEL_OS_KEYBOARD_H_
#define KEEL_OS_KEYBOARD_H_

#include "types.h"
#include "io.h"
#include "port.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64

void keyboard_initialize() {
	while(port_read_8(KEYBOARD_COMMAND_PORT) & 0x1) {
		port_read_8(KEYBOARD_DATA_PORT);
	}
	port_write_8_slow(KEYBOARD_COMMAND_PORT, 0xAE);
	port_write_8_slow(KEYBOARD_COMMAND_PORT, 0x20);
	u8 status = (port_read_8(KEYBOARD_DATA_PORT) | 1) & ~0x10;
	port_write_8_slow(KEYBOARD_COMMAND_PORT, 0x60);
	port_write_8_slow(KEYBOARD_DATA_PORT, status);
	port_write_8_slow(KEYBOARD_DATA_PORT, 0xF4);
}

// clang-format off
char key_map[0x40] = {
	0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'0', '-', '=', 0x0, 0x0, 'q', 'w', 'e', 'r', 't', 'y',
	'u', 'i', 'o', 'p', '[', ']', '\n', 0x0, 'a', 's', 'd',
	'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0x0, '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0,
	'*', 0x0, ' '
};
// clang-format on

u32 keyboard_handle_interrupt(u32 esp) {
	u8 key = port_read_8(KEYBOARD_DATA_PORT);
	if(key == 0x61 || key == 0xFA) { return esp; }
	u8 key_up = (key & 0x80) != 0;
	u8 key_down = !key_up;

	key &= 0x7F;
	if(key >= sizeof(key_map) || !key_map[key]) { return esp; }

	if(key_up) { return esp; }

	putchar(key_map[key]);

	return esp;
}

#endif
