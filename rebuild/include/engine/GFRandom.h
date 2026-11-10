#ifndef FABLE_ENGINE_GFRANDOM_H
#define FABLE_ENGINE_GFRANDOM_H
#include "rebuild_abi.h"
#include "fable_compiler.h"
// Ego_r.pdb ?GFROR13@@YIJK@Z (out of line, retail 0x00497890: rotate right by 13) and ?GFRandom@@YIKKPAK@Z (Ego_r 0x004183C0; inline
// in the retail rows that use it: the seed stepped by *0x24A1 + 0x24DF, rotated, then the remainder by the range, 0 for a zero range --
// the body the landed 0x005472A0 GFRandomNoRepeat and 0x007FEDE0 expand). Created the same day by chunk 1014 (0x00929FC0 draft) and
// Codex pass 18; this is Codex's statement order, the spelling that matched 0x008E7F80.
long FABLE_FASTCALL GFROR13(unsigned long value);                    // 0x00497890
inline unsigned long FABLE_FASTCALL GFRandom(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1;
    *seed += 0x24df;
    *seed = GFROR13(*seed);
    if (range)
        return *seed % range;
    return 0;
}
#endif
