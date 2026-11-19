#ifndef FABLE_ENGINE_CTVERTEXNORMTEX1BUMPBASE_H
#define FABLE_ENGINE_CTVERTEXNORMTEX1BUMPBASE_H
// New header (hand4d, row 0x00A8F1B0 C3DPrimitive2::ConvertToVBFormatStaticBump): Ego_r.pdb CTVertexNormTex1BumpBase
// (struct_layouts_egor.tsv: float X +0, Y +4, Z +8, NormalX +0xC, NormalY +0x10, NormalZ +0x14, U1 +0x18, V1 +0x1C, float UTangent[3]
// +0x20, VTangentSign +0x2C; sizeof 0x30), class key V (FableWin.pdb ?SetPosition@?$CGenericVertex@VCTVertexNormTex1BumpBase@@@@...).
// Retail 0x00A8F1B0 stores the fields at these offsets with a 0x30 stride.
#include "rebuild_abi.h"
class CTVertexNormTex1BumpBase {
public:
    float X;                                    // +0x00
    float Y;                                    // +0x04
    float Z;                                    // +0x08
    float NormalX;                              // +0x0c
    float NormalY;                              // +0x10
    float NormalZ;                              // +0x14
    float U1;                                   // +0x18
    float V1;                                   // +0x1c
    float UTangent[3];                          // +0x20
    float VTangentSign;                         // +0x2c
};
FABLE_STATIC_ASSERT(sizeof(CTVertexNormTex1BumpBase) == 0x30);
#endif // FABLE_ENGINE_CTVERTEXNORMTEX1BUMPBASE_H
