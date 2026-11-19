// sal_levelToGain(int level) -- GameCODA / libpk: a level in millibels to linear gain: 0 at or below -10000 mB (silence), else 10^(mB / 2000) computed as 2^(mB * 0.0005 * log2 10) through the x87 base-2 instructions
// Leaf: ?sal_levelToGain@@YAMH@Z
// Retail-asm: 0x00C54B15..0x00C54B44 `fld [esp+4] ; fmul [esp] ; frndint ; fst [esp+8] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+4] ; fmul [esp] ; fsub [esp+8] ; f2xm1 ; fadd [esp+18h] ; fmulp st(1),st ; fstp [esp+0Ch] ; wait` -- libpk's inline-assembly 2^x block (the landed retail_asm twin MillibelsToAmplitude_00c40ee0 0x00C40EE0, byte-identical); frndint, fscale and f2xm1 have no VC7.1 intrinsic form and the `wait`s are asm-only; the prologue test, the exponent and the two constants are C++.
// Helper (2026-11-05 asm migration): the 17-instruction x87 2^x block is the shared libpk/Pow10.h (LIBPK_POW10_BLOCK; byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family POW10.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c54ae0, 109 bytes, __cdecl, float in st(0).
// Identity: GameCODA 2.10 CODAApi.lib SalSw3DVoice.obj -- CSalSw3DVoice::CalcParams' call relocations (+0xD6, +0x1AA) name
//   ?sal_levelToGain@@YAMH@Z and that release COMDAT is byte-identical (109 bytes, 2 data relocations); retail places it in
//   the SalSw3DVoice TU right before CalcParams 0x00C54C10, its only caller (twice), which binds this spelling. The int
//   parameter is the library's (H); the parameter name is descriptive. A byte-identical copy of the landed 0x00C40EE0
//   (another TU). Was the descriptive MillibelsToAmplitude_00c54ae0(long). Constants: 0x01264AF0 = 0.0005f,
//   0x0122DEDC = 0.0f, the immediate 0x40549A78 = log2(10).
#include "libpk/Pow10.h"
float __cdecl sal_levelToGain(int level)
{
    if (level <= -10000)
        return 0.0f;
    float scale;
    float result;
    float whole;
    float log2of10 = 3.32192802f;                  // 0x40549A78
    float exponent = level * 0.0005f;
    float one = 1.0f;
    LIBPK_POW10_BLOCK(log2of10, exponent, whole, scale, one, result);
    return result;
}