#ifndef FABLE_ENGINE_CTVERTEXCOLTEX1_H
#define FABLE_ENGINE_CTVERTEXCOLTEX1_H
// New header (chunk 782): the 0x18-byte position + diffuse + one-UV vertex the screen-effect renderers fill (the landed
// CEngineScreenEffectDisplacementRenderer ctor 0x00B86C00 model's member names; retail stores X +0x0, Y +0x4, Z +0x8, the
// colour +0xC, U1 +0x10, V1 +0x14 in 0x00B5F090 / 0x00B86C00). No Ego_r.pdb layout row is in struct_layouts_egor.tsv.
#include "rebuild_abi.h"
class CTVertexColTex1 {
public:
    float         X;                            // +0x00
    float         Y;                            // +0x04
    float         Z;                            // +0x08
    unsigned long DiffuseColour;                // +0x0c
    float         U1;                           // +0x10
    float         V1;                           // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CTVertexColTex1) == 0x18);
#endif // FABLE_ENGINE_CTVERTEXCOLTEX1_H
