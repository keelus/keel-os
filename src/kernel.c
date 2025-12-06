#include "gdt.h"
#include "io.h"

void kernel_main(void *multiboot_structure, u32 magic_number) {
	printf("Welcome to keel-os!\n\n");

	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);

	global_descriptor_table_initialize();

	printf("Temporal test:\n");
	printf("- MAX u32: ");
	print_u32(U32_MAX);
	printf("\n- MIN i32: ");
	print_i32(I32_MIN);

	while(1)
		;
}
