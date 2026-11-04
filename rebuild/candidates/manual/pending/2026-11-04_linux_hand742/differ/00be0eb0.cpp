// CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw a screen-space particle group
// Grade: DIFFER(2889v2889) -- same size; the bookmark test/or operand-order lane gap in two SetRenderState blocks (0x00BE1620, 0x00BE16C6) and `mov ebx, 1` scheduled one x87 instruction earlier than retail (0x00BE0FE4); 14 masked lines. Next idea: the lane-gap pattern moved with unrelated FP respellings (5 blocks -> 2), so more neutral respellings of the camera / scale code may hit retail's pattern.
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManager2DParticleGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE0EB0, 2889 bytes, thiscall, `ret 8`; vtable slot 5 of
//   CEnginePrimitiveManager2DParticleGroup (vtable 0x012A22D0). FableWin.pdb's next COMDAT is CRenderStateManager::SetStippleEnable
//   (an empty 16-byte debug body: it emits nothing in retail, so its call site is not reconstructed).
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
// Layout: retail CEngineInternalPrimitive2DParticleGroup = Ego_r minus 0x14 (Layer +0xAC, InverseTransform +0xB0, Pos +0xE0).
// Levers found: depth copied from the local position before the transform (retail's second Z copy at esp+0x14); the X row of the
//   view->world transform as E11*x + (E21*y + E31*z) + E41 and the Y / Z rows left to right; the scale in two statements;
//   CMatrix3x4::PostScale / PostTranslate members (new in the overlay; the by-reference operand fixes the translation add order).
// Header debt: the camera members (the CEngineCamera retail branch is opaque) and the particle-group classes are local models.
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#define CEngineInternalPrimitiveParticleGroup CEngineInternalPrimitiveParticleGroup_Layout
#include "engine/CEngineInternalPrimitiveParticleGroup.h"
#undef CEngineInternalPrimitiveParticleGroup
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
#define CEngineCamera CEngineCamera_Layout
#include "engine/CEngineCamera.h"
#undef CEngineCamera
#define CEnginePrimitiveManager2DParticleGroup CEnginePrimitiveManager2DParticleGroup_Layout
#include "engine/CEnginePrimitiveManager2DParticleGroup.h"
#undef CEnginePrimitiveManager2DParticleGroup
class CEngineCamera { public:
    unsigned char _pad000[0xc0];
    float OneOverHalfWindowWidth;                   // +0xc0
    float OneOverHalfWindowHeight;                  // +0xc4
    float WindowCenterX;                            // +0xc8
    float WindowCenterY;                            // +0xcc
    float AspectRatio;                              // +0xd0
    float HomogenousViewScaleX;                     // +0xd4
    float HomogenousViewScaleY;                     // +0xd8
    float ZBufferFactor;                            // +0xdc
    float ZBufferOffset;                            // +0xe0
    CMatrix3x4 InverseViewTransform;                // +0xe4
    // ?TransformScreenXYLinearZToView@CEngineCamera@@QBEXAAVC3DVector@@@Z
    __forceinline void TransformScreenXYLinearZToView(C3DVector& point) const
    {
        point.X = (point.X - WindowCenterX) * point.Z;
        point.X *= OneOverHalfWindowWidth;
        point.Y = (WindowCenterY - point.Y) * OneOverHalfWindowHeight * point.Z;
    }
    // ?TransformViewToWorld@CEngineCamera@@QBEXAAVC3DVector@@@Z
    __forceinline void TransformViewToWorld(C3DVector& point) const
    {
        const CMatrix3x4& m = InverseViewTransform;
        const float xx = point.X;
        const float yy = point.Y;
        const float zz = point.Z;
        point.X = m.E11 * xx + (m.E21 * yy + m.E31 * zz) + m.E41;
        point.Y = m.E12 * xx + m.E22 * yy + m.E32 * zz + m.E42;
        point.Z = m.E13 * xx + m.E23 * yy + m.E33 * zz + m.E43;
    }
    // ?TransformScreenXYLinearZToWorld@CEngineCamera@@QBEXAAVC3DVector@@@Z
    __forceinline void TransformScreenXYLinearZToWorld(C3DVector& point) const
    {
        TransformScreenXYLinearZToView(point);
        TransformViewToWorld(point);
    }
};
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0

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
class CEngineInternalPrimitive2DParticleGroup : public CEngineInternalPrimitiveParticleGroup {
public:
    long Layer;                                     // +0xac EEngine2DLayer
    CMatrix3x4 InverseTransform;                    // +0xb0
    C3DVector Pos;                                  // +0xe0
    unsigned long AlphaMaskBankIndex;               // +0xec
};
class CEnginePrimitiveManager2DParticleGroup : public CEnginePrimitiveManagerParticleGroup {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass);
};

void CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    CEngineInternalPrimitive2DParticleGroup* group = (CEngineInternalPrimitive2DParticleGroup*)sub.Parent;
    CMatrix3x4 transform = group->InverseTransform;
    C3DVector pos = group->Pos;
    float depth = pos.Z;
    const CEngineCamera& camera = *g_pEngineCamera;
    camera.TransformScreenXYLinearZToWorld(pos);
    float scale = depth * camera.OneOverHalfWindowWidth * 100.0f;
    transform.PostScale(scale / camera.HomogenousViewScaleX);
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
    for (SParticleGroupNode* node = group->Primitives->_M_next; node != group->Primitives; node = node->_M_next)
        RenderSubPrimGroups(*node->Data, pass, group->WorldMat);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
    if (pass == 6)
        NEngineInternal::GPRenderManager->AttachTextureToStage(1, 0);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
}
