#ifndef FABLE_ENGINE_GFGETNEXTPOWEROFTWO_H
#define FABLE_ENGINE_GFGETNEXTPOWEROFTWO_H
#include "rebuild_abi.h"
#include "fable_compiler.h"
// New header (chunk 1014, row 0x009FA620 CTexture::GetNextValidDimensions): FableWin.pdb ?GFGetTopBit@@YIKK@Z (FableWin 0x02C87570;
// retail out of line at 0x0099FDB0, the landed body: -1 for 0, 31 for a set top bit, else a branch-free binary search over value + 1)
// and ?GFGetNextPowerOfTwo@@YIKK@Z (FableWin 0x02D90380: 0 for 0, else 1 << GFGetTopBit(2 * value - 1)). Both __fastcall, inline in
// the release build where the body is visible (GetNextValidDimensions expands both twice). Plain `inline`: the row reaches retail's
// expansion with /d2inlT180 (owner ladder: an inline-budget flag before FABLE_FORCEINLINE).
#ifdef FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE
// Retail opt-in (chunk 2018, rows 0x00B2CB40 / 0x00B2D330): FABLE_FORCEINLINE, the owner's 2026-11-01 rule for Lionhead header helpers --
// FableWin.pdb-only name (no Ego_r.pdb copy), and retail expands every use: 17 expansions of the body in Fable.exe (`mov eax,10000h ;
// sub eax,r ; sar eax,1Fh ; and eax,10h`, e.g. 0x00B2CF9F, 0x00B2D56E, 0x009FA6xx) and no call to the uncalled COMDAT 0x0099FDB0.
// Plain `inline` leaves the variable-argument sites called (c1 120: score 125 at f 0.75 in UpdateDiskCacheTexture, 81 at f 25 in the
// constructor's loop, against the default threshold 74).
FABLE_FORCEINLINE
#else
inline
#endif
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
inline unsigned long FABLE_FASTCALL GFGetNextPowerOfTwo(unsigned long value)
{
    if (value == 0)
        return 0;
    return 1 << GFGetTopBit(value * 2 - 1);
}
#endif
