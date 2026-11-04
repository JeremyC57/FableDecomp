// CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw one particle sprite trail
// Grade: DIFFER(2432v2464) -- the bookmark test/or operand-order lane gap (CRenderStateManager.h) in the blend-op / blend-mode SetRenderState blocks changes VC7.1 cross-jumping there (retail keeps case 1 of the blend-op switch whole and merges case 2 with default); 46 masked lines, all in those blocks. Next idea: none in source; revisit when the lane gap is solved.
// Leaf: ?RenderSingleSpriteTrail@CEnginePrimitiveManagerParticleSpriteTrailGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBE090, 2464 bytes, thiscall, `ret 8`. Identity (catalog name proven):
//   ?RenderSingleSpriteTrail@CEnginePrimitiveManagerParticleSpriteTrailGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z in
//   both FableWin.pdb and Ego_r.pdb; the landed RenderSubPrimGroups 0x00BBC130 calls it for primitive type 20 through manager
//   slot 29 (the sprite-trail group). Non-virtual.
// Purpose: look up the trail's bank graphic and frame 0 (return if missing); under a capture block and VS constant layout 2 pick
//   blend + shaders per pass (6 with the displacement renderer on / 7: displacement shaders, no premultiply; else the trail's blend
//   op / blend mode, point lights or 100% self illumination, the clamped point-light vertex shader and the group pixel shader);
//   bind the frame texture, cull none, upload the frame UV rectangle as VS constant 1, PrepareForRender(0x11), hand the trail to
//   the sprite-trail manager, then cull CCW and restore the lighting manager's primitive defaults.
// Callees: CGraphicInfo::GetFrame 0x009FF8D0, CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail 0x00BCA950,
//   CEnginePrimitiveManagerParticleGroup::SetUpPointLights 0x00BBC0B0 / SetupPixelShader 0x00BBCC70,
//   CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80, CShaderRenderManager::ApplyVertexShader 0x00988020 /
//   ApplyPixelShader 0x00988140 / SetUserVertexShaderConstant 0x00989B00 / SetVertexShaderConstantLayout 0x0098B5E0 and the
//   PrepareForRender updaters, CRenderStateManager::RestoreCaptureBlock 0x00A05840, CGraphicFrameData::Uninitialise, operator delete.
// Levers found: CEngineResourceManager::PeekGraphicBank (FableWin.pdb inline; new in the overlay) gives retail's prologue order
//   (53 -> 46 lines); the point-light shader picker must be __forceinline; SetupPixelShader(0, Stipple, Normal).
// Header debt: CGraphicFrameData / CGraphicFrame / CGraphicInfo / CGraphicDataBank, the sprite-trail primitive, the group managers
//   and CEngineSubPrimitiveBase are local models (no retail header branch yet).
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CArray.h"
#define CGraphicFrame CGraphicFrame_Layout
#include "engine/CGraphicFrame.h"
#undef CGraphicFrame
#define CEnginePrimitiveManagerParticleSpriteTrailGroup CEnginePrimitiveManagerParticleSpriteTrailGroup_Layout
#include "engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h"
#undef CEnginePrimitiveManagerParticleSpriteTrailGroup

void __cdecl operator delete(void* block);                                         // 0x00BFE9BC
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
extern CEngineLightingManager* g_pEngineLightingManager;                           // 0x01436E9C
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                       // 0x01436E84
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;  // 0x01436E38

enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_ADD, ENGINE_PRIMITIVE_BLEND_OP_SUBTRACT, ENGINE_PRIMITIVE_BLEND_OP_REV_SUBTRACT };
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_MODE_0, ENGINE_PRIMITIVE_BLEND_MODE_1, ENGINE_PRIMITIVE_BLEND_MODE_2,
                                 ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE, ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE_INV_SRC_COLOUR };

