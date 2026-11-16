// CEnginePrimitiveManager2DSprites::RenderClockPrim(CEngineInternalPrimitiveBase& prim, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type) -- clip a 2D clock sprite against the viewport and the 2D alpha mask, fetch the mask texture and its two frames, build the seven-vertex clock fan up to the interpolated angle and queue it
// Inline-budget: /d2inlS18 -- GFLimitReturn (the angle clamp) is expanded in retail; at /d2inlS17 and below c2 leaves it called (caller-size cap; /d2inlT<N> has no effect); S18 is the smallest that expands it.
// Leaf: ?RenderClockPrim@CEnginePrimitiveManager2DSprites@@MAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z
// Flags: /d2inlS18
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BAFA00, 3946 bytes (0x00BAFA00..0x00BB0969, `ret 0Ch`), protected virtual thiscall, vtable 0x012A5664 slot 13.
// Spellings: EVIDENCED by retail bytes unless marked: each index-loop arm writes its whole triangle fan entry, the `following` index included
//   (the two stores are one cross-jumped tail at 0x00BB085F; written once after the if/else, c2 gives the AddSubPrimitive type local edi
//   instead of the loop zero, see NOTES.md); `to` declared before `from` (FITTED); clip-space conversion as two statements per coordinate
//   (FITTED, the 0x00BB0970 window-edge lesson); the fraction clamp as an explicit ternary (immediate stores 0x00BB0721..0x00BB0746).
// Retail-asm: 0x00BB0769..0x00BB0783 `fld [ebp+2Ch] ; fadd [ebp+24h] ; fistp dword ptr [ebp+74h] ; fild [ebp+74h] ; fsub [ebp+34h] ; fstp [ebp+3Ch] ; mov eax,[ebp+2Ch] ; cmp eax,[ebp+3Ch] ; jne ; dec dword ptr [ebp+74h]` (0.5f / 1.0f stored as frame constants 0x00BB074C / 0x00BB0759) -- the shared inline-assembly ceiling helper GFFloatToLongCeil (engine/GFFloatToLongCeil.h) expanded in place for the clock segment; VC7.1 emits no bare dword fistp from C++.
// Retail-asm-helper: GFFloatToLongCeil
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DCLOCKSPRITE
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
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInterpolationInfo.h"
#include "engine/CEngineInternalPrimitive2DClockSprite.h"
#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEngineSubPrimitive2DClockSprite.h"
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
#include "engine/GFFloatToLongCeil.h"
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
inline const C2DAlphaMaskInfo* CEnginePrimitive2DViewportManager::GetCurrentAlphaMask() { return &AlphaMaskInfo; }
FABLE_FORCEINLINE const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    CEnginePrimitive2DViewportManager* manager = GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_2D_VIEWPORT),
                                                                      (CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}

