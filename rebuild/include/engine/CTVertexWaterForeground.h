#ifndef FABLE_ENGINE_CTVERTEXWATERFOREGROUND_H
#define FABLE_ENGINE_CTVERTEXWATERFOREGROUND_H
// New (chunk 779, row 0x00BEAE50 CWaterPatchMesh::Load): Ego_r.pdb CTVertexWaterForegroundBase (0x44 bytes: X +0, Y +2 ushort, Z +4,
// SwellData0 +8, SwellData1 +0xA ushort, Depth +0xC, ShoreUCoord +0x10, ShoreLookUpArray[12] +0x14) and CTVertexWaterForeground deriving
// from it with no members (PDB class key V, ghidra_out/pdb_class_keys.tsv); retail's water patch vertex buffer has a 0x44 stride and
// Load fills the fields in this order (0x00BEB0E0..0x00BEB40E). Retail layout.
#include <stddef.h>
#include "rebuild_abi.h"
class CTVertexWaterForegroundBase {
public:
    unsigned short X;                                                    // +0x00
    unsigned short Y;                                                    // +0x02
    float          Z;                                                    // +0x04
    unsigned short SwellData0;                                           // +0x08
    unsigned short SwellData1;                                           // +0x0a
    float          Depth;                                                // +0x0c
    float          ShoreUCoord;                                          // +0x10
    float          ShoreLookUpArray[12];                                 // +0x14
};
class CTVertexWaterForeground : public CTVertexWaterForegroundBase {
};
FABLE_STATIC_ASSERT(sizeof(CTVertexWaterForeground) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTVertexWaterForegroundBase, Depth) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTVertexWaterForegroundBase, ShoreLookUpArray) == 0x14);
#endif // FABLE_ENGINE_CTVERTEXWATERFOREGROUND_H
