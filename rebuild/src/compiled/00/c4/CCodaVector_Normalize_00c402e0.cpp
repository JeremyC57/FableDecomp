// CCodaVector::Normalize() -- return 0 for the zero vector; otherwise compute the length and, unless it is already 1, scale the three components by its reciprocal; returns the length
// Leaf: ?Normalize@CCodaVector@@QAEMXZ
// NF95 (part 2, q11383): was the descriptive stand-in CPKVector3_00c402e0::Normalise; the vendor objects name it
//   ?Normalize@CCodaVector@@QAEMXZ (CODAApi.lib + CODAApiDbg.lib CodaListener.obj relocations), declared by engine/CCodaVector.h
//   (FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT, landed by 0x00C40590 CCodaListener::Commit).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C402E0, 138 bytes (+ int3 padding), __fastcall, float in st(0).
// Library: GameCODA 2.10 (CODAApi.lib .\Release\CodaListener.obj, byte-identical; CODAApiDbg.lib same names). Float constants: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
//   Callers 0x00C405BD / 0x00C405C9 (listener orientation, result dropped), 0x00C41982 (NormaliseCopy), 0x00C42053,
//   0x00C4222E. The manifest's "Vector3_Normalize" is close.
// Shape: the squared sum is written z*z + y*y + x*x (retail x87 order: z, y, x loaded, z*z first); the unit test is
//   `1.0f != length` (`fld st(0) ; fld [one] ; fucompp`; `length != 1.0f` loads the constant first).
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double value);
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#include "engine/CCodaVector.h"

float CCodaVector::Normalize()
{
    if (x == 0.0f && y == 0.0f && z == 0.0f)
        return 0.0f;
    float length = (float)sqrt(z * z + y * y + x * x);
    if (1.0f != length) {
        float scale = 1.0f / length;
        x *= scale;
        y *= scale;
        z *= scale;
    }
    return length;
}
