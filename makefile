
SRCS = $(wildcard ZomsOS/*.c)
OBJ = $(SRCS:%.c=%.o)
DEP = $(OBJ:%.o=%.d)


all: ZomsOS/boot/boot.bin kernel.bin
	cat $^ > disk.img

kernel.bin: ZomsOS/boot/kernel_entry.o $(OBJ)
	$(TARGET)-gcc -T linker.ld -Wl,--oformat=binary -o $@ -nostdlib -ffreestanding -O2 $^ -lgcc

%.bin: %.asm
	nasm $^ -f bin -o $*.bin
	
%.o: %.asm
	nasm $< -f elf -o $*.o

-include $(DEP)

%.o: %.c
	$(TARGET)-gcc -ffreestanding -O2 -Wall -Wextra -MMD -c $< -o $*.o