#ifndef FABLE_ENGINE_CTVERTEXRHWCOLTEX1_H
#define FABLE_ENGINE_CTVERTEXRHWCOLTEX1_H
// New header (chunk 783): the 0x1C-byte pre-transformed (RHW) vertex with a diffuse colour and one UV set. Ego_r.pdb
// CTVertexRHWColTex1Base (ghidra_out/struct_layouts_egor.tsv, 28 bytes): X +0x00, Y +0x04, Z +0x08, RHW +0x0C, DiffuseColour +0x10,
// U1 +0x14, V1 +0x18; the CTVertexRHWColTex1 wrapper (CGenericVertex<...>) adds no data. Retail 0x009FA780 fills four of them with a
// 0x1C stride (DrawAsIndexedTris stride 1Ch) and never writes DiffuseColour.
#include "rebuild_abi.h"
class CTVertexRHWColTex1 {
public:
    float         X;                            // +0x00
    float         Y;                            // +0x04
    float         Z;                            // +0x08
    float         RHW;                          // +0x0c
    unsigned long DiffuseColour;                // +0x10
    float         U1;                           // +0x14
    float         V1;                           // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CTVertexRHWColTex1) == 0x1c);
#endif // FABLE_ENGINE_CTVERTEXRHWCOLTEX1_H
