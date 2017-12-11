INCLUDE "rtl8195a_rom.h"
MEMORY
{
  TCM (rwx) : ORIGIN = 0x1FFF0000, LENGTH = 0x00010000
  DATA_RAM (rwx) : ORIGIN = 0x10002100, LENGTH = 0x10007000 - 0x10002100
  SRAM1 (rwx) : ORIGIN = 0x10007000, LENGTH = 0x10070000 - 0x10007000
  SRAM2 (rwx) : ORIGIN = 0x30000000, LENGTH = 2M
}
StackSize = 0x1000;
ENTRY(Reset_Handler)
SECTIONS
{
    .image2.table :
    {
        KEEP(*(SORT(.image2.ram.data*)))
        KEEP(*(.image2.validate.rodata*))
    } > SRAM2
    .text.sram1 :
    {
        . = ALIGN(4);
        *rtl8195a_crypto.o (.text* .rodata*)
        *mbedtls*.o (.text* .rodata*)
        *libc.a: (.text* .rodata*)
    } > SRAM1
    .text.sram2 :
    {
        . = ALIGN(4);
        *(.mon.ram.text*)
        *(.hal.flash.text*)
        *(.hal.sdrc.text*)
        *(.hal.gpio.text*)
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
        *(.rodata*)
        KEEP(*(.eh_frame*))
    } > SRAM2
    __etext = .;
    __data_start__ = .;
    .data.sram1 :
    {
        . = ALIGN(4);
        __sram_data_start__ = .;
        *rtl8195a_crypto*.o (.data*)
        *mbedtls*.o (.data*)
        __sram_data_end__ = .;
    } > SRAM1
    .data.sram2 :
    {
        __sdram_data_start__ = .;
        *(vtable)
        *(.data*)
        *(.sdram.data*)
        . = ALIGN(4);
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);
        . = ALIGN(4);
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);
        . = ALIGN(4);
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);
        . = ALIGN(4);
        __sdram_data_end__ = .;
    } > SRAM2
    __data_end__ = .;
    __image2_end__ = .;
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > SRAM2
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > SRAM2
    __exidx_end = .;
    .bss.sram1 (NOLOAD) :
    {
        __bss_sram_start__ = .;
        *rtl8195a_crypto.o (.bss* COMMON)
        *mbedtls*.o (.bss* COMMON)
        *(.bss.thread_stack_main)
        __bss_sram_end__ = .;
    } > SRAM1
    .bss.sram2 (NOLOAD) :
    {
        __bss_start__ = .;
        __bss_dram_start__ = .;
        *(.bss*)
        *(COMMON)
        *(.bdsram.data*)
        __bss_dram_end__ = .;
        __bss_end__ = .;
    } > SRAM2
    .bf_data :
    {
        __buffer_data_start__ = .;
        *(.bfsram.data*)
        __buffer_data_end__ = .;
    } > SRAM2
    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
    . = ORIGIN(SRAM1) + LENGTH(SRAM1) - StackSize;
        __HeapLimit = .;
    } > SRAM1
    .TCM_overlay :
    {
        __bss_dtcm_start__ = .;
        *lwip_mem.o (.bss*)
        *lwip_memp.o (.bss*)
        *(.tcm.heap*)
        __bss_dtcm_end__ = .;
    } > TCM
    .stack_dummy (NOLOAD):
    {
        __StackLimit = .;
        *(.stack)
    . += StackSize - (. - __StackLimit);
    } > SRAM1
    __StackTop = ORIGIN(SRAM1) + LENGTH(SRAM1);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);
    ASSERT(__StackLimit >= __HeapLimit, "region RAM exceeds ram limit")
}
