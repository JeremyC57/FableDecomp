// GFROR13(unsigned long) -- rotate-right-by-13 hash step (__fastcall)
// Leaf: GFROR13
// Retail-asm: 0x00497891 `mov [esp],ecx` + 0x00497894 `ror dword ptr [esp],0Dh` rotates the fastcall argument's stack home in memory; VC7.1 C++ (_lrotr or (v>>13)|(v<<19)) rotates in a register (`mov eax,ecx; ror eax,0Dh`), so the original was `__asm ror value, 13` on the parameter.
long __fastcall GFROR13(unsigned long value)
{
    __asm ror value, 13
    return value;
}
