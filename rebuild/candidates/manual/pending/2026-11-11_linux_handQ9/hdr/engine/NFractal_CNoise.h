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
    float PerlinNoise(const C2DVector& position) const;                  // 0x00F35C60
private:
    void InitPerlinNoise();                                              // 0x00F35E00
public:
    long          P[512];                                                // +0x0000 permutation, upper half a copy
    C2DVector     G2[512];                                               // +0x0800 unit gradients, upper half a copy
    unsigned long Seed;                                                  // +0x1800
};
}
FABLE_STATIC_ASSERT(sizeof(NFractal::CNoise) == 0x1804);
#endif
