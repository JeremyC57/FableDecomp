// CEnginePrimitiveManager2DSprites::RenderSpritePrim(CEngineInternalPrimitiveBase& prim, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type) -- clip a 2D sprite against the viewport and the 2D alpha mask, fetch its frame UVs and the mask texture, rotate the quad and queue a 2D sprite sub-primitive
// Leaf: ?RenderSpritePrim@CEnginePrimitiveManager2DSprites@@MAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB0970, 3202 bytes (0x00BB0970..0x00BB15F1, `ret 0Ch`), protected virtual thiscall, vtable 0x012A5664 slot 14.
// Grade: RELOCATION_MATCH (3202v3202). No flags, no pragma. sub esp,74h, no EH frame (both CGraphicFrame objects are inline).
// Retail-asm: 0x00BB113A..0x00BB1142 `fld dword ptr [esp+34h] ; fistp dword ptr [esp+2Ch] ; fild dword ptr [esp+2Ch]` and 0x00BB1187..0x00BB118F
//   `fld dword ptr [esp+3Ch] ; fistp dword ptr [esp+24h] ; fild dword ptr [esp+24h]` -- the round-to-nearest helper engine/GFFloatToLongNear.h
//   expanded in GFFastCos / GFFastSin (the cos-table lookups 0x013CD550 / 0x013CD554).
// Identity: Ego_r.pdb / FableWin.pdb ?RenderSpritePrim@CEnginePrimitiveManager2DSprites@@MAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z;
//   vtable 0x012A5664 slot 14 = 0x00BB0970 (slot 5 RenderSubPrimitive 0x00BAE2D0, slot 13 RenderClockPrim 0x00BAFA00). `chain` unused.
// Callees (out of line): CDisplayManager::GetRenderTargetDimensions 0x009BEDC0, CGraphicFrameData::GetTextureQuadUVs 0x009FC810 (landed,
//   descriptive name), CTexture::GetPixelFormat 0x009F9D80 / GetWidth 0x004372A0 / GetHeight 0x004372C0, CEnginePrimitiveRenderer2D::
//   Layer2DToDistance 0x00B4A6B0 / Layer2DDistanceToZ 0x00B4A6C0 / Layer2DToSortDistance 0x00B4A6A0, CEngineSubPrimitiveBase::operator new
//   0x00BEB540, ??0CEngineSubPrimitive2DSprite 0x00BACE50, CEngineSubPrimitiveRenderer::AddSubPrimitive 0x00B84720, GFAXToVX 0x009E1DA0 /
//   GFAYToVY 0x009E1DD0, CRenderManager2D::Draw2DBoxOutline 0x009DE140, and the expanded GetFrame's callees (ListRemove 0x009FBFB0,
//   ListInsert 0x009FBF90, BeginLoading 0x009FDE80, CFrameData ctor 0x009FE380, InternalGetFrameForcedLoading 0x009FF6B0, Uninitialise
//   0x009FCBA0, operator new 0x00BFEA1A / delete 0x00BFE9BC).
// Expanded helpers: CGraphicInfoFrame::GetFrame (shared opt-in, twice: sprite frame 0x00BB0A80.. and 2D alpha-mask frame 0x00BB0E83..),
//   CGraphicDataBank::PeekGraphic / PeekGraphicInfoFrame (RenderSnow 0x00B54E30 texts), GetCurrent2DAlphaMaskInfo (0x00BAE2D0 text),
//   GFFastCos (shared header) / GFFastSin (landed 0x00A147E0 text), CEnginePrimitiveRenderer2D::Layer2DToZ / GetEnable2DLayer,
//   GetEnable2DDisplacement, GetSatRGBATexture, CGraphicFrame::GetWidth / GetHeight (new overlay opt-ins, hdr/README.md).
// Spellings: EVIDENCED -- half-texel nudge 0.01f / frame size (float pool 0x012A5428 = 0.01); UVs via a 32-byte temporary copied with
//   `rep movsd` into uvs (0x00BB0C7E); vertex order TL TR BL BR with UVs 0 1 3 2 (stores 0x00BB138B..0x00BB142A); colour halved with
//   alpha kept (0x00BB1338..0x00BB1347); the texture read before the clip tests (0x00BB0988); the displacement copy loop (0x00BB14D6).
//   FITTED (chosen by grading; each places c2's 83-node scheduling windows of the 0x00BB10EC block where retail has them):
//   the UV temporary as a named copy in its own block (`{ CTextureQuadUVs quad = ...; uvs = quad; }`: copy from `lea esi,[esp+3Ch]`, not
//   from the returned pointer); `ClipBox.GetWidth() / GetHeight()` for the half extents (the lea/push of GetRenderTargetDimensions after
//   the first corner fstp, 0x00BB1219); the clip-space conversion as two statements per coordinate (`X *= scale_x; X -= 1.0f;`,
//   `Y *= scale_y; Y = 1.0f - Y;`: the colour computation after the last corner store, 0x00BB132B); each vertex's DiffuseColour after its
//   UV (store order 0x00BB140F).
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSPRITE
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_MANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_MEMBERS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_SETTERS_ALPHAFLOAT
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_LAYER2DTODISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_LAYER2DTOZ
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_GETENABLE2DDISPLACEMENT
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXOUTLINE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_SATRGBATEXTURE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICFRAMEDATA_TEXTUREQUADUVS
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_FORMATQUERIES
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_ISLINEAR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_GETASINT
#define FABLE_RETAIL_GFFASTCOS_FORCEINLINE
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInterpolationInfo.h"
#include "engine/CEngineInternalPrimitive2DSprite.h"
#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEngineSubPrimitive2DSprite.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicFrame.h"
#include "engine/CTexture.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DExtentsI.h"
#include "engine/CRGBColour.h"
#include "engine/GFFastCos.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                                       // 0x01436E18
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }                                     // 0x01436E1C
namespace NEngineInternal { extern CEngineScreenEffectDisplacementRenderer* GPScreenEffectDisplacementRenderer; }   // 0x01436E38
namespace NEngineInternal { extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer; }                   // 0x01436E74
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }     // 0x01436E78
namespace NEngineInternal { extern CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D; }                     // 0x01436E7C
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }                         // 0x01436E84
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }                             // 0x01436E98
float __fastcall GFAXToVX(float x);                                                                         // 0x009E1DA0
float __fastcall GFAYToVY(float y);                                                                         // 0x009E1DD0

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
// GFFastSin: the landed body of 0x00A147E0 (global_GFFastSin_00a147e0.cpp), expanded here.
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1];
}
inline const C2DAlphaMaskInfo* CEnginePrimitive2DViewportManager::GetCurrentAlphaMask() { return &AlphaMaskInfo; }
FABLE_FORCEINLINE const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    CEnginePrimitive2DViewportManager* manager = GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_2D_VIEWPORT),
                                                                      (CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}

