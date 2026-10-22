// CEngineInternalPrimitive2DViewport::Render() -- render the viewport's child primitives through a 2D camera: switch the engine camera to an orthographic-style copy of its description (the viewport FOVs, near 0.1 / far 1, Z buffer 0..0.1, origin position, Y-flipped rotation); when the viewport box is non-empty open a render-state capture block, disable fog, set the display viewport, hand every child to its type's primitive manager on one sub-primitive chain, render the 2D sub-primitives, then restore the display viewport and the captured render states; finally restore the camera description
// Leaf: Render
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
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
    float GetWidth() const { return BRX - TLX; }
    float GetHeight() const { return BRY - TLY; }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};
struct C2DExtentsI { long Width, Height; };
struct CMatrix3x4 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
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
    void SetCameraDesc(const CEngineCameraDesc& desc, bool a, bool b);   // 0x00B30B50
};
extern CEngineCamera* g_pEngineCamera_01436ea0;   // 0x01436EA0

struct CDisplayManager {
    unsigned char _pad000[0x1f4];
    C2DBoxI       Viewport;                           // +0x1f4
    C2DExtentsI GetRenderTargetDimensions();          // 0x009BEDC0
    void SetViewport(const C2DBoxF& box);             // 0x009BF490
    void SetViewport(const C2DBoxI& box);             // 0x009BEF80
};
extern CDisplayManager* g_pDisplayManager_01436e1c;   // 0x01436E1C

struct SCaptureEntry { void* Block; unsigned long State; };
struct CCaptureStateManager {
    unsigned char _pad000[8];
    SCaptureEntry CaptureStack[0x500];             // +0x008
    unsigned long CaptureStackCount;               // +0x2808
    unsigned char _pad280c[8];
    unsigned long CaptureMask;                     // +0x2814
    unsigned char _pad2818[0x3a24 - 0x2818];
    unsigned char CaptureBlock[4];                 // +0x3a24
    void OpenCaptureBlock()
    {
        CaptureStack[CaptureStackCount++].Block = CaptureBlock;
        CaptureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CCaptureStateManager* g_pRenderStateManager;   // 0x013CB508

struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;
    unsigned long        m_priorValue;
};
struct CRenderStateManager {
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 AllStatesListSize;        // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2944 - 0x2818];
    CTrackedRenderState  m_fogEnable;              // +0x2944
    __forceinline void DisableFog() { SetState(m_fogEnable, 0); }
    __forceinline void SetState(CTrackedRenderState& state, unsigned long value)
    {
        if (state.m_value != value) {
            if ((m_captureMask & state.m_captureMask) == 0) {
                state.m_captureMask |= m_captureMask;
                m_captured[m_capturedCount].m_pState = &state;
                m_captured[m_capturedCount].m_priorValue = state.m_value;
                ++m_capturedCount;
            }
            state.m_value = value;
            if (!state.m_queued) {
                state.m_queued = true;
                m_dirty[m_dirtyCount] = &state;
                ++m_dirtyCount;
            }
        }
    }
};
extern CRenderStateManager* g_pRenderStateManager_01436e18;   // 0x01436E18

class CEngineInternalPrimitiveBase;
struct CEngineSubPrimitiveChain {
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
struct CEnginePrimitiveManagerTable {
    unsigned char            _pad00[0x10];
    CEnginePrimitiveManager* Managers[40];          // +0x10, by primitive type
};
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;   // 0x01436E84

struct CEngineSubPrimitiveRenderer {
    void RenderType(long type, bool flag);         // 0x00B849F0
};
extern CEngineSubPrimitiveRenderer* g_pPrimitiveRenderer_01436e74;   // 0x01436E74

class CEngineInternalPrimitiveBase {
public:
    virtual void v00();
    unsigned char _pad04[4];
    long          Type;                            // +0x08
};
struct CEnginePrimitiveHandle {
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
    CEngineCameraDesc savedDesc(g_pEngineCamera_01436ea0->CameraDesc);
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
        g_pEngineCamera_01436ea0->SetCameraDesc(desc, false, true);
    }
    C2DBoxI oldViewport = g_pDisplayManager_01436e1c->Viewport;
    C2DExtentsI dimensions = g_pDisplayManager_01436e1c->GetRenderTargetDimensions();
    if (Viewport.GetWidth() > 0.0f && Viewport.GetHeight() > 0.0f) {
        g_pRenderStateManager->OpenCaptureBlock();
        g_pRenderStateManager_01436e18->DisableFog();
        g_pDisplayManager_01436e1c->SetViewport(Viewport);
        unsigned long count = Primitives.GetCount();
        CEngineSubPrimitiveChain chain;
        for (unsigned long i = 0; i < count; ++i) {
            const CEnginePrimitiveHandle& handle = Primitives.m_pFirst[i];
            if (handle.m_pPrimitive) {
                CEnginePrimitiveManager* manager = g_pPrimitiveManagers->Managers[handle.m_pPrimitive->Type];
                if (manager)
                    manager->RenderPrimitive(handle.m_pPrimitive, chain, 3);
            }
        }
        g_pPrimitiveRenderer_01436e74->RenderType(3, true);
        g_pDisplayManager_01436e1c->SetViewport(oldViewport);
        g_pRenderStateManager->RestoreCaptureBlock();
    }
    g_pEngineCamera_01436ea0->SetCameraDesc(savedDesc, false, true);
}