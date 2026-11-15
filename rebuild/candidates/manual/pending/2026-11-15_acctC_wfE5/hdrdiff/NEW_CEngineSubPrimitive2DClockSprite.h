#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVE2DCLOCKSPRITE_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVE2DCLOCKSPRITE_H
// New header (wfE5; no repo header for the class): Ego_r.pdb CEngineSubPrimitive2DClockSprite, 0x13c bytes, over CEngineSubPrimitiveBase
// (RTTI .?AVCEngineSubPrimitive2DClockSprite@@ : CEngineSubPrimitiveBase). Names / offsets from struct_layouts_egor.tsv; constructor
// ??0CEngineSubPrimitive2DClockSprite@@QAE@PAVCEngineInternalPrimitive2DClockSprite@@@Z (landed 0x00BB1780). Read by
// CEnginePrimitiveManager2DSprites::RenderSubPrimitive 0x00BAE2D0 and written by RenderClockPrim 0x00BAFA00.
#include "rebuild_abi.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CTVertexColTex2.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineRenderEnums.h"
class CTexture;
class CEngineInternalPrimitive2DClockSprite;
class CEngineSubPrimitive2DClockSprite : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite* parent);   // 0x00BB1780
    virtual ~CEngineSubPrimitive2DClockSprite();
    CTVertexColTex2           Vertices[7];       // +0x18
    unsigned short            Indices[18];       // +0xf8
    unsigned char             IndexOffset;       // +0x11c
    CTexture*                 Texture[2];        // +0x120
    CRGBColour                Colour;            // +0x128
    CRGBColour                Colour2;           // +0x12c
    CTexture*                 AlphaMask;         // +0x130
    bool                      Filter;            // +0x134
    bool                      WrapTexture;       // +0x135
    EEnginePrimitiveBlendMode BlendMode;         // +0x138
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitive2DClockSprite) == 0x13c);
#endif // FABLE_ENGINE_CENGINESUBPRIMITIVE2DCLOCKSPRITE_H
