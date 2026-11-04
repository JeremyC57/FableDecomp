// CPerlinNoise::Initialise(unsigned long seed) -- build the noise tables from a seed: the identity permutation shuffled with the engine LCG (seed*0x24A1 + 0x24DF, rotated right 13), then 256 random gradient vectors with components GFFloatRandom(2, seed) - 1 in [-1, 1)
// Leaf: ?Initialise@CPerlinNoise@@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F35ED0, 307 bytes, `ret 4`. Ego_r.pdb CPerlinNoise::Initialise(unsigned long).
// Callees: GFROR13(unsigned long) 0x00497890 (`__fastcall`, Ego_r ?GFROR13@@YIJK@Z, landed), the fmod intrinsic `_CIfmod` 0x00BFEB28.
// Constants: 0x012387C8 = 2^32 (double, the unsigned fild fix-up), 0x0129CA60 = 131072.0, 0x012387C0 = 1/65536 (double),
//   0x0122DED8 = 1.0f.
// Layout: Ego_r.pdb CPerlinNoise (0x1000): IndexTable[256] (unsigned long) +0, VectorTable[256] (C3DVector) +0x400.
// Shape (what matched): each component is Ego_r's GFFloatRandom(2.0f, &seed) - 1.0f (?GFFloatRandom@@YIMMPAK@Z, out of line at
//   0x004978A0, landed), inlined here with the landed body and agent635's named `double result = fmod(...)` local (the same body
//   that matched at 0x00A146C0): VC7.1 then computes the next LCG argument (`mov ecx,esi ; imul ; add`) before the
//   `fmul [1/65536]`, as retail; the one-expression fmod * (1/65536) puts the fmul first. The `> 0.0001` range test folds away
//   for the constant 2.0f. The seed parameter is the running LCG state (its slot is the `fild` source); a per-iteration
//   `C3DVector& v` reference gives the +0x408 IV base; the shuffle reads the i-th entry first. (Spelling the shuffle step as
//   the inline GFRandom(256, &seed) 0x004CDAE0 swaps the table/index registers ebx/edi, so it is written out.)
#include <math.h>
#pragma intrinsic(fmod)

long __fastcall GFROR13(unsigned long value);           // 0x00497890
inline float __fastcall GFFloatRandom(float range, unsigned long* seed)   // out of line at 0x004978A0
{
    double scaled = (double)range;
    *seed = *seed * 0x24A1 + 0x24DF;
    scaled *= 65536.0;
    *seed = GFROR13(*seed);
    if (scaled > 0.0001) {
        double result = fmod((double)*seed, scaled);
        return (float)(result * (1.0 / 65536.0));
    }
    return 0.0f;
}

#include "engine/C3DVector.h"

class CPerlinNoise {
public:
    unsigned long IndexTable[256];                      // +0x000
    C3DVector     VectorTable[256];                     // +0x400
    void Initialise(unsigned long seed);
};

void CPerlinNoise::Initialise(unsigned long seed)
{
    unsigned long i;
    for (i = 0; i < 256; ++i)
        IndexTable[i] = i;
    for (i = 0; i < 256; ++i) {
        seed = GFROR13(seed * 0x24A1 + 0x24DF);
        unsigned long swap = IndexTable[i];
        IndexTable[i] = IndexTable[seed & 0xFF];
        IndexTable[seed & 0xFF] = swap;
    }
    for (i = 0; i < 256; ++i) {
        C3DVector& v = VectorTable[i];
        v.X = GFFloatRandom(2.0f, &seed) - 1.0f;
        v.Y = GFFloatRandom(2.0f, &seed) - 1.0f;
        v.Z = GFFloatRandom(2.0f, &seed) - 1.0f;
    }
}