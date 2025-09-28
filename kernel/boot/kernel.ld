OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
  . = 0x80000000;

  .text : {
    *(.text.entry)
    *(.text .text.*)
    *(.rodata .rodata.*)
}

  .data : {
    *(.data .data.*)
    *(.sdata .sdata.*)
    edata = .;
  }

  .bss : {
    __bss_start = .;
    *(.bss .bss.*)
    *(.sbss .sbss.*)
    __bss_end = .;
  }

  end = .;

  .stack (NOLOAD) : {
    . = ALIGN(16);
    stack_bottom = .;
    . = . + 16K;
    stack_top = .;
  }
}
