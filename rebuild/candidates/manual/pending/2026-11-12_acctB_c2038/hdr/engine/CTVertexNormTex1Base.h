#ifndef FABLE_ENGINE_CTVERTEXNORMTEX1BASE_H
#define FABLE_ENGINE_CTVERTEXNORMTEX1BASE_H
// New header (chunk 2038): Ego_r.pdb CTVertexNormTex1Base (struct_layouts_egor.tsv: float X +0, Y +4, Z +8, NormalX +0xC, NormalY +0x10,
// NormalZ +0x14, U1 +0x18, V1 +0x1C; sizeof 0x20), class key V (pdb_class_keys / the FableWin CGenericVertex<CTVertexNormTex1Base>
// instantiation). Retail 0x00BB8C4E..0x00BB8CA4 (RenderLandscapeDecalGroup) stores the eight fields at these offsets.
#include "rebuild_abi.h"
class CTVertexNormTex1Base {
public:
    float X;                                    // +0x00
    float Y;                                    // +0x04
    float Z;                                    // +0x08
    float NormalX;                              // +0x0c
    float NormalY;                              // +0x10
    float NormalZ;                              // +0x14
    float U1;                                   // +0x18
    float V1;                                   // +0x1c
};
FABLE_STATIC_ASSERT(sizeof(CTVertexNormTex1Base) == 0x20);
#endif // FABLE_ENGINE_CTVERTEXNORMTEX1BASE_H