template <class T> inline const T& __fastcall GFLimitReturn(const T& value, const T& low, const T& high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

void CEnginePrimitiveManager2DSprites::RenderClockPrim(CEngineInternalPrimitiveBase& prim, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type)
{
    CEngineInternalPrimitive2DClockSprite& clock = static_cast<CEngineInternalPrimitive2DClockSprite&>(prim);
    const C2DBoxI& viewport = NEngineInternal::GPDisplayManager->GetViewport();
    if (viewport.BRX < clock.ClipBox.TLX || viewport.TLX > clock.ClipBox.BRX || viewport.BRY < clock.ClipBox.TLY || viewport.TLY > clock.ClipBox.BRY)
        return;
    const C2DAlphaMaskInfo* clip_info = GetCurrent2DAlphaMaskInfo();
    if (clip_info->AlphaMaskBankIndex && clip_info->AlphaMaskBank && clip_info->MaskObjectsOutsideAlphaMask) {
        if (clock.ClipBox.TLX > clip_info->AlphaMaskClipBox.BRX || clock.ClipBox.BRX < clip_info->AlphaMaskClipBox.TLX ||
            clock.ClipBox.TLY > clip_info->AlphaMaskClipBox.BRY || clock.ClipBox.BRY < clip_info->AlphaMaskClipBox.TLY)
            return;
    }
    if (clock.Colour.A == 0)
        return;
    if (!NEngineInternal::GPPrimitiveRenderer2D->GetEnable2DLayer(clock.Layer))
        return;
    unsigned long flags = (clock.EnginePrimitiveFlags >> 7) & 1;

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

    CTextureQuadUVs uvs;
    CTexture* textures[2];
    if (clock.BankIndex) {
        CGraphicFrame frame;
        if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(clock.BankIndex, clock.Frame)->GetFrame(frame, 0, flags))
            return;
        textures[0] = frame.GetTexture();
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
    } else {
        textures[0] = 0;
    }
    if (clock.BankIndex2) {
        CGraphicFrame frame;
        if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(clock.BankIndex2, clock.Frame2)->GetFrame(frame, 0, flags))
            return;
        textures[1] = frame.GetTexture();
        if (!textures[0]) {
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
        }
    } else {
        textures[1] = 0;
    }
    if (!textures[0] && !textures[1]) {
        uvs.U[0] = 0.0f;
        uvs.V[0] = 0.0f;
        uvs.U[1] = 1.0f;
        uvs.V[1] = 0.0f;
        uvs.U[2] = 1.0f;
        uvs.V[2] = 1.0f;
        uvs.U[3] = 0.0f;
        uvs.V[3] = 1.0f;
    } else {
        C2DVector offset = (clock.UVOffsetsEnd - clock.UVOffsetsStart) * clock.GetCurrentInterpolate() + clock.UVOffsetsStart;
        uvs.U[0] += offset.X;
        uvs.V[0] += offset.Y;
        uvs.U[1] += offset.X;
        uvs.V[1] += offset.Y;
        uvs.U[2] += offset.X;
        uvs.V[2] += offset.Y;
        uvs.U[3] += offset.X;
        uvs.V[3] += offset.Y;
        if (textures[0] && textures[0]->GetPixelFormat().IsLinear()) {
            float width = (float)textures[0]->GetWidth(0);
            uvs.U[0] *= width;
            uvs.U[1] *= width;
            uvs.U[2] *= width;
            uvs.U[3] *= width;
            float height = (float)textures[0]->GetHeight(0);
            uvs.V[0] *= height;
            uvs.V[1] *= height;
            uvs.V[2] *= height;
            uvs.V[3] *= height;
        } else if (textures[1] && textures[1]->GetPixelFormat().IsLinear()) {
            float width = (float)textures[1]->GetWidth(0);
            uvs.U[0] *= width;
            uvs.U[1] *= width;
            uvs.U[2] *= width;
            uvs.U[3] *= width;
            float height = (float)textures[1]->GetHeight(0);
            uvs.V[0] *= height;
            uvs.V[1] *= height;
            uvs.V[2] *= height;
            uvs.V[3] *= height;
        }
    }

    float half_width = (clock.ClipBox.BRX - clock.ClipBox.TLX) * 0.5f;
    float half_height = (clock.ClipBox.BRY - clock.ClipBox.TLY) * 0.5f;
    float z = NEngineInternal::GPPrimitiveRenderer2D->Layer2DToZ(clock.Layer);
    C2DVector centre(half_width + clock.ClipBox.TLX, half_height + clock.ClipBox.TLY);
    C2DVector top_left(clock.ClipBox.TLX, clock.ClipBox.TLY);
    C2DVector top_right(clock.ClipBox.BRX, clock.ClipBox.TLY);
    C2DVector bottom_left(clock.ClipBox.TLX, clock.ClipBox.BRY);
    C2DVector bottom_right(clock.ClipBox.BRX, clock.ClipBox.BRY);
    const C2DExtentsI target = NEngineInternal::GPDisplayManager->GetRenderTargetDimensions();
    float scale_x = 1.0f / (target.X * 0.5f);
    float scale_y = 1.0f / (target.Y * 0.5f);
    centre.X *= scale_x;
    centre.X -= 1.0f;
    centre.Y *= scale_y;
    centre.Y = 1.0f - centre.Y;
    top_left.X *= scale_x;
    top_left.X -= 1.0f;
    top_left.Y *= scale_y;
    top_left.Y = 1.0f - top_left.Y;
    top_right.X *= scale_x;
    top_right.X -= 1.0f;
    top_right.Y *= scale_y;
    top_right.Y = 1.0f - top_right.Y;
    bottom_left.X *= scale_x;
    bottom_left.X -= 1.0f;
    bottom_left.Y *= scale_y;
    bottom_left.Y = 1.0f - bottom_left.Y;
    bottom_right.X *= scale_x;
    bottom_right.X -= 1.0f;
    bottom_right.Y *= scale_y;
    bottom_right.Y = 1.0f - bottom_right.Y;

    CEngineSubPrimitive2DClockSprite* sub = new CEngineSubPrimitive2DClockSprite(&clock);
    sub->Vertices[0].X = centre.X;
    sub->Vertices[0].Y = centre.Y;
    sub->Vertices[0].Z = z;
    sub->Vertices[0].U1 = (uvs.U[2] - uvs.U[0]) * 0.5f + uvs.U[0];
    sub->Vertices[0].V1 = (uvs.V[2] - uvs.V[0]) * 0.5f + uvs.V[0];
    sub->Vertices[1].X = centre.X;
    sub->Vertices[1].Y = top_left.Y;
    sub->Vertices[1].Z = z;
    sub->Vertices[1].U1 = (uvs.U[1] - uvs.U[0]) * 0.5f + uvs.U[0];
    sub->Vertices[1].V1 = uvs.V[0];
    sub->Vertices[2].X = top_right.X;
    sub->Vertices[2].Y = top_right.Y;
    sub->Vertices[2].Z = z;
    sub->Vertices[2].U1 = uvs.U[1];
    sub->Vertices[2].V1 = uvs.V[1];
    sub->Vertices[3].X = bottom_right.X;
    sub->Vertices[3].Y = bottom_right.Y;
    sub->Vertices[3].Z = z;
    sub->Vertices[3].U1 = uvs.U[2];
    sub->Vertices[3].V1 = uvs.V[2];
    sub->Vertices[4].X = bottom_left.X;
    sub->Vertices[4].Y = bottom_left.Y;
    sub->Vertices[4].Z = z;
    sub->Vertices[4].U1 = uvs.U[3];
    sub->Vertices[4].V1 = uvs.V[3];
    sub->Vertices[5].X = top_left.X;
    sub->Vertices[5].Y = top_left.Y;
    sub->Vertices[5].Z = z;
    sub->Vertices[5].U1 = uvs.U[0];
    sub->Vertices[5].V1 = uvs.V[0];

    float angle = (clock.AngleEnd - clock.AngleStart) * clock.GetCurrentInterpolate() + clock.AngleStart;
    angle = GFLimitReturn(angle, 0.0f, 1.0f);
    float fraction;
    if (angle <= 0.0 || angle >= 1.0f)
        fraction = angle < 0.0f ? 0.0f : (angle > 1.0f ? 1.0f : angle);
    else if (angle < 0.125f || angle > 0.875f)
        fraction = (float)fmod((double)angle, 0.125) * 8.0f;
    else
        fraction = (float)fmod((double)(angle - 0.125f), 0.25) * 4.0f;

    long segment = GFFloatToLongCeil(angle * 4.0f + 0.5f);
    long next = segment == 5 ? 1 : segment + 1;
    const CTVertexColTex2& to = sub->Vertices[next];
    const CTVertexColTex2& from = sub->Vertices[segment];
    sub->Vertices[6].X = (to.X - from.X) * fraction + from.X;
    sub->Vertices[6].Y = (to.Y - from.Y) * fraction + from.Y;
    sub->Vertices[6].Z = z;
    sub->Vertices[6].U1 = (to.U1 - from.U1) * fraction + from.U1;
    sub->Vertices[6].V1 = (to.V1 - from.V1) * fraction + from.V1;
    long index = 0;
    unsigned short* indices = sub->Indices;
    for (long i = 1; i < 6; ++i) {
        long following = i == 5 ? 1 : i + 1;
        if (segment == i) {
            indices[index++] = 0;
            indices[index++] = (unsigned short)i;
            indices[index++] = 6;
            sub->IndexOffset = (unsigned char)index;
            indices[index++] = 0;
            indices[index++] = 6;
            indices[index++] = (unsigned short)following;
        } else {
            indices[index++] = 0;
            indices[index++] = (unsigned short)i;
            indices[index++] = (unsigned short)following;
        }
    }

    sub->Colour = clock.Colour2;
    sub->Colour2 = clock.Colour;
    sub->Texture[0] = textures[1];
    sub->Texture[1] = textures[0];
    sub->AlphaMask = alpha_mask;
    sub->Filter = clock.Filter;
    sub->WrapTexture = clock.WrapTexture;
    sub->SetAlpha(0xff);
    sub->SetRenderFlags(1);
    ESubPrimitiveType list;
    if (clock.EnginePrimitiveFlags & 0x400)
        list = SUBPRIM_TYPE_DISPLACEMENT_BUFFER;
    else
        list = type;
    NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(list, sub, NEngineInternal::GPPrimitiveRenderer2D->Layer2DToSortDistance(clock.Layer), this);
    if (NEngineInternal::GPPrimitiveRendererSharedData->Show2DBoundingRect) {
        C2DBoxF box;
        box.TLX = GFAXToVX(clock.ClipBox.TLX);
        box.TLY = GFAYToVY(clock.ClipBox.TLY);
        box.BRX = GFAXToVX(clock.ClipBox.BRX);
        box.BRY = GFAYToVY(clock.ClipBox.BRY);
        NEngineInternal::GPRenderManager->Draw2DBoxOutline(box, CRGBColour(255, 0, 0, 255), 0);
    }
}