// NFractal::CPerlinNoise::Get2DNoise(const C2DVector& point) const -- 2D gradient (Perlin) noise: floor the coordinates, hash the four lattice corners through the index table, dot each corner's gradient (X, Y of the table vector) with the offset to the point and blend with the 3t^2 - 2t^3 fade
// Leaf: ?Get2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36010, 440 bytes, thiscall `ret 4`, frame-pointer function.
// Identity: Ego_r.pdb ?Get2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z 0x00BC77B0 (donor order Get2DNoise, Get3DNoise,
//   GetGaussian2DNoise = retail 0x00F36010, 0x00F361D0, 0x00F36540); Ego_d.pdb records it with the locals used here. The catalog
//   label "CPerlinNoise::GetGaussian2DNoise" was wrong (NAME_FIXES.tsv).
// Retail-asm: 0x00F36034 `fistp dword ptr [ebp-4]` and 0x00F36065 `fistp dword ptr [ebp-8]` -- each coordinate copied to a frame
//   word and floored by the engine's inline-assembly GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h: 0.5f / 1.0f frame
//   constants, bare dword fistp, fild/fadd, integer compare, `inc` on memory). VC7.1 emits no bare dword fistp from C++ (per-site
//   C++ / intrinsic attempt table: rebuild/candidates/manual/pending/2026-11-04_linux_hand747/ATTEMPTS_TABLE.md and the helper's
//   own evidence block). This row carries no asm of its own.
// Matched (hand2c 2026-11-19): Ego_d's locals and CPerlinNoise's PDB private inline helpers (SCurve, Interp, LookupIndex,
//   LookupGradient by value -- the 12-byte frame temporaries 0x00F3615C..0x00F36162 -- in the overlay opt-in
//   FABLE_RETAIL_CPERLINNOISE_GETNOISE); hash order b00, b10, b01, b11 (0x00F36095 / 0x00F360CB), gradient dot products through
//   C2DVector::Dot.
// Data: 1.0f 0x0122DED8, 3.0f 0x0122DED4; IndexTable (256 unsigned longs) at +0, VectorTable (256 C3DVector) at +0x400.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_CPERLINNOISE_GETNOISE
#include "engine/GFFloatToLongFloor.h"
#include "engine/CPerlinNoise.h"

namespace NFractal {
float CPerlinNoise::Get2DNoise(const C2DVector& point) const
{
    long x_int = GFFloatToLongFloor(point.X);
    long y_int = GFFloatToLongFloor(point.Y);
    float v0_x = point.X - x_int;
    float v0_y = point.Y - y_int;
    float v1_x = v0_x - 1.0f;
    float v1_y = v0_y - 1.0f;
    float tx = SCurve(v0_x);
    float ty = SCurve(v0_y);
    unsigned long b00 = LookupIndex(LookupIndex(x_int) + y_int);
    unsigned long b10 = LookupIndex(LookupIndex(x_int + 1) + y_int);
    unsigned long b01 = LookupIndex(LookupIndex(x_int) + y_int + 1);
    unsigned long b11 = LookupIndex(LookupIndex(x_int + 1) + y_int + 1);
    C2DVector gradient00 = LookupGradient(b00);
    C2DVector gradient10 = LookupGradient(b10);
    C2DVector gradient01 = LookupGradient(b01);
    C2DVector gradient11 = LookupGradient(b11);
    float value00 = gradient00.Dot(C2DVector(v0_x, v0_y));
    float value10 = gradient10.Dot(C2DVector(v1_x, v0_y));
    float value01 = gradient01.Dot(C2DVector(v0_x, v1_y));
    float value11 = gradient11.Dot(C2DVector(v1_x, v1_y));
    float result = Interp(Interp(value00, value10, tx), Interp(value01, value11, tx), ty);
    return result;
}
}