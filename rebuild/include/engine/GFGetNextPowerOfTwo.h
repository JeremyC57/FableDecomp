#ifndef FABLE_ENGINE_GFGETNEXTPOWEROFTWO_H
#define FABLE_ENGINE_GFGETNEXTPOWEROFTWO_H
#include "rebuild_abi.h"
#include "fable_compiler.h"
// New header (chunk 1014, row 0x009FA620 CTexture::GetNextValidDimensions): FableWin.pdb ?GFGetTopBit@@YIKK@Z (FableWin 0x02C87570;
// retail out of line at 0x0099FDB0, the landed body: -1 for 0, 31 for a set top bit, else a branch-free binary search over value + 1)
// and ?GFGetNextPowerOfTwo@@YIKK@Z (FableWin 0x02D90380: 0 for 0, else 1 << GFGetTopBit(2 * value - 1)). Both __fastcall, inline in
// the release build where the body is visible (GetNextValidDimensions expands both twice). Plain `inline`: the row reaches retail's
// expansion with /d2inlT180 (owner ladder: an inline-budget flag before FABLE_FORCEINLINE).
inline unsigned long FABLE_FASTCALL GFGetTopBit(unsigned long value)
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
inline unsigned long FABLE_FASTCALL GFGetNextPowerOfTwo(unsigned long value)
{
    if (value == 0)
        return 0;
    return 1 << GFGetTopBit(value * 2 - 1);
}
#endif
