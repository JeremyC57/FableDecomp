// CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw a screen-space particle group
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManager2DParticleGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE0EB0, 2889 bytes, thiscall, `ret 8`; vtable slot 5 of CEnginePrimitiveManager2DParticleGroup
//   (vtable 0x012A22D0). FableWin.pdb's next COMDAT is CRenderStateManager::SetStippleEnable (an empty 16-byte debug body: it emits
//   nothing in retail, so its call site is not reconstructed).
// Inferred: 8 dead release asserts on the parent group (FABLE_RETAIL_ASSERT, fable_assert.h): FableWin's debug copy of this row is 400i
//   against 214i without them (each dead arm 18i at /Od), so the original carried several debug checks here; the count is FITTED inside
//   the matching window 6..8 (0..5: bookmark test/or 0x00BE16C6; 9..10: 0x00BE1620).
// FITTED: FABLE_RETAIL_CENGINECAMERA_DISTH_NAMED (overlay; the repo _MESH2D_ORIGINAL text of TransformDistScreenToViewHorizontal on the
//   FORCEINLINE branch: keeps retail's depth*1/hw*100/hvs order at 0x00BE0FA0). EVIDENCED: FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR (repo,
//   donor 024543E0 left-to-right rows; with it the PostScale run schedules `mov ebx,1` at 0x00BE0FE4 as retail), the camera helpers called
//   through NEngineInternal::GPCamera with no named reference (retail reloads [0x01436EA0] per helper; X row (X-WCX)*Z*1/hw at 0x00BE0F02).
// Purpose: place a screen-space particle group in the world: transform its screen position (X, Y, linear Z) through the camera to
//   world space, scale its inverse transform by Z * OneOverHalfWindowWidth * 100 / HomogenousViewScaleX and translate it there;
//   remember the bounding sphere, open a capture block with shaders on, set wrap UVs / blend on / alpha test GREATER / no fog,
//   alpha ref + z-write + z-test per pass (6, 7: ref 0, no z; else ref 0x80 + z-write when the group writes z, z-test = group
//   flag), set the scaled matrix as world transform under VS constant layout 5, PrepareForRender(1), render every sub-group with
//   the group's world matrix, detach stage 0 (and 1 in pass 6) and reset the world transform.
// Callees: RenderSubPrimGroups 0x00BBC130, CShaderRenderManager::SetWorldTransform 0x009881F0 / ResetWorldTransform 0x00988290 /
//   EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 / DisablePixelShaders
//   0x00988190 / SetVertexShaderConstantLayout 0x0098B5E0 and the PrepareForRender updaters, RestoreCaptureBlock 0x00A05840.
//   Constant 100.0f is 0x01230A08.
// Headers: shared CEnginePrimitiveManager2DParticleGroup.h / CEnginePrimitiveManagerParticleGroup.h (_RENDER opt-ins), the shared
//   CEngineInternalPrimitiveParticleGroup.h and the new overlay CEngineInternalPrimitive2DParticleGroup.h (Ego_r layout minus 0x14).
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DPARTICLEGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DPARTICLEGROUP_RENDER
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_SCREENTOWORLD
#define FABLE_RETAIL_CENGINECAMERA_DISTH_NAMED
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEnginePrimitiveManager2DParticleGroup.h"
#include "engine/CEngineInternalPrimitive2DParticleGroup.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineCamera.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                    // 0x01436EA0

void CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    CEngineInternalPrimitive2DParticleGroup* group = (CEngineInternalPrimitive2DParticleGroup*)sub.GetParentBase();
    CMatrix3x4 transform = group->InverseTransform;
    FABLE_RETAIL_ASSERT(group);
    FABLE_RETAIL_ASSERT(group);
    FABLE_RETAIL_ASSERT(group);
    FABLE_RETAIL_ASSERT(group);
    FABLE_RETAIL_ASSERT(group);
    FABLE_RETAIL_ASSERT(group);
    FABLE_RETAIL_ASSERT(group);
    FABLE_RETAIL_ASSERT(group);
    C3DVector pos = group->Pos;
    float depth = pos.Z;
    NEngineInternal::GPCamera->TransformScreenXYLinearZToView(pos);
    NEngineInternal::GPCamera->TransformViewToWorld(pos);
    float scale = 100.0f;
    NEngineInternal::GPCamera->TransformDistScreenToViewHorizontal(depth, scale);
    transform.PostScale(scale);
    transform.PostTranslate(pos);
    CurrentWorldPos = C3DVector(group->BSphere.Center);
    CurrentBSRadius = group->BSphere.Radius;
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    ReadZBuffer = group->ReadZBuffer;
    WriteZBuffer = group->ZBufferWritable;
    if (pass == 6) {
        NEngineInternal::GPRenderManager->SetAlphaRef(0);
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    } else if (pass == 7) {
        NEngineInternal::GPRenderManager->SetAlphaRef(0);
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    } else {
        if (WriteZBuffer) {
            NEngineInternal::GPRenderManager->SetAlphaRef(0x80);
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
        } else {
            NEngineInternal::GPRenderManager->SetAlphaRef(0);
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
        }
        NEngineInternal::GPRenderManager->SetZBufferEnable(ReadZBuffer);
    }
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    CScopedVertexShaderConstantLayout layout(5);
    PointLightsSet = false;
    PointLightCount = 0;
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    for (std::list<CEnginePrimitiveHandle>::const_iterator it = group->Primitives.begin(); it != group->Primitives.end(); ++it)
        RenderSubPrimGroups((CEngineInternalPrimitiveBase&)it->GetData(), pass, group->WorldMat);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
    if (pass == 6)
        NEngineInternal::GPRenderManager->AttachTextureToStage(1, 0);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
}
