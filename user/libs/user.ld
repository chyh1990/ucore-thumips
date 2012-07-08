OUTPUT_FORMAT(elf32-tradlittlemips)
OUTPUT_ARCH(mips:isa32)
ENTRY(_start)

SECTIONS
{
  . = 0x10000000;
  .text      :
  {
    . = ALIGN(4);
    wrs_user_text_start = .; _wrs_user_text_start = .;
    *(.startup)
    *(.text) 
    *(.text.*)
    *(.gnu.linkonce.t*)
    *(.mips16.fn.*) 
    *(.mips16.call.*) /* for MIPS */
    *(.rodata) *(.rodata.*) *(.gnu.linkonce.r*) *(.rodata1)
  }
. = ALIGN(16);
wrs_user_text_end = .; _wrs_user_text_end = .;
etext = .; _etext = .;
.data    : AT(etext)
{
  wrs_user_data_start = .; _wrs_user_data_start = .;
  *(.data)
  *(.data.*)
  *(.gnu.linkonce.d*)
  *(.data1)
  *(.eh_frame)
  *(.gcc_except_table)
  . = ALIGN(8);
  _gp = . + 0x7ff0;  /* set gp for MIPS startup code */
    /* got*, dynamic, sdata*, lit[48], and sbss should follow _gp */
  *(.got.plt)
  *(.got)
  *(.dynamic)
  *(.got2)
  *(.sdata) *(.sdata.*) *(.lit8) *(.lit4)
    . = ALIGN(16);
}
. = ALIGN(16);
edata = .; _edata = .;
wrs_user_data_end = .; _wrs_user_data_end = .;
.bss       :
{
  wrs_user_bss_start = .; _wrs_user_bss_start = .;
  *(.sbss) *(.scommon) *(.dynbss) *(.bss) *(COMMON)
  . = ALIGN(16);
}
. = ALIGN(16);
end = .; _end = .;
wrs_user_bss_end = .; _wrs_user_bss_end = .;

/DISCARD/ :
{
  /*
  *(.stub)
  *(.debug*) */
  *(.note)    /* seen on Pentium ELF */
  *(.comment)    /* seen on PPC & SH ELF */
  *(.reginfo)  
  *(.pdr)    /* seen on MIPS ELF */
}

}
