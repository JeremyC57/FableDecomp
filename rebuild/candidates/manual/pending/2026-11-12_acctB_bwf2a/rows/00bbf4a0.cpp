// CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw a particle
// sprite group: fetch frame 0 of the group's sprite graphic (give up when it is not loaded) and bind its texture, set the pass states
// (displacement pass 6 with 3D displacement on / mask pass 7: displacement vertex shader by sprite kind; else blend op / mode, point lights
// or full self illumination, the point-light vertex shader by sprite kind and the group pixel shader), bind the sprite buffers, load
// constants 1..3 (inverse view axes, sprite offset selectors, frame size factors), then pack each visible sprite with a loaded frame
// (position, fast cos/sin of its angle and size, colour with doubled alpha -- premultiplied when additive --, frame texel offset) into
// ConstantBlock and draw in batches of ten sprites; finally restore the lighting manager's primitive defaults.
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerParticleSpriteGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBF4A0, 4169 bytes, thiscall `ret 8`, no EH frame. Vtable 0x012A6140 slot 5.
// Callees: CGraphicInfo::GetFrame 0x009FF8D0, CGraphicFrameData::Uninitialise 0x009FCBA0 + operator delete 0x00BFE9BC (expanded ~CGraphicFrame),
//   CShaderRenderManager::ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 / SetUserVertexShaderConstants 0x009896D0 and the
//   PrepareForRender(0x15) updaters (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0,
//   UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateWorldSpaceLighting 0x0098A540,
//   UpdateCombinedProjectionTransform 0x00988A50, UpdateCameraPos 0x00989700), CEnginePrimitiveManagerParticleGroup::SetUpPointLights
//   0x00BBC0B0 / SetupPixelShader 0x00BBCC70, CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80,
//   CRenderManagerCore::DrawCurrentBuffersAsIndexedTris 0x00A0AC60, IDirect3DDevice9 SetTexture / SetStreamSource / SetIndices (65/100/104).
// Shape (each FITTED against retail): the early GetFrame failure and the normal exit each expand ~CGraphicFrame; the pass 6 / 7 vertex
//   shader pickers (Apply*DisplacementVertexShader) and the point-light pickers are expanded and their ApplyVertexShader calls cross-jump;
//   UpdateGlobalLightingStates follows both light branches (the point-light arm jumps to it); the camera constants go through named
//   references c0..c2 to ConstantBlock[0..2] (written in place, the inverse view rows read off the camera after the first two loads);
//   each sprite's four constant rows are references `ConstantBlock[count++]`; the angle is copied to a local so GFFastSin reuses
//   GFFastCos's angle * 1024 (both written as the landed out-of-line bodies 0x00A14840 / 0x00A147E0); GetFloatTLX/TLY carry FableWin's
//   two-local text (14i/38b debug), which gives retail's `cmp esi,28h` before the second fild.
// Retail-asm: 0x00BC010D fistp dword ptr [esp+24h] (GFFloatToLongNear inside GFFastCos; engine/GFFloatToLongNear.h)
// Retail-asm: 0x00BC0159 fistp dword ptr [esp+2Ch] (GFFloatToLongNear inside GFFastSin; engine/GFFloatToLongNear.h)
// Bodies in this file: CEnginePrimitiveRenderUtil::ApplyVertexShader / ApplyPixelShader / GetCurrentCamera / SetupBlendOpState /
//   SetupAlphaBlendingStatesFromBlendMode are declared by the shared opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
//   (engine/CEnginePrimitiveRenderUtil.h: "the including row defines the inline bodies") and are copied verbatim from the sibling row
//   0x00BC25F0 CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive (not yet landed: /opt/fable_lane/w22/agent2059/src/00bc25f0.cpp,
//   its 'Inline-budget' lines). Visible-body facts: FableWin.pdb has only debug copies (SetupBlendOpState 0x02EAC160 78b/25i,
//   ApplyVertexShader 0x02E74E10 28b/12i, GetCurrentCamera 0x02E74B40 10b/5i), Ego_r.pdb and Fable.exe have none, and this body never
//   calls them: it calls CShaderRenderManager::ApplyVertexShader 0x00988020 directly (0x00BBF7F9, 0x00BBFA04, 0x00BBFDFD, 0x00BBFE34),
//   reads g_pEngineCamera inline (0x00BBFF0A) and reads CRenderStateManager::D3DBlendOpModeTable inline (0x00BBF600, 0x00BBFA36).
// Inline-budget (measured on this row, repair grades): SetupBlendOpState and SetupAlphaBlendingStatesFromBlendMode are FABLE_FORCEINLINE
//   as in 0x00BC25F0, whose c2 trace measured the same two helpers (variable-argument sites: SetupAlphaBlendingStatesFromBlendMode c1 693,
//   over c2's hard 450 cap; SetupBlendOpState c1 154, f 0.0312, score 836: refused as plain inline); this row has the same variable-argument
//   sites (group->BlendOp / group->BlendMode) and as plain inline it grades DIFFER(3225v4169), 81 hunks.
//   GFFastSin (and GFFastCos through the overlay opt-in FABLE_RETAIL_GFFASTCOS_FORCEINLINE) is FABLE_FORCEINLINE: retail expands both
//   (their fistp sites 0x00BC010D / 0x00BC0159 sit in this body, while the out-of-line copies 0x00A14840 / 0x00A147E0 exist); with
//   GFFastSin plain inline the row grades DIFFER(4005v4169), 59 hunks (frame `sub esp,30h` v retail's 38h).
// Names: GFDebugPDynamicCast comes from the shared template in engine/CThing.h (FableWin
//   ??$GFDebugPDynamicCast@PAVCEnginePrimitiveManager@@PAVCEnginePrimitiveManagerParticleGroup@@...). g_pDisplacementRenderer_01436e38 is the
//   established landed spelling of 0x01436E38 (CEngine::Initialise 0x00B26360 stores `new CEngineScreenEffectDisplacementRenderer` there;
//   also 0x00B24500, 0x00BC25F0); no PDB / string / FSE / EgoCore name exists for it (descriptive, see NOTES.md and NAME_FIXES.tsv).
// Flags: none
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
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_GETENABLE3D
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
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_GFFASTCOS_FORCEINLINE
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveManagerParticleSpriteGroup.h"
#include "engine/CEngineInternalPrimitiveParticleSpriteGroup.h"
#include "engine/CEngineInternalPrimitiveParticleSprite.h"
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
#include "engine/GFFastCos.h"
#include "engine/CThing.h"                                  // GFDebugPDynamicCast (shared template, CThing.h:47)

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                           // 0x01436E84
extern CEngineLightingManager* g_pEngineLightingManager;                               // 0x01436E9C
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;      // 0x01436E38
extern CEngineCamera* g_pEngineCamera;                                                 // 0x01436EA0
extern long g_particleSpriteRenderEnabled_0139b6e0;                                    // 0x0139B6E0 (descriptive)


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

void CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    if (!g_particleSpriteRenderEnabled_0139b6e0)
        return;
    CEngineInternalPrimitiveParticleSpriteGroup* group = (CEngineInternalPrimitiveParticleSpriteGroup*)sub.GetParentBase();
    const CGraphicInfo* info = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(group->SpriteBankIndex);
    CGraphicFrame frame;
    if (!info->GetFrame(frame, 0, 0, 0))
        return;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame.GetTexture());
    CEnginePrimitiveManagerParticleGroup* group_manager =
        GFDebugPDynamicCast(g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(24), (CEnginePrimitiveManagerParticleGroup*)0);
    bool additive;
    if (pass == RENDER_PASS_DISPLACEMENT_BUFFER && g_pDisplacementRenderer_01436e38->GetEnable3DDisplacement()) {
        additive = false;
        SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
        SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);
        if (group->Flags & 1) {
            Apply3DFaceMeDisplacementVertexShader();
        } else if (group->Flags & 0x40) {
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
        if (group->Flags & 1) {
            Apply3DFaceMeDisplacementVertexShader();
        } else if (group->Flags & 0x40) {
            NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
            ApplyCrossedSpriteDisplacementVertexShader();
        } else {
            ApplyBillboardDisplacementVertexShader();
        }
        ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(group->BlendOp);
        additive = SetupAlphaBlendingStatesFromBlendMode(group->BlendMode);
        unsigned long light_count = 0;
        if (group->Flags & 0x80)
            light_count = group_manager->SetUpPointLights();
        else
            g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        if (group->Flags & 1) {
            Apply3DFaceMePointLightVertexShader(light_count);
        } else if (group->Flags & 0x40) {
            NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
            ApplyCrossedSpritePointLightVertexShader(light_count);
        } else {
            ApplyBillboardPointLightVertexShader(light_count);
        }
        group_manager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
    }
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, &VertexBuffer);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(&IndexBuffer, 0);
    float z_offset = 0.0f;
    float w_offset = 0.0f;
    if (group->Flags & 2)
        z_offset = 1.0f;
    else if (group->Flags & 4)
        z_offset = -1.0f;
    else if (group->Flags & 8)
        w_offset = -1.0f;
    else if (group->Flags & 0x10)
        w_offset = 1.0f;
    const CMatrix3x4& view = GetCurrentCamera().PeekInverseNonHomogenousViewTransform();
    CEngineParticleConstantBlock& c0 = ConstantBlock[0];
    CEngineParticleConstantBlock& c1 = ConstantBlock[1];
    CEngineParticleConstantBlock& c2 = ConstantBlock[2];
    c0.X = view.E11;
    c0.Y = view.E21;
    c0.Z = z_offset;
    c0.W = w_offset;
    c1.X = view.E12;
    c1.Y = view.E22;
    c1.Z = info->GetFrameWidthFactor();
    c1.W = info->GetFrameHeightFactor();
    c2.X = view.E13;
    c2.Y = view.E23;
    c2.Z = (group->Flags & 0x20) ? 1.0f : 0.0f;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(1, 3, &ConstantBlock[0].X);
    unsigned long count = 0;
    for (std::list<CEnginePrimitiveHandle>::const_iterator it = group->Primitives.begin(); it != group->Primitives.end(); ++it) {
        const CEngineInternalPrimitiveParticleSprite& sprite = (const CEngineInternalPrimitiveParticleSprite&)it->GetData();
        if (sprite.Size.X > 0.0f && sprite.Size.Y > 0.0f && info->GetFrame(frame, sprite.Frame, 0, 0)) {
            CRGBFloatColour colour(sprite.Colour);
            colour.A *= 2.0f;
            if (additive) {
                colour.R *= colour.A;
                colour.G *= colour.A;
                colour.B *= colour.A;
            }
            CEngineParticleConstantBlock& position = ConstantBlock[count++];
            position.X = sprite.WorldPos.X;
            position.Y = sprite.WorldPos.Y;
            position.Z = sprite.WorldPos.Z;
            float angle = sprite.Angle;
            CEngineParticleConstantBlock& shape = ConstantBlock[count++];
            shape.X = GFFastCos(angle);
            shape.Y = GFFastSin(angle);
            shape.Z = sprite.Size.X;
            shape.W = sprite.Size.Y;
            CEngineParticleConstantBlock& col = ConstantBlock[count++];
            col.X = colour.R;
            col.Y = colour.G;
            col.Z = colour.B;
            col.W = colour.A;
            CEngineParticleConstantBlock& offset = ConstantBlock[count++];
            offset.X = frame.GetFloatTLX();
            offset.Y = frame.GetFloatTLY();
            if (count == 40) {
                NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(4, count, &ConstantBlock[0].X);
                NEngineInternal::GPShaderRenderManager->PrepareForRender(0x15);
                NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 40, 0, 20);
                count = 0;
            }
        }
    }
    if (count) {
        unsigned long sprites = count >> 2;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(4, sprites * 4, &ConstantBlock[0].X);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0x15);
        NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, sprites * 4, 0, sprites * 2);
    }
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}
