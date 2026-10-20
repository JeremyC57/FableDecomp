// CPerlinNoise::Initialise(unsigned long seed) -- build the noise tables from a seed: the identity permutation shuffled with the engine LCG (seed*0x24A1 + 0x24DF, rotated right 13), then 256 random gradient vectors with components in [-1, 1)
// Grade: DIFFER(307v307) -- scheduling only: our `fmul qword [1/65536]` follows the fmod call directly, retail computes the next LCG argument (`mov ecx,esi ; imul ; add`) first (x2 per iteration)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F35ED0, 307 bytes, `ret 4`.
// Callees: GFROR13(unsigned long) 0x00497890 (`__fastcall`, rotate right by 13), the fmod intrinsic `_CIfmod` 0x00BFEB28.
// Constants: 0x012387C8 = 2^32 (double, the unsigned fild fix-up), 0x0129CA60 = 131072.0, 0x012387C0 = 1/65536 (double),
//   0x0122DED8 = 1.0f.
// Layout: Ego_r.pdb CPerlinNoise (0x1000): IndexTable[256] (unsigned long) +0, VectorTable[256] (C3DVector) +0x400.
// Shape: the seed parameter is the running LCG state (its slot is the `fild` source); each component is
//   `(float)(fmod((double)seed, 131072.0) * (1.0 / 65536.0)) - 1.0f` (the float cast before
//   the subtraction gives retail's `fsub dword [1.0f]`); a per-iteration `C3DVector& v` reference gives the +0x408 IV base; the swap reads the i-th entry first.
#include <math.h>
#pragma intrinsic(fmod)

long __fastcall GFROR13(unsigned long value);           // 0x00497890

struct C3DVector { float X, Y, Z; };

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
        seed = GFROR13(seed * 0x24A1 + 0x24DF);
        v.X = (float)(fmod((double)seed, 131072.0) * (1.0 / 65536.0)) - 1.0f;
        seed = GFROR13(seed * 0x24A1 + 0x24DF);
        v.Y = (float)(fmod((double)seed, 131072.0) * (1.0 / 65536.0)) - 1.0f;
        seed = GFROR13(seed * 0x24A1 + 0x24DF);
        v.Z = (float)(fmod((double)seed, 131072.0) * (1.0 / 65536.0)) - 1.0f;
    }
}
