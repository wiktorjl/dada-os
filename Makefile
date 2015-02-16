ASM_CMD = nasm
ASM_FLAGS = -felf
LD_CMD = ld
GCC_CMD = gcc
CC_CUST_FLAGS = -g -I /home/wjl/dada-os/src
CC_FLAGS = -std=gnu99 -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs

OBJCOPY = objcopy
OBJCOPY_FLAGS = --only-keep-debug

BUILD_DIR = build
SRC_DIR = src
SCRIPT_DIR = scripts
LINKER_DIR = src
LINKER_SCRIPT = linker.ld
KERNEL_IMAGE = kernel.bin
KERNEL_SYMBOLS = kernel.sym

OBJS = $(SRC_DIR)/kernel.o      \
    $(SRC_DIR)/math.o           \
    $(SRC_DIR)/sys.o            \
    $(SRC_DIR)/io.o             \
    $(SRC_DIR)/idt.o            \
    $(SRC_DIR)/irq.o            \
    $(SRC_DIR)/string.o         \
    $(SRC_DIR)/loader.o         \
    $(SRC_DIR)/console.o        \
    $(SRC_DIR)/kbd.o            \
    $(SRC_DIR)/gdt.o            \
    $(SRC_DIR)/physmem.o        \
    $(SRC_DIR)/atapi.o          \
    $(SRC_DIR)/kheap.o          \
    $(SRC_DIR)/vmm.o            

ASMS = $(SRC_DIR)/loader.o      \
    $(SRC_DIR)/gdtasm.o         \
    $(SRC_DIR)/idtasm.o         \
    $(SRC_DIR)/asmutil.o
	
.PHONY: all

all: clean kernel symbols strip 

strip:
	$(OBJCOPY) --strip-debug $(BUILD_DIR)/$(KERNEL_IMAGE)

symbols:
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(BUILD_DIR)/$(KERNEL_IMAGE) $(BUILD_DIR)/$(KERNEL_SYMBOLS)

%.o: %.c
	$(GCC_CMD) -o $@ -c $< $(CC_CUST_FLAGS) $(CC_FLAGS) 

%.o: %.s
	$(ASM_CMD) $(ASM_FLAGS) -o $@ $<

kernel: $(ASMS) $(OBJS) 
	$(LD_CMD) -T  $(LINKER_DIR)/$(LINKER_SCRIPT) -o $(BUILD_DIR)/$(KERNEL_IMAGE) $(SRC_DIR)/loader.o $(SRC_DIR)/asmutil.o $(SRC_DIR)/math.o $(SRC_DIR)/sys.o $(SRC_DIR)/gdtasm.o $(SRC_DIR)/string.o $(SRC_DIR)/idt.o $(SRC_DIR)/kernel.o $(SRC_DIR)/console.o $(SRC_DIR)/kbd.o $(SRC_DIR)/gdt.o $(SRC_DIR)/idtasm.o $(SRC_DIR)/irq.o $(SRC_DIR)/physmem.o $(SRC_DIR)/kheap.o $(SRC_DIR)/vmm.o $(SRC_DIR)/io.o $(SRC_DIR)/atapi.o

clean: 
	@echo "Removing kernel..."
	rm -fv build/kernel.bin
	rm -fv iso/boot/kernel.bin
	@echo "Removing object files..."
	rm -fv src/*.o
	rm -fv src/experiment/current/*.o
	rm -fv src/utils/*.o
	rm -fv src/ds/*.o
	@echo "Removing ISO..."
	rm -fv bootable.iso
	@echo "Done! All clean."

cleanswp: 
	@echo "Removing swp files..."
	rm -fv src/.*.swp
	rm -fv src/*.*~

makeiso:
	@echo "Removing old ISO"
	rm -fv bootable.iso
	@echo "Copying kernel to ISO directory"
	cp -v build/kernel.bin iso/boot/
	@echo "Creating new ISO"
	grub-mkrescue -o bootable.iso iso

run:
	@echo "Running!"
	scripts/run.sh
	@echo "Done!"
