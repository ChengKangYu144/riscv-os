    .section .text.entry
    .globl _start
_start:
    # 设置栈指针
    la sp, stack_top

    # 调试：输出 'S' 表示启动成功
    # li t0, 0x10000000
    # li t1, 'S'
    # sb t1, 0(t0)

    # 跳转到 C main
    call main

1:  j 1b    # 如果 main 返回，死循环
