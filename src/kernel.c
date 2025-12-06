#include "io.h"

void kernel_main(void *multiboot_structure, uint32_t magic_number) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);

	printf("Welcome to keel-os!\n\n");
	printf("Temporal test:\n");
	printf("- MAX UINT32: ");
	print_uint32(UINT32_MAX);
	printf("\n- MIN INT32: ");
	print_int32(INT32_MIN);

	while(1)
		;
}
