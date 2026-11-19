// NFractal::CPerlinNoise::GetGaussian2DNoise(const C2DVector& point) const -- 2D value noise: floor the coordinates, hash the four lattice corners through the index table, blend the corner gradients' X components with the 3t^2 - 2t^3 fade
// Leaf: ?GetGaussian2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36540, 321 bytes, thiscall `ret 4`, frame-pointer function.
// Identity: Ego_r.pdb ?GetGaussian2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z (donor order Get2DNoise, Get3DNoise,
//   GetGaussian2DNoise = retail 0x00F36010, 0x00F361D0, 0x00F36540).
// Retail-asm: 0x00F36564 `fistp dword ptr [ebp+8]` and 0x00F36595 `fistp dword ptr [ebp-4]` -- each coordinate copied to a frame
//   word and floored by the engine's inline-assembly GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h: 0.5f / 1.0f frame
//   constants, bare dword fistp, fild/fadd, integer compare, `inc` on memory). VC7.1 emits no bare dword fistp from C++ (per-site
//   C++ / intrinsic attempt table: rebuild/candidates/manual/pending/2026-11-04_linux_hand747/ATTEMPTS_TABLE.md and the helper's
//   own evidence block). This row carries no asm of its own.
// Matched (hand2c 2026-11-19): ported onto engine/CPerlinNoise.h with Ego_d's locals (x_int, y_int, v0_x, v0_y, tx, ty, b00..b11,
//   value00..value11, result) and CPerlinNoise's PDB private inline helpers SCurve / Interp / LookupIndex / LookupScalar (overlay
//   opt-in FABLE_RETAIL_CPERLINNOISE_GETNOISE, shared with Get2DNoise 0x00F36010); their grouping puts the scheduling-region cut
//   where retail pops edi / esi before the two x87 cleanup stores (0x00F36675..0x00F36679). No `#pragma optimize("y", off)`: the
//   helper's asm already gives the ebp frame.
// Data: 3.0f 0x0122DED4; IndexTable (256 unsigned longs) at +0, VectorTable (256 C3DVector) at +0x400.
#define FABLE_RETAIL_CPERLINNOISE_GETNOISE
#include "engine/GFFloatToLongFloor.h"
#include "engine/CPerlinNoise.h"

namespace NFractal {
float CPerlinNoise::GetGaussian2DNoise(const C2DVector& point) const
{
    long x_int = GFFloatToLongFloor(point.X);
    long y_int = GFFloatToLongFloor(point.Y);
    float v0_x = point.X - x_int;
    float v0_y = point.Y - y_int;
    float tx = SCurve(v0_x);
    float ty = SCurve(v0_y);
    unsigned long b00 = LookupIndex(LookupIndex(x_int) + y_int);
    unsigned long b10 = LookupIndex(LookupIndex(x_int + 1) + y_int);
    unsigned long b01 = LookupIndex(LookupIndex(x_int) + y_int + 1);
    unsigned long b11 = LookupIndex(LookupIndex(x_int + 1) + y_int + 1);
    float value00 = LookupScalar(b00);
    float value10 = LookupScalar(b10);
    float value01 = LookupScalar(b01);
    float value11 = LookupScalar(b11);
    float result = Interp(Interp(value00, value10, tx), Interp(value01, value11, tx), ty);
    return result;
}
}