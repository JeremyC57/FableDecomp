// C2DVector::GetRandomUnitVector(unsigned long* seed) -- a unit vector at a random angle: (cos, sin) of a random turn fraction from the cosine table
// Grade: RELOCATION_MATCH (3 grades: cos evaluated first via a C2DVector result local with X/Y assigned in order; the dead local keeps retail's extra frame word) -- rasm1 retail_asm: C++ body + a minimal 2-instruction inline-asm (no control flow) + the Retail-asm evidence line
// Retail-asm: two inlined copies of the engine's inline-assembly float-to-long helper (GFFastCos 0x00A14840 and GFFastSin 0x00A147E0 expanded in place, each storing angle*1024 - 0.5 to its own by-value copy, reloading it and rounding with a bare `fistp dword`); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's inline-asm.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongNear.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Leaf: ?GetRandomUnitVector@C2DVector@@SI?AV1@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A13EB0, 191 bytes, plain `ret` (static __fastcall: hidden result pointer in ecx,
//   seed in edx). Caller 0x00B110BD. PDB name (Ego_r and FableWin) ?GetRandomUnitVector@C2DVector@@SI?AV1@PAK@Z.
// Callees: GFFloatRandom 0x004978A0 (`?GFFloatRandom@@YIMMPAK@Z`, seed in ecx, max 1.0f pushed); GFFastCos 0x00A14840
//   and GFFastSin 0x00A147E0 inlined (cosine table 0x013CD550).
// Shape: the random turn fraction is scaled by 1024 once (shared by both inlines), X = cos lookup (no shift), Y = sin
//   lookup (-256), X kept in ecx until the end, Y stored straight from st(0).
#include "fable_compiler.h"
extern float GFastCosTable[1025];      // 0x013CD550 ?GFastCosTable@@3PAMA (cos(2*pi*k/1024), k = 0..1024)

float __fastcall GFFloatRandom(float max, unsigned long* seed);    // 0x004978A0

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
#include "engine/GFFloatToLongNear.h"

FABLE_FORCEINLINE float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1];
}

FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1];
}

class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
    static C2DVector __fastcall GetRandomUnitVector(unsigned long* seed);
};

C2DVector __fastcall C2DVector::GetRandomUnitVector(unsigned long* seed)
{
    float angle = GFFloatRandom(1.0f, seed);
    C2DVector result;
    result.X = GFFastCos(angle);
    result.Y = GFFastSin(angle);
    return result;
}