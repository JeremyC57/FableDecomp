// CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw a 3D particle group
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerParticleGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBC460, thiscall, `ret 8`; vtable slot 5 of CEnginePrimitiveManagerParticleGroup
//   (vtable 0x012A5BB8). Identity: FableWin.pdb / Ego_r.pdb ?RenderSubPrimitive@CEnginePrimitiveManagerParticleGroup@@UAEX...
// Purpose: remember the group's bounding sphere for the point-light pickers, open a render-state capture block, force vertex +
//   pixel shaders on, set wrap UVs / alpha test GREATER 0 / no fog / the group's dither flag, choose blend + z-write per pass
//   (6 displacement: blend, no z-write; 7: neither; else z-write = group flag, blend = !z-write, z-test = group flag), then
//   render every sub-group through RenderSubPrimGroups with the group's world matrix under vertex-shader constant layout 5;
//   detach texture stage 0 (and 1 for the displacement pass or z-writing groups) before the scopes restore state.
// Callees: CEnginePrimitiveManagerParticleGroup::RenderSubPrimGroups 0x00BBC130, CEngineLightingManager::UpdateGlobalLightingStates
//   0x00B46C80, CShaderRenderManager::ResetWorldTransform 0x00988290 / EnableVertexShaders 0x00987FE0 / EnablePixelShaders
//   0x00988110 / DisableVertexShaders 0x009880E0 / DisablePixelShaders 0x00988190 / SetVertexShaderConstantLayout 0x0098B5E0,
//   CRenderStateManager::RestoreCaptureBlock 0x00A05840 (CRenderStateScopedCaptureBlock dtor).
// Layout: retail CEngineInternalPrimitiveParticleGroup (Ego_r offsets minus 0x14 after the STLport containers: Primitives +0x48,
//   BSphere +0x5C, WorldMat +0x70, ReadZBuffer +0xA1, ZBufferWritable +0xA8, ClipEffectToWeatherMask +0xA9, EnableDithering
//   +0xAA); CEnginePrimitiveManagerParticleGroup members +0x0C..+0x24 packed (CurrentWorldPos at +0x0E, Ego_r.pdb).
//   Local models (header debt): CEngineSubPrimitiveBase, the list node and both particle-group classes.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#define CEngineInternalPrimitiveParticleGroup CEngineInternalPrimitiveParticleGroup_Layout
#include "engine/CEngineInternalPrimitiveParticleGroup.h"
#undef CEngineInternalPrimitiveParticleGroup
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A

class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
};
struct SParticleGroupNode {                         // STLport list<CEnginePrimitiveHandle> node
    SParticleGroupNode* _M_next;
    SParticleGroupNode* _M_prev;
    void*               HandleVTable;               // +0x08 CEnginePrimitiveHandle
    CEngineInternalPrimitiveBase* Data;             // +0x0c
};
#include "engine/C3DBoundingSphere.h"
class CEngineInternalPrimitiveParticleGroup : public CEngineInternalPrimitiveBase {
public:
    SParticleGroupNode* Primitives;                 // +0x48
    unsigned char PrimitiveIDToIterator[0xc];       // +0x4c
    void* FreePrimitiveIDs;                         // +0x58
    C3DBoundingSphere BSphere;                      // +0x5c
    float DistanceFromCam;                          // +0x6c
    CMatrix3x4 WorldMat;                            // +0x70
    bool ScreenDisplacement;                        // +0xa0
    bool ReadZBuffer;                               // +0xa1
    long WaterStage;                                // +0xa4
    bool ZBufferWritable;                           // +0xa8
    bool ClipEffectToWeatherMask;                   // +0xa9
    bool EnableDithering;                           // +0xaa
};
#pragma pack(push, 1)
class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass);
protected:
    void RenderSubPrimGroups(CEngineInternalPrimitiveBase& primitive, ERenderPassType pass, const CMatrix3x4& transform);   // 0x00BBC130
    bool PointLightsSet;                            // +0x0c
    signed char PointLightCount;                    // +0x0d
    C3DVector CurrentWorldPos;                      // +0x0e
    unsigned char _pad1a[2];
    float CurrentBSRadius;                          // +0x1c
    bool ClipEffectToWeatherMask;                   // +0x20
    bool ReadZBuffer;                               // +0x21
    bool WriteZBuffer;                              // +0x22
    bool DisplayBoundingBox;                        // +0x23
    bool DisplayBoundingSphere;                     // +0x24
};
#pragma pack(pop)

void CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    CEngineInternalPrimitiveParticleGroup* group = (CEngineInternalPrimitiveParticleGroup*)sub.Parent;
    CurrentWorldPos = C3DVector(group->BSphere.Center);
    CurrentBSRadius = group->BSphere.Radius;
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetAlphaRef(0);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPRenderManager->SetDitherEnable(group->EnableDithering);
    ReadZBuffer = group->ReadZBuffer;
    WriteZBuffer = group->ZBufferWritable;
    ClipEffectToWeatherMask = group->ClipEffectToWeatherMask;
    if (pass == 6) {
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    } else if (pass == 7) {
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    } else {
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(WriteZBuffer);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(!WriteZBuffer);
        NEngineInternal::GPRenderManager->SetZBufferEnable(ReadZBuffer);
    }
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    CScopedVertexShaderConstantLayout layout(5);
    PointLightsSet = false;
    PointLightCount = 0;
    for (SParticleGroupNode* node = group->Primitives->_M_next; node != group->Primitives; node = node->_M_next) {
        NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
        RenderSubPrimGroups(*node->Data, pass, group->WorldMat);
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
    if (pass == 6 || WriteZBuffer)
        NEngineInternal::GPRenderManager->AttachTextureToStage(1, 0);
}