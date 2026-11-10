#ifndef FABLE_ENGINE_GFRANDOM_H
#define FABLE_ENGINE_GFRANDOM_H
#include "rebuild_abi.h"
// New header (chunk 1014, row 0x00929FC0): Ego_r.pdb ?GFROR13@@YIJK@Z (out of line, retail 0x00497890: rotate right by 13) and
// ?GFRandom@@YIKKPAK@Z (Ego_r 0x004183C0; inline in the retail rows that use it: the seed stepped by *0x24A1 + 0x24DF, rotated, then
// the remainder by the range, 0 for a zero range -- the body the landed 0x005472A0 GFRandomNoRepeat and 0x007FEDE0 expand).
long FABLE_FASTCALL GFROR13(unsigned long value);                    // 0x00497890
inline unsigned long FABLE_FASTCALL GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
#endif
