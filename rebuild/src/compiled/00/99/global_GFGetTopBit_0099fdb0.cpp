// GFGetTopBit(unsigned long value) -- index of the highest set bit, floor(log2(value)): -1 for 0, 31 for values with the top bit set, else a branch-free five-step binary search over (value + 1)
// Leaf: ?GFGetTopBit@@YIKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099FDB0, length 123 bytes. Callees: none.
// Name (agentNF3, 2026-11-10): FableWin.pdb ?GFGetTopBit@@YIKK@Z (FableWin 0x02C87570; __fastcall, unsigned long in and out), listed
//   next to ?GFGetNextPowerOfTwo@@YIKK@Z, whose body is `1 << GFGetTopBit(2 * value - 1)` (the expansion retail inlines twice into
//   CTexture::GetNextValidDimensions 0x009FA620, landed on engine/GFGetNextPowerOfTwo.h); second source: this body (the top-bit index,
//   -1 for 0). No Ego_r.pdb symbol at this body. Was landed as the descriptive free `long __fastcall HighestSetBit(unsigned long)`;
//   the manifest's CEngine::GetScratchZBufferD24S8 is a propagated name.
// This is the out-of-line definition retail keeps; engine/GFGetNextPowerOfTwo.h carries the same body as the inline copy its callers
//   expand, so this TU defines it instead of including that header.
// Shape: `test esi,esi ; jne` + `or eax,-1` (value 0), `cmp esi,80000000h ; jb` + `mov eax,1Fh`, then `inc esi` (the parameter
//   itself incremented) and four steps of `((1 << (r + k)) - n) >> 31 & k` added into edx (the first step's `0x10000 - n` is the
//   folded `1 << 16`); the last step adds into eax.
#include "rebuild_abi.h"
unsigned long FABLE_FASTCALL GFGetTopBit(unsigned long value)
{
    if (value == 0)
        return (unsigned long)-1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}
