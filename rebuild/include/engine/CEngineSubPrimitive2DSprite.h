#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVE2DSPRITE_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVE2DSPRITE_H
// New header (wfE5; no repo header for the class): Ego_r.pdb CEngineSubPrimitive2DSprite, 0xa8 bytes, over CEngineSubPrimitiveBase
// (RTTI .?AVCEngineSubPrimitive2DSprite@@ : CEngineSubPrimitiveBase). Names / offsets from struct_layouts_egor.tsv; constructor
// ??0CEngineSubPrimitive2DSprite@@QAE@PAVCEngineInternalPrimitive2DSprite@@@Z (landed 0x00BACE50). Read by
// CEnginePrimitiveManager2DSprites::RenderSubPrimitive 0x00BAE2D0 (+0x98 / +0x9c / +0xa0 / +0xa1 / +0xa4) and written by RenderSpritePrim.
#include "rebuild_abi.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CTVertexColTex2.h"
#include "engine/CEngineRenderEnums.h"
class CTexture;
class CEngineInternalPrimitive2DSprite;
class CEngineSubPrimitive2DSprite : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitive2DSprite(CEngineInternalPrimitive2DSprite* parent);   // 0x00BACE50
    virtual ~CEngineSubPrimitive2DSprite();
    CTVertexColTex2           Vertices[4];       // +0x18
    CTexture*                 Texture;           // +0x98
    CTexture*                 AlphaMask;         // +0x9c
    bool                      Filter;            // +0xa0
    bool                      WrapTexture;       // +0xa1
    EEnginePrimitiveBlendMode BlendMode;         // +0xa4
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitive2DSprite) == 0xa8);
#endif // FABLE_ENGINE_CENGINESUBPRIMITIVE2DSPRITE_H
