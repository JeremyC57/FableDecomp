#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVESPRITE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVESPRITE_H
// New header (wfE5; no repo header for the class): Ego_r.pdb CEngineInternalPrimitiveSprite, 0x88 bytes, : CEngineInternalPrimitiveBase
// (0x48) -- member names, types and offsets from ghidra_out/struct_layouts_egor.tsv. RenderSingleSprite 0x00BC04F0 reads WorldPos +0x48,
// BankIndex +0x54, Frame +0x5C, SizeX/Y +0x60/+0x64, OffsetFactorX/Y +0x68/+0x6C, BSphereRadius +0x70, Angle +0x74, BlendMode +0x78,
// BlendOp +0x7C, Colour +0x80 and the flag byte +0x84 (Modulate2X 1, Lit 2, Sprite3D 4, RotateAroundCentre 8, RotateAroundZAxis 0x10,
// IsParticle 0x20, ZBuffer 0x40). No own virtuals are modelled (the base's vptr is the class's).
#include "rebuild_abi.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
class CTexture;
class CEngineInternalPrimitiveSprite : public CEngineInternalPrimitiveBase {
public:
    C3DVector                 WorldPos;                                  // +0x48
    unsigned long             BankIndex;                                 // +0x54
    CTexture*                 Texture;                                   // +0x58
    unsigned long             Frame;                                     // +0x5c
    float                     SizeX;                                     // +0x60
    float                     SizeY;                                     // +0x64
    float                     OffsetFactorX;                             // +0x68
    float                     OffsetFactorY;                             // +0x6c
    float                     BSphereRadius;                             // +0x70
    float                     Angle;                                     // +0x74
    EEnginePrimitiveBlendMode BlendMode;                                 // +0x78
    EEnginePrimitiveBlendOp   BlendOp;                                   // +0x7c
    CRGBColour                Colour;                                    // +0x80
    unsigned char             Modulate2X : 1;                            // +0x84 bit 0
    unsigned char             Lit : 1;                                   //        bit 1
    unsigned char             Sprite3D : 1;                              //        bit 2
    unsigned char             RotateAroundCentre : 1;                    //        bit 3
    unsigned char             RotateAroundZAxis : 1;                     //        bit 4
    unsigned char             IsParticle : 1;                            //        bit 5
    unsigned char             ZBuffer : 1;                               //        bit 6
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveSprite) == 0x88);
#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVESPRITE_H
