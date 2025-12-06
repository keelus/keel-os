CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin

AS = as
ASFLAGS = --32

LD = ld
LDFLAGS = -melf_i386

objects = obj/loader.o \
			 obj/kernel.o

all: keel-os.bin

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

keel-os.bin: linker.ld $(objects)
	$(LD) $(LDFLAGS) -T $< -o $@ $(objects)

iso: keel-os.bin
	mkdir -p iso/boot/grub
	cp keel-os.bin iso/boot
	echo "set timeout=0" > iso/boot/grub/grub.cfg
	echo "set default=0" >> iso/boot/grub/grub.cfg
	echo "" >> iso/boot/grub/grub.cfg
	echo "menuentry \"keel-os (32-bit)\" {" >> iso/boot/grub/grub.cfg
	echo "	multiboot /boot/keel-os.bin" >> iso/boot/grub/grub.cfg
	echo "	boot" >> iso/boot/grub/grub.cfg
	echo "}" >> iso/boot/grub/grub.cfg
	nix-shell -p grub2 xorriso --run "grub-mkrescue --output=keel-os.iso iso"
	rm -rf iso

run: iso
	qemu-system-i386 --drive file=keel-os.iso,format=raw -display sdl

clean:
	find . -name "*.o" -delete
	rm -rf *.bin *.iso iso

.PHONY: clean iso run
