#!/bin/bash
# 清理编译产物
cd "$(dirname "$0")"

set -e

echo "[*] Cleaning build artifacts..."
rm -f *.o
rm -f ../kernel.elf ../kernel.bin
echo "[*] Clean done."
