#ifndef FABLE_ENGINE_CTVERTEXCOLTEX2_H
#define FABLE_ENGINE_CTVERTEXCOLTEX2_H
// New header (wfE5; no repo header for the class): the two-texture-coordinate coloured vertex, Ego_r.pdb CTVertexColTex2Base
// (struct_layouts_egor.tsv, 0x20 bytes: X, Y, Z, DiffuseColour, U1, V1, U2, V2), flat like the repo's CTVertexColTex1.h.
// Users: CEngineSubPrimitive2DSprite::Vertices[4] (+0x18), CEngineSubPrimitive2DClockSprite::Vertices[7] (+0x18); stride 0x20 in
// DrawAsIndexedTris (0x00BAF01C / 0x00BAF957).
#include "rebuild_abi.h"
class CTVertexColTex2 {
public:
    float         X;                            // +0x00
    float         Y;                            // +0x04
    float         Z;                            // +0x08
    unsigned long DiffuseColour;                // +0x0c
    float         U1;                           // +0x10
    float         V1;                           // +0x14
    float         U2;                           // +0x18
    float         V2;                           // +0x1c
};
FABLE_STATIC_ASSERT(sizeof(CTVertexColTex2) == 0x20);
#endif // FABLE_ENGINE_CTVERTEXCOLTEX2_H
