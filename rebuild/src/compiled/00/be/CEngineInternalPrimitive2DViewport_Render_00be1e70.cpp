// CEngineInternalPrimitive2DViewport::Render() -- render the viewport's child primitives through a 2D camera: switch the engine camera to an orthographic-style copy of its description (the viewport FOVs, near 0.1 / far 1, Z buffer 0..0.1, origin position, Y-flipped rotation); when the viewport box is non-empty open a render-state capture block, disable fog, set the display viewport, hand every child to its type's primitive manager on one sub-primitive chain, render the 2D sub-primitives, then restore the display viewport and the captured render states; finally restore the camera description
// Leaf: Render
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE1E70, length 674 bytes (`ret`).
// Callees: CEngineCameraDesc copy ctor 0x00B4AF50, CEngineCamera::SetCameraDesc 0x00B30B50 (camera @ 0x01436EA0), CDisplayManager
//   GetRenderTargetDimensions 0x009BEDC0 (result unused) / SetViewport(const C2DBoxF&) 0x009BF490 / SetViewport(const C2DBoxI&) 0x009BEF80
//   (display @ 0x01436E1C), the capture-block render-state manager @ 0x013CB508 (inline open, RestoreCaptureBlock 0x00A05840), the tracked
//   render-state manager @ 0x01436E18 (inline fog-enable SetState, as the landed EnableFog 0x00B46890), the primitive manager table
//   @ 0x01436E84 (slot 10, +0x28), CEngineSubPrimitiveRenderer::RenderType 0x00B849F0 (@ 0x01436E74).
// Layout: engine CEngineInternalPrimitive2DViewport (PDB): Viewport +0x48, HorizontalFOV +0x58, VerticalFOV +0x5c, Use2DFOV +0x60,
//   Primitives +0x94 (retail 12-byte vector of 8-byte handles, as the landed GetChildPrimitive 0x00BE1DF0). Modelled locally.
// Shape: the working description lives in its own block; the description stores follow the source order (constants, then the
//   viewport fields U/H/V, then position and matrix); the unused dimensions result is a named local (a discarded temporary shares
//   the chain's frame words, 8 bytes short); the fog disable is a __forceinline manager member (a direct SetState swaps eax/ecx and the
//   capture-block store order); the child entry is a `const&` local (an indexed member read folds the lea into the load).
// NF45 (round 7): the tracked render-state manager 0x01436E18 is NEngineInternal::GPRenderManager (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderStateManager at offset 0): the local
//   DisableFog / SetState model is the shared inline SetFogEnable(false) (DeviceState.FogEnable +0x2944). The capture open on the
//   0x013CB508 manager (push of BookmarkDummyState +0x3A24, mask shift) and RestoreCaptureBlock 0x00A05840 are a
//   CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h, repo SCOPED_FORCEINLINE; the static is
//   ?StateManager@CRenderStateCaptureBlock@@0PAVCRenderStateManager@@A). Was a local CCaptureStateManager on g_pRenderStateManager
//   and a local CRenderStateManager / CTrackedRenderState model on g_pRenderStateManager_01436e18. The local C2DExtentsI is the
//   shared engine/C2DExtentsI.h (pulled in by the render manager; X / Y). The camera, display and primitive-manager models are
//   unchanged (row debt). The primitive manager table 0x01436E84 is NEngineInternal::GPPrimitiveTypeInfo
//   (?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A, engine/CEnginePrimitiveTypeInfo.h PrimitiveManagers +0x10;
//   was a local CEnginePrimitiveManagerTable on g_pPrimitiveManagers). The sub-primitive renderer 0x01436E74 extern is respelled
//   NEngineInternal::GPSubPrimitiveRenderer (the spelling the landed 0x00BCA6B0 binds; was g_pPrimitiveRenderer_01436e74).
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
    float GetWidth() const { return BRX - TLX; }
    float GetHeight() const { return BRY - TLY; }
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
};
#include "engine/C3DVector.h"
struct CEngineCameraDesc {
    C2DBoxF    Window;               // +0x00
    CMatrix3x4 RotationMatrix;       // +0x10
    C3DVector  WorldPosition;        // +0x40
    float      HorizontalFOV;        // +0x4c
    float      VerticalFOV;          // +0x50
    bool       Use2DFOV;             // +0x54
    float      NearZPlane;           // +0x58
    float      FarZPlane;            // +0x5c
    float      ZBufferStart;         // +0x60
    float      ZBufferEnd;           // +0x64
    bool       StateValid;           // +0x68
    CEngineCameraDesc(const CEngineCameraDesc& other);   // 0x00B4AF50
};
class CEngineCamera {
public:
    unsigned char     _pad00[0x14];
    CEngineCameraDesc CameraDesc;    // +0x14
    void SetCameraState(const CEngineCameraDesc& desc, bool a, bool b);   // 0x00B30B50
};
namespace NEngineInternal { extern CEngineCamera* GPCamera; }   // 0x01436EA0

