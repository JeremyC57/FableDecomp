// CEnginePrimitiveManagerText::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw a 3D text sub-primitive: vertex-shader constant layout 1, a render-state capture block, shaders on unless the streaming fonts run fixed-function (then the screen-space scale constant from the render target size), wrap / linear stage 0, alpha blend SRC_ALPHA / INV_SRC_ALPHA without alpha test; with a mask texture, stage 1 clamped (point / no mip unless the mask manager says otherwise), the mask UVs to the vertex shader (or the streaming font's fixed-function mask UVs) and the mask shaders, else the plain shaders; then the font renders the string at its viewport-offset position
// Grade: DIFFER(2217v2217) -- size match; vertex-shader constants, the inline PrepareForRender expansion, the member-wise text position, the viewport / font-bank calls and the +0x0C / +8 mask-texture pointer split (descriptive layout) are retail-shaped (slot-agnostic 214 lines); residue: the SetRenderState lane differs in operand / register order and the CScopedShadersEnable destructor's compare form
// Next idea: spell the render-state block with the state values in locals / a different argument order and the scoped shader guard's flag as a bool member test, then re-check the lane register order
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerText@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC6DB0, 2217 bytes, thiscall `ret 8`; vtable slot 5.
// Callees: CShaderRenderManager::SetVertexShaderConstantLayout 0x0098B5E0 / EnableVertexShaders 0x00987FE0 / DisableVertexShaders
//   0x009880E0 / EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190 / SetUserVertexShaderConstant(KMMMM) 0x00989B00 /
//   SetUserVertexShaderConstant(K, const C4DVector&) 0x00989AB0 / ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 and the
//   inline PrepareForRender expansion (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0,
//   UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, ResetWorldTransform 0x00988290,
//   UpdateCombinedProjectionTransform 0x00988A50), CDisplayManager::GetRenderTargetDimensions 0x009BEDC0,
//   CRenderManager2D::GetViewportBoxFloat 0x00A0AAA0, CStreamingFontBank::SetAlphaMaskUVs 0x009A0EC0 (static fastcall),
//   CRenderStateManager::RestoreCaptureBlock 0x00A05840; CFontBank virtual slots 3 (PrepareToRender) / 5 (Render).
// Globals: CStreamingFontBank's use-fixed-function flag 0x013C5A7C (set by CStreamingFontBank::SetUseFixedFunction 0x009A0EF0).
// Layout: Ego_r.pdb CEnginePrimitiveManagerText (VShader +0x0C, VShaderMask +0x14, PShader +0x1C, PShaderMask +0x24) and
//   CEngineSubPrimitiveText (Pos +0x18, Scale +0x24, Colour +0x28, Text +0x2C, Font +0x30, Mask +0x34).
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CRENDERMANAGER2D_VIEWPORTBOXFLOAT
#include "engine/CRenderManager2D.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsI.h"
#include "engine/CFontBank.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();      // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager_01436e1c;    // 0x01436E1C
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CStreamingFontBank { public:
    static bool UseFixedFunction;                       // 0x013C5A7C (file static of the streaming-font module)
    static void __fastcall SetAlphaMaskUVs(const C2DVector& uv0, const C2DVector& uv1);   // 0x009A0EC0
};
struct CTextMaskTexture {                               // the +0x0C sub-object (descriptive)
    unsigned char _pad00[0x08];
    C4DVector MaskUVs;                                  // +0x08 (settings +0x14)
    unsigned char _pad18[0x11];
    bool KeepMaskFiltering;                             // +0x29 (settings +0x35)
};
struct CTextMaskSettings {                              // the +0x94 object of the 0x01436E84 engine table (descriptive)
    unsigned char _pad00[0x0c];
    CTextMaskTexture Texture;                           // +0x0c
};
struct CEnginePrimitiveManagerTable {
    unsigned char _pad00[0x94];
    CTextMaskSettings* TextMask;                        // +0x94
};
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;   // 0x01436E84
class CRGBColour;
class CTexture;
class CEngineSubPrimitiveBase { public:
    virtual ~CEngineSubPrimitiveBase();
    unsigned char _pad04[0x14];
};
class CEngineSubPrimitiveText : public CEngineSubPrimitiveBase { public:
    C3DVector Pos;                                      // +0x18
    float Scale;                                        // +0x24
    CRGBColour* ColourPlaceholder() { return (CRGBColour*)&Colour; }
    unsigned long Colour;                               // +0x28 CRGBColour
    CWideString Text;                                   // +0x2c
    CFontBank* Font;                                    // +0x30
    CTexture* Mask;                                     // +0x34
};
class CEnginePrimitiveManagerText : public CEnginePrimitiveManager { public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass);
    CVertexShader VShader;                              // +0x0c
    CVertexShader VShaderMask;                          // +0x14
    CPixelShader PShader;                               // +0x1c
    CPixelShader PShaderMask;                           // +0x24
};

void CEnginePrimitiveManagerText::RenderSubPrimitive(CEngineSubPrimitiveBase& base, ERenderPassType pass)
{
    CEngineSubPrimitiveText& sub = static_cast<CEngineSubPrimitiveText&>(base);
    bool useShaders = !CStreamingFontBank::UseFixedFunction;
    CScopedVertexShaderConstantLayout layout(1);
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(useShaders, true);
    if (useShaders) {
        C2DExtentsI dims = g_pDisplayManager_01436e1c->GetRenderTargetDimensions();
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, 1.0f / ((float)dims.X * 0.5f),
                                                                           1.0f / ((float)dims.Y * 0.5f), -1.0f, 1.0f);
    }
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode((ALPHA_BLEND_MODE)4);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode((ALPHA_BLEND_MODE)5);
    if (sub.Mask) {
        const CTextMaskTexture* maskTexture = &g_pPrimitiveManagers->TextMask->Texture;
        if (!maskTexture->KeepMaskFiltering) {
            NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureMipMapMode(1, TEXTURE_MIPMAP_DISABLE);
        }
        NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
        if (useShaders) {
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, maskTexture->MaskUVs);
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VShaderMask, false);
        } else {
            C4DVector uvs = maskTexture->MaskUVs;
            CStreamingFontBank::SetAlphaMaskUVs(C2DVector(uvs.X, uvs.Y), C2DVector(uvs.Z, uvs.W));
        }
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PShaderMask);
    } else {
        if (useShaders)
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PShader);
    }
    const C2DBoxF& viewport = NEngineInternal::GPRenderManager->GetViewportBoxFloat();
    C3DVector pos;
    pos.X = sub.Pos.X + viewport.TLX;
    pos.Y = viewport.TLY + sub.Pos.Y;
    pos.Z = sub.Pos.Z;
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    sub.Font->PrepareToRender(false);
    sub.Font->Render(sub.Text, pos, sub.Scale, *sub.ColourPlaceholder(), 0, sub.Mask);
}
