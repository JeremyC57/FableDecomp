// CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw one particle sprite: bind its bank frame's texture inside a render-state capture block, pick the blend / alpha-test states and the vertex / pixel shaders for the pass, upload the billboard axes and the sprite constants, draw the 2-triangle quad, then restore
// Leaf: ?RenderSingleSprite@CEnginePrimitiveManagerParticleSpriteGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: /d2inlT79
// Inline-budget: /d2inlT79 (smallest; T78 grades 3710v3751, T76 3673, default 3626) -- at the default threshold VC7.1 calls the inline
//   CGraphicDataBank::PeekGraphic and CRenderStateScopedCaptureBlock() out of line (retail expands both: 0x00BC053C..0x00BC0574 and
//   0x00BC0591..0x00BC05BD); every other site is expanded already. Same inline-budget class as the landed decal-group row 0x00BB8EF0 (/d2inlT189).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC04F0, 3751 bytes, `ret 8`. Identity: Ego_r.pdb / FableWin.pdb (0x02EB58B0, the particle-sprite-group
//   TU of the landed sibling 0x00BBF4A0 RenderSubPrimitive, whose helper texts / opt-ins this row reuses).
// Retail-asm: 0x00BC10C5..0x00BC10CD `fld [esp+1Ch] ; fistp dword ptr [esp+48h] ; fild dword ptr [esp+48h]` and 0x00BC1118..0x00BC1120 (same) --
//   the round-to-nearest helper engine/GFFloatToLongNear.h expanded in GFFastSin / GFFastCos (the cos-table lookups 0x013CD550).
// FITTED: FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED + _BOOKMARK (existing landed fitted texts: one more named symbol per
//   SetRenderState expansion): without them the one bookmark test / or at 0x00BC0BD4 (`test esi,eax ; or eax,esi`) has its operands
//   swapped (a c2 record-count tie; dead asserts 1..6 do not move it; DESIRED + ADDSTATE_DIRTY matches too). The landed sibling 0x00BBF4A0
//   does NOT match with these texts (it uses the default), so they are a per-row record-count device, not a TU-wide text.
// EVIDENCED: alpha_scale / premultiplied C3DVector (0x00BC106F `je` -> fld 1.0f, three products stored as one vector); c2.W = 0 store
//   (0x00BC0FA4); PeekRenderEnabled (+8, 0x00BC0628; FableWin ?PeekRenderEnabled@CEngineComponent@@QBE_NXZ); SetupLocalLights / FindLocalLights
//   (FableWin.pdb static fastcall names, no retail copy).
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_BOOKMARK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP_APPLYSHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPLOCALLIGHTS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKINVERSENONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_FRAMEFACTORS
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_FLOATTL
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_GFFASTCOS_FORCEINLINE
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveManagerParticleSpriteGroup.h"
#include "engine/CEngineInternalPrimitiveParticleSpriteGroup.h"
#include "engine/CEngineInternalPrimitiveSprite.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveManagerParticleGroup.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicFrame.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/GFFastCos.h"
#include "engine/CThing.h"                                  // GFDebugPDynamicCast (shared template, CThing.h:47)

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                           // 0x01436E84
extern CEngineLightingManager* g_pEngineLightingManager;                               // 0x01436E9C
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;      // 0x01436E38
extern CEngineCamera* g_pEngineCamera;                                                 // 0x01436EA0
extern long g_spriteRenderEnabled_0139b6e4;                                            // 0x0139B6E4 (descriptive)


// GFFastSin: the landed body of 0x00A147E0 (global_GFFastSin_00a147e0.cpp), expanded here.
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1];
}

inline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera()
{
    return *g_pEngineCamera;
}
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


FABLE_FORCEINLINE CEngineLocalLightArray* __fastcall CEnginePrimitiveRenderUtil::FindLocalLights(const C3DVector& position, float radius,
                                                                                               CEnginePrimitiveThemeChannelDesc channel)
{
    return g_pEngineLightingManager->GetLightsInSphereFast(C3DBoundingSphere(position, radius), 5, channel);
}
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::SetupLocalLights(const CEngineLocalLightArray& lights)
{
    g_pEngineLightingManager->SetupShaderPointLights(lights);
}

void CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    if (!g_spriteRenderEnabled_0139b6e4)
        return;
    CEngineInternalPrimitiveSprite* sprite = (CEngineInternalPrimitiveSprite*)sub.GetParentBase();
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->EntryExists(sprite->BankIndex))
        return;
    const CGraphicInfo* info = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(sprite->BankIndex);
    CGraphicFrame frame;
    if (!info->GetFrame(frame, 0, 0, 0))
        return;
    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame.GetTexture());
    bool additive;
    if (pass == RENDER_PASS_DISPLACEMENT_BUFFER && g_pDisplacementRenderer_01436e38->PeekRenderEnabled()) {
        additive = false;
        SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
        SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);
        if (sprite->Sprite3D) {
            Apply3DFaceMeDisplacementVertexShader();
        } else if (sprite->RotateAroundZAxis) {
            NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
            ApplyCrossedSpriteDisplacementVertexShader();
        } else {
            ApplyBillboardDisplacementVertexShader();
        }
        ApplyPixelShader(PixelShaderDisplacement);
    } else if (pass == RENDER_PASS_DISPLACEMENT_STENCIL) {
        additive = false;
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaRef(16);
        NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
        if (sprite->Sprite3D) {
            Apply3DFaceMeDisplacementVertexShader();
        } else if (sprite->RotateAroundZAxis) {
            NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
            ApplyCrossedSpriteDisplacementVertexShader();
        } else {
            ApplyBillboardDisplacementVertexShader();
        }
        ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(sprite->BlendOp);
        additive = SetupAlphaBlendingStatesFromBlendMode(sprite->BlendMode);
        CEnginePrimitiveManagerParticleGroup* group_manager =
            GFDebugPDynamicCast(g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(24), (CEnginePrimitiveManagerParticleGroup*)0);
        unsigned long light_count = 0;
        if (sprite->Lit) {
            if (sprite->IsParticle) {
                light_count = group_manager->SetUpPointLights();
            } else {
                CEngineLocalLightArray* lights = FindLocalLights(sprite->WorldPos, sprite->BSphereRadius, CEnginePrimitiveThemeChannelDesc());
                light_count = lights->LightCount;
                SetupLocalLights(*lights);
            }
        } else {
            g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
        }
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        if (sprite->Sprite3D) {
            Apply3DFaceMePointLightVertexShader(light_count);
        } else if (sprite->RotateAroundZAxis) {
            NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
            ApplyCrossedSpritePointLightVertexShader(light_count);
        } else {
            ApplyBillboardPointLightVertexShader(light_count);
        }
        if (sprite->IsParticle)
            group_manager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
        else
            ApplyPixelShader(PixelShader);
    }
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, &VertexBuffer);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(&IndexBuffer, 0);
    const CMatrix3x4& view = GetCurrentCamera().PeekInverseNonHomogenousViewTransform();
    CEngineParticleConstantBlock& c0 = ConstantBlock[0];
    CEngineParticleConstantBlock& c1 = ConstantBlock[1];
    CEngineParticleConstantBlock& c2 = ConstantBlock[2];
    c0.X = view.E11;
    c0.Y = view.E21;
    c0.Z = sprite->OffsetFactorX;
    c0.W = sprite->OffsetFactorY;
    c1.X = view.E12;
    c1.Y = view.E22;
    c1.Z = info->GetFrameWidthFactor();
    c1.W = info->GetFrameHeightFactor();
    c2.X = view.E13;
    c2.Y = view.E23;
    c2.Z = sprite->RotateAroundCentre ? 1.0f : 0.0f;
    c2.W = 0.0f;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(1, 3, &ConstantBlock[0].X);
    if (sprite->Colour.A > 0 && sprite->SizeX > 0.0f && sprite->SizeY > 0.0f && info->GetFrame(frame, sprite->Frame, 0, 0)) {
        CRGBFloatColour colour(sprite->Colour);
        float alpha_scale = additive ? colour.A : 1.0f;
        C3DVector premultiplied(colour.R * alpha_scale, colour.G * alpha_scale, colour.B * alpha_scale);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, sprite->WorldPos, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(5, GFFastCos(sprite->Angle), GFFastSin(sprite->Angle), sprite->SizeX, sprite->SizeY);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(6, premultiplied.X, premultiplied.Y, premultiplied.Z, colour.A);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(7, frame.GetFloatTLX(), frame.GetFloatTLY(), 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0x15);
        NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 6, 0, 2);
    }
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}
