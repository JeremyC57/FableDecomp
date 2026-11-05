// GFROR13(unsigned long) -- rotate-right-by-13 hash step (__fastcall)
// Leaf: GFROR13
// Retail-asm: 0x00497891 `mov [esp],ecx` + 0x00497894 `ror dword ptr [esp],0Dh` rotates the fastcall argument's stack home in memory; VC7.1 C++ (_lrotr or (v>>13)|(v<<19)) rotates in a register (`mov eax,ecx; ror eax,0Dh`), so the original was `inline-asm ror value, 13` on the parameter.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/GFROR13.h (byte-matching path + FABLE_PORTABLE twin);
//   the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family ROR.
#include "engine/GFROR13.h"
long __fastcall GFROR13(unsigned long value)
{
    GF_ROR13(value);
    return value;
}
