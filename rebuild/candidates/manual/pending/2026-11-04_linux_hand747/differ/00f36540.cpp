// NFractal::CPerlinNoise::GetGaussian2DNoise(const C2DVector& pos) const -- 2D value noise: floor the coordinates, hash the four lattice corners through the 256-entry permutation table, blend the corner scalars with the 3t^2 - 2t^3 weight
// Grade: DIFFER(321v321) -- best f dl=4 (321v321): only the epilogue differs -- retail pops edi/esi before the two x87 cleanup fstp st(2)/st(0) (best try l; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?GetGaussian2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z
// Flags: none
// Retail-asm: 0x00F36568 `fistp dword ptr [ebp+8]` and 0x00F36599 `fistp dword ptr [ebp-4]` -- each coordinate copied to the helper's
//   frame word and floored by the engine's inline-assembly GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h: 0.5f / 1.0f frame
//   constants, bare dword fistp, fild/fadd, integer compare, `inc` on memory). VC7.1 emits no bare dword fistp from C++ (NOTES.md
//   "C++ attempts before helper"). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36540, 321 bytes, thiscall `ret 4`, frame-pointer function.
// Identity: Ego_r.pdb ?GetGaussian2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z (donor order Get2DNoise, Get3DNoise,
//   GetGaussian2DNoise = retail 0x00F36010, 0x00F361D0, 0x00F36540).
// Data: 3.0f 0x0122DED4, the permutation table at this+0 (256 longs) and the gradients at +0x400 (12-byte records).
#pragma optimize("y", off)
#include "engine/GFFloatToLongFloor.h"
class C2DVector { public: float X, Y; };
namespace NFractal {
class CPerlinNoise {
public:
    long  Permutation[256];                             // +0x000
    float Gradients[256][3];                            // +0x400
    float GetGaussian2DNoise(const C2DVector& pos) const;
private:
    unsigned long LookupIndex(unsigned long index) const { return Permutation[index & 0xff]; }
    float LookupScalar(unsigned long index) const { return Gradients[LookupIndex(index) & 0xff][0]; }
    float Interp(float a, float b, float t) const { return a + (b - a) * t; }
};
float CPerlinNoise::GetGaussian2DNoise(const C2DVector& pos) const
{
    long ix = GFFloatToLongFloor(pos.X);
    long iy = GFFloatToLongFloor(pos.Y);
    float fx = pos.X - (float)ix;
    float fy = pos.Y - (float)iy;
    float sx = (3.0f - (fx + fx)) * fx * fx;
    float g00 = LookupScalar(LookupIndex(ix) + iy);
    float g01 = LookupScalar(LookupIndex(ix) + iy + 1);
    float a = Interp(g00, LookupScalar(LookupIndex(ix + 1) + iy), sx);
    float sy = (3.0f - (fy + fy)) * fy * fy;
    float b = Interp(g01, LookupScalar(LookupIndex(ix + 1) + iy + 1), sx);
    return Interp(a, b, sy);
}
}
