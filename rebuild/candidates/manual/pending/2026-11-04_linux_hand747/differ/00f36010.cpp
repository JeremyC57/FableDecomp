// NFractal::CPerlinNoise::Get2DNoise(const C2DVector& pos) const -- 2D gradient (Perlin) noise: floor both coordinates, hash the four lattice corners through the permutation table, blend the four gradient dot products with the 3t^2 - 2t^3 fade
// Grade: DIFFER(473v442) -- try a (rewritten with Ego_r LookupIndex / LookupGradient / Interp) 473v442 frame-masked dl=71: gradient copies (retail copies only X/Y through integer registers) and the dot-product x87 schedule; the old draft's GetGaussian2DNoise label belongs to 0x00F36540 (best try a; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?Get2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z
// Flags: none
// Retail-asm: 0x00F36034 `fistp dword ptr [ebp-4]` and 0x00F36065 `fistp dword ptr [ebp-8]` -- each coordinate copied to the helper's
//   frame word and floored by the engine's inline-assembly GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h). VC7.1 emits no bare
//   dword fistp from C++ (NOTES.md "C++ attempts before helper"). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36010, 442 bytes (padded to 448), thiscall `ret 4`, frame-pointer function.
// Identity: Ego_r.pdb ?Get2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z (donor order Get2DNoise, Get3DNoise, GetGaussian2DNoise =
//   retail 0x00F36010, 0x00F361D0, 0x00F36540; the old draft's "GetGaussian2DNoise" label was the neighbour's). Ego_r's private inline
//   helpers LookupIndex / LookupGradient (C3DVector by value) / Interp.
#include "engine/GFFloatToLongFloor.h"
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
namespace NFractal {
class CPerlinNoise {
public:
    long      Permutation[256];                     // +0x000
    C3DVector Gradients[256];                       // +0x400
    float Get2DNoise(const C2DVector& pos) const;
private:
    unsigned long LookupIndex(unsigned long index) const { return Permutation[index & 0xff]; }
    C3DVector LookupGradient(unsigned long index) const { return Gradients[LookupIndex(index) & 0xff]; }
    float Interp(float a, float b, float t) const { return a + (b - a) * t; }
};
float CPerlinNoise::Get2DNoise(const C2DVector& pos) const
{
    long ix = GFFloatToLongFloor(pos.X);
    long iy = GFFloatToLongFloor(pos.Y);
    float fx = pos.X - (float)ix;
    float fy = pos.Y - (float)iy;
    float fx1 = fx - 1.0f;
    float fy1 = fy - 1.0f;
    float sx = (3.0f - (fx + fx)) * fx * fx;
    C3DVector g00 = LookupGradient(LookupIndex(ix) + iy);
    C3DVector g10 = LookupGradient(LookupIndex(ix + 1) + iy);
    C3DVector g01 = LookupGradient(LookupIndex(ix) + iy + 1);
    C3DVector g11 = LookupGradient(LookupIndex(ix + 1) + iy + 1);
    float a = Interp(g00.X * fx + g00.Y * fy, g10.X * fx1 + g10.Y * fy, sx);
    float b = Interp(g01.X * fx + g01.Y * fy1, g11.X * fx1 + g11.Y * fy1, sx);
    return Interp(a, b, (3.0f - (fy + fy)) * fy * fy);
}
}
