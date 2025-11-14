    .section .text.entry
    .globl _start
_start:
    # 设置栈指针
    la sp, stack_top

    # 调试：输出 'S' 表示启动成功
    # li t0, 0x10000000
    # li t1, 'S'
    # sb t1, 0(t0)

    # 清零 BSS 段
    call clear_bss

    # 跳转到 C main
    call main

1:  j 1b    # 如果 main 返回，死循环

# BSS 清零函数
clear_bss:
    la t0, __bss_start
    la t1, __bss_end
    bge t0, t1, clear_done
clear_loop:
    sd zero, 0(t0)
    addi t0, t0, 8
    blt t0, t1, clear_loop
clear_done:
    ret

# 栈空间
    .section .bss
    .align 16
stack_bottom:
    .space 0x4000   # 16KB 栈
stack_top: