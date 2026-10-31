// _global::GFFloatRandom(float, unsigned long*) -- game-side float random in [0, range): step the LCG seed (x*0x24A1 + 0x24DF), scramble it with GFROR13, and return fmod(seed, range*65536) / 65536 (0 when range*65536 <= 0.0001)
// Grade: RELOCATION_MATCH (113v113). Lever (sec1b): the range is widened into the double local first and scaled by a
//   separate `scaled *= 65536.0` after the LCG step; a single `65536.0 * range` initialiser schedules the fmul early.
//   GFROR13 respelled to the PDB `long` return (?GFROR13@@YIJK@Z, NAME_FIXES 00497890).
// Leaf: GFFloatRandom
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004978A0, 113 bytes, `ret 4` (__fastcall: the seed pointer in ecx -> esi,
//   the float range on the stack; 180 call sites, e.g. the landed CThingAICreature::RandomiseMovementSpeed 0x00830260 and
//   NParticleEngine::CPSCEmitterGeneric::GetRandomRadius 0x00B102C0 declare it the same way).
// Callees: 0x00497890 GFROR13 (landed, __fastcall), 0x00BFEB28 _CIfmod (the `fmod` intrinsic).
// Constants: 0x012387D8 65536.0, 0x012387D0 0.0001, 0x012387C8 2^32 (unsigned -> double fix-up), 0x012387C0
//   1/65536 (1.52587890625e-05), 0x0122DEDC 0.0f.
// Shape: the scaled range is a named double local spilled across the GFROR13 call ([esp+4]); the new seed is stored
//   twice (after the LCG step and after the scramble) and converted as `(double)(unsigned long)` (`fild` + 2^32 add);
//   `if (scaled > 0.0001)` is `test ah,41h ; jne` to the `return 0.0f` tail; `fmod` takes the seed in st(1).
#include <math.h>
#pragma intrinsic(fmod)

long __fastcall GFROR13(unsigned long value);    // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed)
{
    double scaled = (double)range;
    *seed = *seed * 0x24A1 + 0x24DF;
    scaled *= 65536.0;
    *seed = GFROR13(*seed);
    if (scaled > 0.0001)
        return (float)(fmod((double)*seed, scaled) * (1.0 / 65536.0));
    return 0.0f;
}