#!/bin/bash
cd "$(dirname "$0")"

set -e

CROSS_COMPILE=riscv64-unknown-elf-
CC=${CROSS_COMPILE}gcc
OBJCOPY=${CROSS_COMPILE}objcopy
QEMU=qemu-system-riscv64

CFLAGS="-Wall -O2 -ffreestanding -nostdlib -I../include -mcmodel=medany"
LDFLAGS="-T ../kernel/boot/kernel.ld -nostdlib"

if [ "$1" = "clean" ]; then
    ./clean.sh
    exit 0
fi

# 编译源文件
echo "[1] 编译内核..."
$CC $CFLAGS -c ../kernel/boot/entry.S -o entry.o
$CC $CFLAGS -c ../kernel/main.c -o main.o
$CC $CFLAGS -c ../kernel/driver/uart.c -o uart.o

# 链接
echo "[2] 链接生成 kernel.elf..."
$CC $CFLAGS entry.o main.o uart.o -o ../kernel.elf $LDFLAGS

# 生成二进制镜像
echo "[3] 生成 kernel.bin..."
$OBJCOPY -O binary ../kernel.elf ../kernel.bin

# 运行 QEMU
echo "[4] 启动 QEMU..."
$QEMU -machine virt -nographic -bios none -kernel ../kernel.elf
