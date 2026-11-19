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
// Data: 3.0f 0x0122DED4; IndexTable (256 unsigned longs) at +0, VectorTable (256 C3DVector) at +0x400.
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/CPerlinNoise.h"

namespace NFractal {
float CPerlinNoise::GetGaussian2DNoise(const C2DVector& point) const
{
    long x_int = GFFloatToLongFloor(point.X);
    long y_int = GFFloatToLongFloor(point.Y);
    float v0_x = point.X - x_int;
    float v0_y = point.Y - y_int;
    float v1_x = v0_x - 1.0f;
    float v1_y = v0_y - 1.0f;
    float tx = (3.0f - 2.0f * v0_x) * v0_x * v0_x;
    float ty = (3.0f - 2.0f * v0_y) * v0_y * v0_y;
    unsigned long b00 = IndexTable[(IndexTable[x_int & 0xff] + y_int) & 0xff];
    unsigned long b01 = IndexTable[(IndexTable[x_int & 0xff] + y_int + 1) & 0xff];
    unsigned long b10 = IndexTable[(IndexTable[(x_int + 1) & 0xff] + y_int) & 0xff];
    unsigned long b11 = IndexTable[(IndexTable[(x_int + 1) & 0xff] + y_int + 1) & 0xff];
    float value00 = VectorTable[b00 & 0xff].X;
    float value01 = VectorTable[b01 & 0xff].X;
    float value10 = VectorTable[b10 & 0xff].X;
    float value11 = VectorTable[b11 & 0xff].X;
    float result = GFGetInterpolatedValue(GFGetInterpolatedValue(value00, value10, tx), GFGetInterpolatedValue(value01, value11, tx), ty);
    return result;
}
}
