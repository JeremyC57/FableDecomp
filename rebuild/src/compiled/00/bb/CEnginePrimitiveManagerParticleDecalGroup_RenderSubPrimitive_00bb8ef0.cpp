// CEnginePrimitiveManagerParticleDecalGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- render a particle decal group: when the manager is enabled and the group (the sub-primitive's parent) holds decals and is a landscape decal group, fetch frame 0 of its decal-bank graphic (return if it is not available), then inside a render-state capture block with vertex + pixel shaders on: bind the frame's texture to stage 0, clamp U and V, set the blend op and the source / dest blend factors from the group (premultiplied alpha for the additive modes), set up point lights for a lit group (else 100% self illumination), update the global lighting states, flush the shader state, select vertex-shader constant layout 3, draw the landscape decals and restore the lighting manager's primitive defaults
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerParticleDecalGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: /d2inlT189
// Inline-budget: /d2inlT189 (smallest; /d2inlT188 gives 1884v1941) -- c2 inline trace (tools/inl.sh, cost rule, default T 74): retail expands
//   CGraphicDataBank::PeekGraphic (c1 55, f 0.125, score 110), CRenderStateScopedCaptureBlock() (c1 38, f 0.0625, score 119) and
//   ~CScopedShadersEnable (c1 60, f 0.0625, score 188) behind the four early exits (enable, empty group, mesh-decal group, no frame); every other
//   site is already expanded at T 74, so no other decision moves. Plain C++ control flow first: early returns + an `if (GetFrame) { }` body
//   (probe v3) give the same frequencies (every exit test halves them; the end needs f >= 0.38 for ~CScopedShadersEnable) and different code;
//   the scoped helpers have retail out-of-line copies (~CScopedShadersEnable 0x009D8B40, CScopedShadersEnable 0x009D8AD0, both uncalled), so
//   they stay plain `inline` in the shared headers. SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode are FABLE_FORCEINLINE (owner
//   2026-11-01 rule: FableWin.pdb-only debug copies, no Ego_r.pdb copy, none in this TU's retail COMDAT run 0x00BB9690..0x00BB9952): as plain
//   inline the trace refuses them (SetupBlendOpState c1 154, score 484 at f 0.0625; SetupAlphaBlendingStatesFromBlendMode c1 693, over c2's
//   hard 450 cost cap that no /d2inlT lifts; probe v4p: 1296v1941).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB8EF0, 1941 bytes (`sub esp,10h` .. `ret 8` at 0x00BB9682, int3 to 0x00BB9690; the catalog's 2659 also
//   covered 0x00BB9690..0x00BB9952, a run of unreferenced inline COMDATs of the decal TU: STLport list helpers, distance 0x00BB9770,
//   ApplyLandscapeVertexShader 0x00BB97F0, ApplyMeshVertexShader 0x00BB9810, CArray / vector<CTVertexNormTex1> helpers). No EH frame.
// Identity: Ego_r.pdb + FableWin.pdb ?RenderSubPrimitive@CEnginePrimitiveManagerParticleDecalGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z;
//   vtable slot 5 of the class's RTTI vtable 0x012A5998 (rtti-vtables.tsv; the only reference to 0x00BB8EF0 is 0x012A59AC); the slot-5 override
//   of CEnginePrimitiveManager::RenderSubPrimitive (`ret 8`, two arguments; `pass` is not read).
// FableWin.pdb's COMDAT list for this TU (donor 0x02EABCA0 RenderSubPrimitive, then SetupAlphaBlendingStatesFromBlendMode 0x02EAC0C0,
//   SetupBlendOpState 0x02EAC160, RenderLandscapeDecalGroup 0x02EAC1D0, ..., list::size 0x02EAD750, ...,
//   GFDebugPDynamicCast<CEnginePrimitiveManager*, CEnginePrimitiveManagerParticleGroup*> 0x02EAEFB0) names the helpers this body uses.
// Callees: CGraphicInfo::GetFrame 0x009FF8D0, CGraphicFrameData::Uninitialise 0x009FCBA0 + operator delete 0x00BFE9BC (the expanded
//   ~CGraphicFrame), CShaderRenderManager::EnableVertexShaders 0x00987FE0 / EnablePixelShaders 0x00988110 / DisableVertexShaders 0x009880E0 /
//   DisablePixelShaders 0x00988190 (CScopedShadersEnable), IDirect3DDevice9::SetTexture (slot 65, AttachTextureToStage),
//   CEnginePrimitiveManagerParticleGroup::SetUpPointLights 0x00BBC0B0, CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80, the
//   PrepareForRender(1) updaters (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient
//   0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50),
//   CShaderRenderManager::SetVertexShaderConstantLayout 0x0098B5E0 (CScopedVertexShaderConstantLayout), RenderLandscapeDecalGroup 0x00BB8750
//   (its only caller, 0x00BB9533), CRenderStateManager::RestoreCaptureBlock 0x00A05840 (~CRenderStateScopedCaptureBlock).
// Globals: NEngineInternal::GPResourceManager 0x01436E98, GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14,
//   g_pEnginePrimitiveTypeInfo 0x01436E84 (GetPrimitiveManager(24) = the particle-group manager, +0x70), g_pEngineLightingManager 0x01436E9C
//   (established spellings; no PDB data symbol on this machine), CRenderStateCaptureBlock::StateManager 0x013CB508,
//   CShaderRenderManager::SingletonInstance 0x013BC470; D3D tables CRenderStateManager::D3DTextureUVModeTable 0x01396DC8 (CLAMP 0x01396DD0),
//   D3DBlendOpModeTable 0x01396ED0, D3DAlphaBlendModeTable 0x01396F68.
// Inline bodies proven against retail's uncalled out-of-line copies (free probes): CGraphicDataBank::PeekGraphic = 0x0041C1B0 (MATCH),
//   distance<list<CEnginePrimitiveHandle>::const_iterator> (list::size, FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE) = 0x00BB9770 (MATCH).
// Shape: `CGraphicFrame frame;` is declared before the graphic lookup (retail zeroes FrameData in the dead `sub` home at 0x00BB8F44, before the
//   bank read); the remaining shape follows the parked sibling 0x00BBE090 RenderSingleSpriteTrail (blend op / blend mode helpers, point lights
//   or self illumination, UpdateGlobalLightingStates, RestoreDefaultPrimitiveParameters). The second LandscapeDecal test before the call is
//   retail's (0x00BB951E): the mesh-decal branch is compiled out on PC (FableWin RenderMeshDecalGroup is an empty 0x10-byte stub).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAME
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveManagerParticleDecalGroup.h"
#include "engine/CEngineInternalPrimitiveParticleDecalGroup.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveManagerParticleGroup.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicFrame.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                           // 0x01436E84
extern CEngineLightingManager* g_pEngineLightingManager;                               // 0x01436E9C