void CEnginePrimitiveManager2DSprites::RenderSpritePrim(CEngineInternalPrimitiveBase& prim, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type)
{
    CEngineInternalPrimitive2DSprite& sprite = static_cast<CEngineInternalPrimitive2DSprite&>(prim);
    CTexture* texture = sprite.Texture;
    const C2DBoxI& viewport = NEngineInternal::GPDisplayManager->GetViewport();
    if (viewport.BRX < sprite.ClipBox.TLX || viewport.TLX > sprite.ClipBox.BRX || viewport.BRY < sprite.ClipBox.TLY || viewport.TLY > sprite.ClipBox.BRY)
        return;
    const C2DAlphaMaskInfo* clip_info = GetCurrent2DAlphaMaskInfo();
    if (clip_info->AlphaMaskBankIndex && clip_info->AlphaMaskBank && clip_info->MaskObjectsOutsideAlphaMask) {
        if (sprite.ClipBox.TLX > clip_info->AlphaMaskClipBox.BRX || sprite.ClipBox.BRX < clip_info->AlphaMaskClipBox.TLX ||
            sprite.ClipBox.TLY > clip_info->AlphaMaskClipBox.BRY || sprite.ClipBox.BRY < clip_info->AlphaMaskClipBox.TLY)
            return;
    }
    if (sprite.Colour.A == 0)
        return;
    if (!NEngineInternal::GPPrimitiveRenderer2D->GetEnable2DLayer(sprite.Layer))
        return;
    unsigned long flags = (sprite.EnginePrimitiveFlags & 0x80) ? 0x11 : 0;

    CTextureQuadUVs uvs;
    if (sprite.BankIndex) {
        CGraphicFrame frame;
        if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(sprite.BankIndex, sprite.Frame)->GetFrame(frame, 0, flags))
            return;
        texture = frame.GetTexture();
        {
            CTextureQuadUVs quad = frame.FrameData->GetTextureQuadUVs();
            uvs = quad;
        }
        float du = 0.01f / frame.GetWidth();
        float dv = 0.01f / frame.GetHeight();
        uvs.U[1] += du;
        uvs.U[2] += du;
        uvs.V[2] += dv;
        uvs.V[3] += dv;
        C2DVector offset = (sprite.UVOffsetsEnd - sprite.UVOffsetsStart) * sprite.GetCurrentInterpolate() + sprite.UVOffsetsStart;
        uvs.U[0] += offset.X;
        uvs.V[0] += offset.Y;
        uvs.U[1] += offset.X;
        uvs.V[1] += offset.Y;
        uvs.U[2] += offset.X;
        uvs.V[2] += offset.Y;
        uvs.U[3] += offset.X;
        uvs.V[3] += offset.Y;
    } else {
        uvs.U[0] = 0.0f;
        uvs.V[0] = 0.0f;
        uvs.U[1] = 1.0f;
        uvs.V[1] = 0.0f;
        uvs.U[2] = 1.0f;
        uvs.V[2] = 1.0f;
        uvs.U[3] = 0.0f;
        uvs.V[3] = 1.0f;
    }
    if (texture && texture->GetPixelFormat().IsLinear()) {
        float width = (float)texture->GetWidth(0);
        uvs.U[0] *= width;
        uvs.U[1] *= width;
        uvs.U[2] *= width;
        uvs.U[3] *= width;
        float height = (float)texture->GetHeight(0);
        uvs.V[0] *= height;
        uvs.V[1] *= height;
        uvs.V[2] *= height;
        uvs.V[3] *= height;
    }

    CTexture* alpha_mask;
    const C2DAlphaMaskInfo* mask_info = GetCurrent2DAlphaMaskInfo();
    if (mask_info->AlphaMaskBankIndex && mask_info->AlphaMaskBank) {
        CGraphicFrame mask;
        if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(mask_info->AlphaMaskBankIndex, 0)->GetFrame(mask, 0, flags))
            return;
        alpha_mask = mask.GetTexture();
    } else {
        alpha_mask = NEngineInternal::GPResourceManager->GetSatRGBATexture();
    }

    float half_width = sprite.ClipBox.GetWidth() * 0.5f;
    float half_height = sprite.ClipBox.GetHeight() * 0.5f;
    float angle = (sprite.EndAngle - sprite.StartAngle) * sprite.GetCurrentInterpolate() + sprite.StartAngle;
    float cosine = GFFastCos(angle);
    float sine = GFFastSin(angle);
    C2DVector across(cosine * half_width, -(half_width * sine));
    C2DVector down(-(sine * half_height), -(cosine * half_height));
    C2DVector centre(half_width + sprite.ClipBox.TLX, half_height + sprite.ClipBox.TLY);
    C2DVector corner0 = centre - across + down;
    C2DVector corner1 = centre + across + down;
    C2DVector corner2 = centre - across - down;
    C2DVector corner3 = centre + across - down;
    const C2DExtentsI target = NEngineInternal::GPDisplayManager->GetRenderTargetDimensions();
    float scale_x = 1.0f / (target.X * 0.5f);
    float scale_y = 1.0f / (target.Y * 0.5f);
    corner0.X *= scale_x;
    corner0.X -= 1.0f;
    corner0.Y *= scale_y;
    corner0.Y = 1.0f - corner0.Y;
    corner1.X *= scale_x;
    corner1.X -= 1.0f;
    corner1.Y *= scale_y;
    corner1.Y = 1.0f - corner1.Y;
    corner2.X *= scale_x;
    corner2.X -= 1.0f;
    corner2.Y *= scale_y;
    corner2.Y = 1.0f - corner2.Y;
    corner3.X *= scale_x;
    corner3.X -= 1.0f;
    corner3.Y *= scale_y;
    corner3.Y = 1.0f - corner3.Y;
    unsigned long colour = sprite.Colour.GetAsInt();
    colour = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    float z = NEngineInternal::GPPrimitiveRenderer2D->Layer2DToZ(sprite.Layer);

    CEngineSubPrimitive2DSprite* sub = new CEngineSubPrimitive2DSprite(&sprite);
    sub->Vertices[0].X = corner0.X;
    sub->Vertices[0].Y = corner0.Y;
    sub->Vertices[0].Z = z;
    sub->Vertices[0].U1 = uvs.U[0];
    sub->Vertices[0].V1 = uvs.V[0];
    sub->Vertices[0].DiffuseColour = colour;
    sub->Vertices[1].X = corner1.X;
    sub->Vertices[1].Y = corner1.Y;
    sub->Vertices[1].Z = z;
    sub->Vertices[1].U1 = uvs.U[1];
    sub->Vertices[1].V1 = uvs.V[1];
    sub->Vertices[1].DiffuseColour = colour;
    sub->Vertices[2].X = corner2.X;
    sub->Vertices[2].Y = corner2.Y;
    sub->Vertices[2].Z = z;
    sub->Vertices[2].U1 = uvs.U[3];
    sub->Vertices[2].V1 = uvs.V[3];
    sub->Vertices[2].DiffuseColour = colour;
    sub->Vertices[3].X = corner3.X;
    sub->Vertices[3].Y = corner3.Y;
    sub->Vertices[3].Z = z;
    sub->Vertices[3].U1 = uvs.U[2];
    sub->Vertices[3].V1 = uvs.V[2];
    sub->Vertices[3].DiffuseColour = colour;
    sub->Texture = texture;
    sub->BlendMode = sprite.BlendMode;
    sub->AlphaMask = alpha_mask;
    sub->Filter = sprite.Filter;
    sub->WrapTexture = sprite.WrapTexture;
    sub->SetAlpha(0xff);
    sub->SetRenderFlags(1);
    if (sprite.EnginePrimitiveFlags & 0x400) {
        if (NEngineInternal::GPScreenEffectDisplacementRenderer->GetEnable2DDisplacement()) {
            NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_DISPLACEMENT_BUFFER, sub,
                                                                    NEngineInternal::GPPrimitiveRenderer2D->Layer2DToSortDistance(sprite.Layer), this);
            CEngineSubPrimitive2DSprite* stencil = new CEngineSubPrimitive2DSprite(&sprite);
            for (long i = 0; i < 4; ++i)
                stencil->Vertices[i] = sub->Vertices[i];
            stencil->Texture = sub->Texture;
            stencil->AlphaMask = sub->AlphaMask;
            stencil->Filter = sub->Filter;
            stencil->WrapTexture = sub->WrapTexture;
            stencil->BlendMode = sub->BlendMode;
            NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_DISPLACEMENT_STENCIL, stencil,
                                                                    NEngineInternal::GPPrimitiveRenderer2D->Layer2DToSortDistance(sprite.Layer), this);
        }
    } else {
        NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(type, sub, NEngineInternal::GPPrimitiveRenderer2D->Layer2DToSortDistance(sprite.Layer), this);
    }
    if (NEngineInternal::GPPrimitiveRendererSharedData->Show2DBoundingRect) {
        C2DBoxF box;
        box.TLX = GFAXToVX(sprite.ClipBox.TLX);
        box.TLY = GFAYToVY(sprite.ClipBox.TLY);
        box.BRX = GFAXToVX(sprite.ClipBox.BRX);
        box.BRY = GFAYToVY(sprite.ClipBox.BRY);
        NEngineInternal::GPRenderManager->Draw2DBoxOutline(box, CRGBColour(255, 0, 0, 255), 0);
    }
}