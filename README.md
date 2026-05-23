# MyOS

A hobby operating system built from scratch in C and assembly using the GRUB multiboot standard.

---

## Overview

MyOS is a learning-focused operating system project exploring low-level systems programming, including bootloading, memory layout, input handling, and simple graphical interfaces in VGA text mode.

This project is actively developed and experimental in nature.

---

## Features

- VGA text-mode graphical interface
- PS/2 mouse support (polling-based input)
- Draggable window system
- Software-based framebuffer logic
- GRUB multiboot bootloader support

---

## Screenshots

not yet

---

## Build Instructions

### Requirements
- GCC (i386 cross or multilib)
- GNU Make
- GRUB tools
- QEMU (for testing)

---

### Build

```bash
make
Run in QEMU
make run
Project Structure
myos/
├── kernel/
│   └── kernel.c
│   └── boot.s
├── grub/
│   └── grub.cfg
├── isodir/
├── build/
├── linker.ld
├── Makefile
└── README.md
Notes

This is an experimental operating system project created for educational purposes.
It is not intended for production use.

License

For learning and personal use.
