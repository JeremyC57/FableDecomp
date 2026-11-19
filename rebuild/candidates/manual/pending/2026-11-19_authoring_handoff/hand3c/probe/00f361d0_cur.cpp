// NFractal::CPerlinNoise::Get3DNoise(const C3DVector& point) const -- 3D gradient (Perlin) noise: floor each coordinate, hash the eight lattice corners through the index table, dot each corner's gradient with the offset to the point and blend the eight values trilinearly with the 3t^2 - 2t^3 fade
// Grade: DIFFER(873v873) -- one x87 instruction scheduled late: retail 0x00F3640F `fsub st(1)` (result_z0's outer Interp b-a) precedes gradient001's `lea eax,[esi+1] ; and ; lea ; lea`; ours issues it after them (0x00F3641A). Everything else matches.
// Leaf: ?Get3DNoise@CPerlinNoise@NFractal@@QBEMABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F361D0, 873 bytes, thiscall `ret 4`, frame-pointer function.
// Identity: Ego_r.pdb ?Get3DNoise@CPerlinNoise@NFractal@@QBEMABVC3DVector@@@Z 0x00BC79E0 (donor order Get2DNoise, Get3DNoise,
//   GetGaussian2DNoise = retail 0x00F36010, 0x00F361D0, 0x00F36540); Ego_d.pdb records it with the locals used here.
// Retail-asm: 0x00F361F5 / 0x00F36226 / 0x00F36257 `fistp dword ptr [ebp-...]` -- each coordinate copied to a frame word and floored by
//   the engine's inline-assembly GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h: 0.5f / 1.0f frame constants, bare dword
//   fistp, fild/fadd, integer compare, `inc` on memory). VC7.1 emits no bare dword fistp from C++ (per-site C++ / intrinsic attempt
//   table: rebuild/candidates/manual/pending/2026-11-04_linux_hand747/ATTEMPTS_TABLE.md and the helper's own evidence block). This
//   row carries no asm of its own.
// Data: 1.0f 0x0122DED8, 3.0f 0x0122DED4; IndexTable (256 unsigned longs) at +0, VectorTable (256 C3DVector) at +0x400.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPERLINNOISE_GETNOISE
#include "engine/GFFloatToLongFloor.h"
#include "engine/CPerlinNoise.h"

namespace NFractal {
float CPerlinNoise::Get3DNoise(const C3DVector& point) const
{
    long x_int = GFFloatToLongFloor(point.X);
    long y_int = GFFloatToLongFloor(point.Y);
    long z_int = GFFloatToLongFloor(point.Z);
    float v0_x = point.X - x_int;
    float v0_y = point.Y - y_int;
    float v0_z = point.Z - z_int;
    float v1_x = v0_x - 1.0f;
    float v1_y = v0_y - 1.0f;
    float v1_z = v0_z - 1.0f;
    unsigned long b00 = LookupIndex(LookupIndex(x_int) + y_int);
    unsigned long b10 = LookupIndex(LookupIndex(x_int + 1) + y_int);
    unsigned long b01 = LookupIndex(LookupIndex(x_int) + y_int + 1);
    unsigned long b11 = LookupIndex(LookupIndex(x_int + 1) + y_int + 1);
    float tx = SCurve(v0_x);
    float ty = SCurve(v0_y);
    float tz = SCurve(v0_z);
    C3DVector gradient000 = LookupGradient(b00 + z_int);
    C3DVector gradient100 = LookupGradient(b10 + z_int);
    C3DVector gradient010 = LookupGradient(b01 + z_int);
    C3DVector gradient110 = LookupGradient(b11 + z_int);
    float value000 = gradient000.Dot(C3DVector(v0_x, v0_y, v0_z));
    float value100 = gradient100.Dot(C3DVector(v1_x, v0_y, v0_z));
    float value010 = gradient010.Dot(C3DVector(v0_x, v1_y, v0_z));
    float value110 = gradient110.Dot(C3DVector(v1_x, v1_y, v0_z));
    float result_z0 = Interp(Interp(value000, value100, tx), Interp(value010, value110, tx), ty);
    C3DVector gradient001 = LookupGradient(b00 + z_int + 1);
    C3DVector gradient101 = LookupGradient(b10 + z_int + 1);
    C3DVector gradient011 = LookupGradient(b01 + z_int + 1);
    C3DVector gradient111 = LookupGradient(b11 + z_int + 1);
    float value001 = gradient001.Dot(C3DVector(v0_x, v0_y, v1_z));
    float value101 = gradient101.Dot(C3DVector(v1_x, v0_y, v1_z));
    float value011 = gradient011.Dot(C3DVector(v0_x, v1_y, v1_z));
    float value111 = gradient111.Dot(C3DVector(v1_x, v1_y, v1_z));
    float result_z1 = Interp(Interp(value001, value101, tx), Interp(value011, value111, tx), ty);
    float result = Interp(result_z0, result_z1, tz);
    return result;
}
}