// FableWin.pdb ??$GFDebugPDynamicCast@PAVCEnginePrimitiveManager@@PAVCEnginePrimitiveManagerParticleGroup@@@@YI... (this TU's COMDAT list): the
// release path is the plain static cast (the text engine/CThing.h and the landed rows define).
template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }

// The engine/CEnginePrimitiveRenderUtil.h opt-in declares these FableWin.pdb members and leaves the bodies to the including row (no retail copy):
// blend op 1 / 2 / other -> D3DBLENDOP_SUBTRACT / REVSUBTRACT / ADD (0x00BB9166..0x00BB91E6); blend mode 3 (ONE, ONE) and 4 (ONE, INVSRCCOLOR)
// premultiply, any other mode is SRCALPHA / INVSRCALPHA (0x00BB91EB..0x00BB9415).
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
{
    switch (op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case ENGINE_PRIMITIVE_BLEND_OP_REVSUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
        break;
    default:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        break;
    }
}
FABLE_FORCEINLINE bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}

void CEnginePrimitiveManagerParticleDecalGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    if (!GetEnable())
        return;
    const CEngineInternalPrimitiveParticleDecalGroup& group =
        static_cast<const CEngineInternalPrimitiveParticleDecalGroup&>(*sub.PeekParentBase());
    if (group.Primitives.size() > 0 && group.LandscapeDecal) {
        CGraphicFrame frame;
        const CGraphicInfo* info = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(group.DecalBankIndex);
        if (!info->GetFrame(frame, 0, 0, 0))
            return;
        CRenderStateScopedCaptureBlock capture;
        CScopedShadersEnable shaders(true, true);
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame.GetTexture());
        NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
        SetupBlendOpState(group.BlendOp);
        bool premultiplied_alpha = SetupAlphaBlendingStatesFromBlendMode(group.BlendMode);
        long point_light_count = 0;
        if (group.Flags & 0x80)
            point_light_count = GFDebugPDynamicCast(g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(24),
                                                    (CEnginePrimitiveManagerParticleGroup*)0)->SetUpPointLights();
        else
            g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
        CScopedVertexShaderConstantLayout layout(3);
        if (group.LandscapeDecal)
            RenderLandscapeDecalGroup(group, premultiplied_alpha, point_light_count);
        g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
    }
}