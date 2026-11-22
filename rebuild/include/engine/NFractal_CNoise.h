#ifndef FABLE_ENGINE_NFRACTAL_CNOISE_H
#define FABLE_ENGINE_NFRACTAL_CNOISE_H
// NFractal::CNoise (chunk Q9, row 0x00F35C60 PerlinNoise; new header): Ken Perlin's 2D gradient-noise tables.
// Layout: Ego_r.pdb CNoise (struct_layouts_egor.tsv, 6148 bytes): P[512] +0x0000 (long), G2[512] +0x0800 (C2DVector),
// Seed +0x1800; the landed InitPerlinNoise 0x00F35E00 and PerlinNoise address the same offsets ([esi+eax*8+800h]).
// Methods (Ego_r.pdb ?<Name>@CNoise@NFractal@@ ; FableWin.pdb has the same): PerlinNoise QBEMABVC2DVector@@@Z (public const,
// retail 0x00F35C60), InitPerlinNoise AAEXXZ (private, retail 0x00F35E00). The ctors (??0CNoise@NFractal@@QAE@XZ / QAE@J@Z)
// are not declared: no row here reaches them.
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
namespace NFractal {
class CNoise {
public:
#ifdef FABLE_RETAIL_NFRACTAL_CNOISE_CTOR
    // Retail opt-in (agentNF20, 2026-11-11, row 0x00F36BA0): Ego_r.pdb / FableWin.pdb ??0CNoise@NFractal@@QAE@XZ; retail 0x00F36BA0
    // stores Seed = 0 (immediate) and calls InitPerlinNoise 0x00F35E00, plain `ret` (no argument: not the (long) overload ??0..QAE@J@Z).
    CNoise();                                                            // 0x00F36BA0
#endif
    float PerlinNoise(const C2DVector& position) const;                  // 0x00F35C60
private:
#ifdef FABLE_RETAIL_NFRACTAL_CNOISE_ORIGINAL_HELPERS_PASS26
    // Pass26 actual FableWin032c5e10 calls these original const private helpers.
    float SCurve(float t) const { return t*t*(3.0f-2.0f*t); }
    float At2(const C2DVector& point,float rx,float ry) const { return (rx*point.X)+(ry*point.Y); }
    float Lerp(float t,float a,float b) const { return a+t*(b-a); }
#endif
    void InitPerlinNoise();                                              // 0x00F35E00
#ifdef FABLE_RETAIL_NFRACTAL_CNOISE_DONOR_LERP_PASS26
    // FableWin 032C60F0 multiplies the gradient difference by t before adding a.
    float SCurve(float t) const { return t*t*(3.0f-2.0f*t); }
#ifdef FABLE_RETAIL_NFRACTAL_CNOISE_GRADIENT_GROUPING_PASS26
    // Bracket: Grouping the complete gradient sum twice matches 00F35CC7 and 00F35D65. A single sum group or groups around the products miss both windows together.
    float At2(const C2DVector& point, float rx, float ry) const
    {
        return ((rx * point.X + ry * point.Y));
    }
#else
    float At2(const C2DVector& point,float rx,float ry) const { return (rx*point.X)+(ry*point.Y); }
#endif
    float Lerp(float t,float a,float b) const { return ((b-a)*t)+a; }
#endif
public:
    long          P[512];                                                // +0x0000 permutation, upper half a copy
    C2DVector     G2[512];                                               // +0x0800 unit gradients, upper half a copy
    unsigned long Seed;                                                  // +0x1800
};
}
FABLE_STATIC_ASSERT_AT(28, sizeof(NFractal::CNoise) == 0x1804);
#endif