class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
};
class CTexture;
class CGraphicFrameData { public:                   // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                 // +0x00
    void*           Data;                           // +0x04
    CCountedPointer<CTexture> PTexture;             // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch; // +0x10
    unsigned char   Alpha : 1;                      // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                            // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void Release() { if (--ReferenceCount == 0) delete this; }
    __forceinline CTexture* GetTexture() const { return PTexture.Data; }
    __forceinline float GetFloatTLX() const { return TLX * (1.0f / 32767.0f); }
    __forceinline float GetFloatTLY() const { return TLY * (1.0f / 32767.0f); }
};
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame() { if (FrameData) { FrameData->Release(); FrameData = 0; } }
    __forceinline CTexture* GetTexture() const { return FrameData->GetTexture(); }
    __forceinline float GetFloatTLX() const { return FrameData->GetFloatTLX(); }
    __forceinline float GetFloatTLY() const { return FrameData->GetFloatTLY(); }
};
class CGraphicInfoHeader { public:                  // CGraphicDataBank::CGraphicHeader
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    unsigned char  _pad0c[0x1c - 0x0c];
};
class CGraphicInfo { public:
    CGraphicInfoHeader Header;                      // +0x00
    unsigned long      TextureType;                 // +0x1c
    void*              Bank;                        // +0x20
    long               Index;                       // +0x24
    void*              Frames;                      // +0x28
    bool GetFrame(CGraphicFrame& frame, unsigned long frameNum, unsigned long mipmapLevel, unsigned long flags) const;   // 0x009FF8D0
    __forceinline float GetFrameWidthFactor() const { return (float)Header.FrameWidth / Header.Width; }
    __forceinline float GetFrameHeightFactor() const { return (float)Header.FrameHeight / Header.Height; }
};
class CGraphicDataBank { public:
    unsigned char _pad000[0x1e0];
    CArray<CGraphicInfo> GraphicList;               // +0x1e0
    __forceinline const CGraphicInfo* PeekGraphic(unsigned long index) const
    {
        return index < GraphicList.size() ? &GraphicList[index] : 0;
    }
};
class CEngineInternalPrimitiveSpriteTrail : public CEngineInternalPrimitiveBase {
public:
    unsigned long MaxLength;                        // +0x48
    unsigned long GraphicIndex;                     // +0x4c
    unsigned long GraphicFrame;                     // +0x50
    float         SkipPoints;                       // +0x54
    float         Width;                            // +0x58
    unsigned long HeadColour, MidColour, TailColour;   // +0x5c
    unsigned char Trail[0xc];                       // +0x68 CArray<CSpriteTrailEntry>
    long          NextPos;                          // +0x74
    unsigned short Flags;                           // +0x78
    EEnginePrimitiveBlendOp BlendOp;                // +0x7c
    EEnginePrimitiveBlendMode BlendMode;            // +0x80
};
class CEnginePrimitiveManagerSpriteTrails { public:
    void RenderSpriteTrail(CEngineInternalPrimitiveSpriteTrail* trail, bool premultiply);   // 0x00BCA950
};
class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager { public:
    long SetUpPointLights();                                                       // 0x00BBC0B0
    void SetupPixelShader(unsigned long constant, const CPixelShader& a, const CPixelShader& b);   // 0x00BBCC70
};
class CEnginePrimitiveManagerParticleSpriteTrailGroup : public CEnginePrimitiveManager {
public:
    void RenderSingleSpriteTrail(CEngineSubPrimitiveBase& sub, ERenderPassType pass);
private:
    __forceinline void ApplySpriteTrailPointLightVertexShader(unsigned long lights)
    {
        unsigned long index = lights;
        if (lights > VertexShadersSpriteTrailPointLights.size() - 1)
            index = VertexShadersSpriteTrailPointLights.size() - 1;
        ApplyVertexShader(VertexShadersSpriteTrailPointLights[index]);
    }
    __forceinline void ApplyDisplacementVertexShader() { ApplyVertexShader(VertexShaderDisplacement); }
    CArray<CVertexShader> VertexShadersSpriteTrailPointLights;   // +0x0c
    CVertexShader VertexShaderDisplacement;         // +0x18
    CPixelShader PixelShader;                       // +0x20
    CPixelShader PixelShaderStipple;                // +0x28
    CPixelShader PixelShaderDisplacement;           // +0x30
    CCountedPointer<CEnginePrimitiveManagerSpriteTrails> PTrailManager;   // +0x38
};

__forceinline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
__forceinline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
__forceinline void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
{
    switch (op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUBTRACT:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case ENGINE_PRIMITIVE_BLEND_OP_REV_SUBTRACT:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
        break;
    default:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        break;
    }
}
__forceinline bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE_INV_SRC_COLOUR:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}

void CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    CEngineInternalPrimitiveSpriteTrail* trail = static_cast<CEngineInternalPrimitiveSpriteTrail*>(sub.Parent);
    const CGraphicInfo* info = NEngineInternal::GPResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->PeekGraphic(trail->GraphicIndex);
    CGraphicFrame frame;
    if (!info->GetFrame(frame, trail->GraphicFrame, 0, 0))
        return;
    CRenderStateScopedCaptureBlock capture;
    CScopedVertexShaderConstantLayout layout(2);
    CEnginePrimitiveManagerParticleGroup* groupManager = (CEnginePrimitiveManagerParticleGroup*)g_pEnginePrimitiveTypeInfo->PrimitiveManagers[24];
    bool premultiply;
    if (pass == 6 && g_pDisplacementRenderer_01436e38->PeekRenderEnabled()) {
        premultiply = false;
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else if (pass == 7) {
        premultiply = false;
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(trail->BlendOp);
        premultiply = SetupAlphaBlendingStatesFromBlendMode(trail->BlendMode);
        unsigned long lights = 0;
        if (trail->Flags & 0x80)
            lights = groupManager->SetUpPointLights();
        else
            g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        ApplySpriteTrailPointLightVertexShader(lights);
        groupManager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame.GetTexture());
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, frame.GetFloatTLX(), frame.GetFloatTLY(),
                                                                        info->GetFrameWidthFactor(), info->GetFrameHeightFactor());
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
    PTrailManager->RenderSpriteTrail(trail, premultiply);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}
