CC = gcc
AS = as
LD = ld

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m elf_i386

KERNEL = build/kernel.bin

all: iso

build:
	mkdir -p build

kernel: build
	$(AS) --32 kernel/boot.s -o build/boot.o
	$(CC) $(CFLAGS) -c kernel/kernel.c -o build/kernel.o
	$(LD) $(LDFLAGS) -T linker.ld -o $(KERNEL) build/boot.o build/kernel.o

iso: kernel
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/kernel.bin
	cp grub/grub.cfg isodir/boot/grub/
	grub-mkrescue -o myos.iso isodir

run: iso
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -rf build isodir myos.iso
