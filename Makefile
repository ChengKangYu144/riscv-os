# 交叉编译工具链
CROSS_COMPILE ?= riscv64-unknown-elf-
CC  := $(CROSS_COMPILE)gcc
LD  := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

# QEMU 路径
QEMU := qemu-system-riscv64

# 编译选项
CFLAGS = -Wall -O2 -ffreestanding -nostdlib -march=rv64imac_zicsr -mabi=lp64 -mcmodel=medany
LDFLAGS := -T kernel/boot/kernel.ld

# 目录结构
KERNEL_DIR := kernel
BOOT_DIR   := $(KERNEL_DIR)/boot
INCLUDE_DIR:= include
DRIVER_DIR := $(KERNEL_DIR)/driver
MM_DIR     := $(KERNEL_DIR)/mm
LIB_DIR    := $(KERNEL_DIR)/lib

# 源码文件
OBJS := \
	$(BOOT_DIR)/entry.o \
	$(KERNEL_DIR)/main.o \
	${KERNEL_DIR}/test.o \
	$(DRIVER_DIR)/uart.o \
	$(KERNEL_DIR)/console.o \
	$(MM_DIR)/pmm.o \
	$(MM_DIR)/vmm.o \
	$(MM_DIR)/page.o \
	$(LIB_DIR)/string.o \
	$(LIB_DIR)/assert.o

# 最终目标
TARGET := kernel.elf
BIN    := kernel.bin

# 默认目标
all: $(BIN)

# 生成二进制
$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $< $@
	@echo "[3] 生成 $@ 完成"

# 链接
$(TARGET): $(OBJS)
	@echo "[2] 链接生成 $@ ..."
	$(LD) $(LDFLAGS) -o $@ $^

# 编译 C 文件
%.o: %.c
	@echo "[1] 编译 $< ..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# 编译汇编文件
%.o: %.S
	@echo "[1] 编译 $< ..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# 运行
run: $(BIN)
	@echo "[4] 启动 QEMU..."
	$(QEMU) -machine virt -nographic -bios none -kernel $(TARGET)

# 清理
clean:
	rm -f $(OBJS) $(TARGET) $(BIN)

.PHONY: all run clean