class CDisplayManager { public:
    unsigned char _pad000[0x1f4];
    C2DBoxI       Viewport;                           // +0x1f4
    const C2DExtentsI GetRenderTargetDimensions();          // 0x009BEDC0
    void SetViewport(const C2DBoxF& box);             // 0x009BF490
    void SetViewport(const C2DBoxI& box);             // 0x009BEF80
};
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A

class CEngineInternalPrimitiveBase;
class CEngineSubPrimitiveChain { public:
    void* First;
    void* Last;
    CEngineSubPrimitiveChain() : First(0), Last(0) {}
};
class CEnginePrimitiveManager {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void RenderPrimitive(CEngineInternalPrimitiveBase* primitive, CEngineSubPrimitiveChain& chain, long pass);   // slot 10 (+0x28)
};
#include "engine/CEnginePrimitiveTypeInfo.h"
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }   // 0x01436E84

class CEngineSubPrimitiveRenderer { public:
    void RenderType(long type, bool flag);         // 0x00B849F0
};
namespace NEngineInternal { extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer; }   // 0x01436E74 ?GPSubPrimitiveRenderer@NEngineInternal@@3PAVCEngineSubPrimitiveRenderer@@A

class CEngineInternalPrimitiveBase {
public:
    virtual void v00();
    unsigned char _pad04[4];
    long          Type;                            // +0x08
};
class CEnginePrimitiveHandle { public:
    long                          m_id;
    CEngineInternalPrimitiveBase* m_pPrimitive;
};
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
};
class CEngineInternalPrimitive2DViewport : public CEngineInternalPrimitiveBase {
public:
    unsigned char       _pad0c[0x48 - 0x0c];
    C2DBoxF             Viewport;                  // +0x48
    float               HorizontalFOV;             // +0x58
    float               VerticalFOV;               // +0x5c
    bool                Use2DFOV;                  // +0x60
    unsigned char       _pad61[0x94 - 0x61];
    CRetailHandleVector Primitives;                // +0x94
    void Render();
};

void CEngineInternalPrimitive2DViewport::Render()
{
    CEngineCameraDesc savedDesc(NEngineInternal::GPCamera->CameraDesc);
    {
        CEngineCameraDesc desc(savedDesc);
        desc.NearZPlane = 0.1f;
        desc.FarZPlane = 1.0f;
        desc.ZBufferStart = 0.0f;
        desc.ZBufferEnd = 0.1f;
        desc.Use2DFOV = Use2DFOV;
        desc.HorizontalFOV = HorizontalFOV;
        desc.VerticalFOV = VerticalFOV;
        desc.WorldPosition.X = 0.0f;
        desc.WorldPosition.Y = 0.0f;
        desc.WorldPosition.Z = 0.0f;
        desc.RotationMatrix.E43 = 0.0f;
        desc.RotationMatrix.E42 = 0.0f;
        desc.RotationMatrix.E41 = 0.0f;
        desc.RotationMatrix.E32 = 0.0f;
        desc.RotationMatrix.E31 = 0.0f;
        desc.RotationMatrix.E23 = 0.0f;
        desc.RotationMatrix.E21 = 0.0f;
        desc.RotationMatrix.E13 = 0.0f;
        desc.RotationMatrix.E12 = 0.0f;
        desc.RotationMatrix.E33 = 1.0f;
        desc.RotationMatrix.E11 = 1.0f;
        desc.RotationMatrix.E22 = -1.0f;
        NEngineInternal::GPCamera->SetCameraState(desc, false, true);
    }
    C2DBoxI oldViewport = NEngineInternal::GPDisplayManager->Viewport;
    C2DExtentsI dimensions = NEngineInternal::GPDisplayManager->GetRenderTargetDimensions();
    if (Viewport.GetWidth() > 0.0f && Viewport.GetHeight() > 0.0f) {
        CRenderStateScopedCaptureBlock capture_block;
        NEngineInternal::GPRenderManager->SetFogEnable(false);
        NEngineInternal::GPDisplayManager->SetViewport(Viewport);
        unsigned long count = Primitives.GetCount();
        CEngineSubPrimitiveChain chain;
        for (unsigned long i = 0; i < count; ++i) {
            const CEnginePrimitiveHandle& handle = Primitives.m_pFirst[i];
            if (handle.m_pPrimitive) {
                CEnginePrimitiveManager* manager = NEngineInternal::GPPrimitiveTypeInfo->PrimitiveManagers[handle.m_pPrimitive->Type];
                if (manager)
                    manager->RenderPrimitive(handle.m_pPrimitive, chain, 3);
            }
        }
        NEngineInternal::GPSubPrimitiveRenderer->RenderType(3, true);
        NEngineInternal::GPDisplayManager->SetViewport(oldViewport);
    }
    NEngineInternal::GPCamera->SetCameraState(savedDesc, false, true);
}