// CEnginePrimitiveManagerParticleDecalGroup::RenderMeshDecalStencilClear(const C3DVector& minimum, const C3DVector& maximum) -- apply the stencil-clear vertex and pixel shaders, set nine tracked render states (stencil +0x2BC4/+0x2BB0/+0x2B9C from 0x01396F3C, +0x2BD8 from 0x01396E34, +0x2840/+0x2868/+0x28B8/+0x28CC to 0, +0x2818 from 0x01396F9C), build the eight corners of the box and draw them as twelve indexed triangles after the shader manager's PrepareForRender(0)
// Leaf: RenderMeshDecalStencilClear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB8090, length 1408 bytes, `ret 8`.
// Callees: CShaderRenderManager::ApplyVertexShader @ 0x00988020, ApplyPixelShader @ 0x00988140, the inlined
//   PrepareForRender of the landed CEngineWeatherRenderer::RenderSnowBox 0x00B54210 (SetVSConstants @ 0x0098B930,
//   UpdatePixelShader @ 0x00988A20, UpdatePixelShaderConstants @ 0x00989BF0, UpdateAmbient @ 0x00989760, UpdateFogColour
//   @ 0x009897C0, UpdateLightGlobals @ 0x0098A760, ResetWorldTransform @ 0x00988290, UpdateCombinedProjectionTransform
//   @ 0x00988A50), CRenderManagerCore::DrawAsIndexedTris @ 0x00A0AF40 with the box index table @ 0x0139B488.
//   Render core 0x01436E18: the model of the landed SetupStencilMask 0x00BF1030 with the `__forceinline` SetState.
// Shape: SetState reads the capture mask into a named local: with the plain landed form VC7.1 commutes `test/or` (state
//   mask vs capture mask) from the eighth expansion on, retail only from the ninth; the extra local moves that flip one
//   block later.
// Layout: engine/CEnginePrimitiveManagerParticleDecalGroup.h (MeshDecalStencilClearVertexShader +0x64,
//   MeshDecalStencilClearPixelShader +0x8c).
#include "engine/CEnginePrimitiveManagerParticleDecalGroup.h"

class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W; };
class CVertexShader;
class CPixelShader;

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

class CShaderConstantRange { public:
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};

class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CShaderRenderManager {
public:
    void ApplyVertexShader(CVertexShader* shader, bool force);   // 0x00988020
    void ApplyPixelShader(CPixelShader* shader);                 // 0x00988140

    __forceinline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];                       // +0x008
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];                       // +0x1e9
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
public:
    void ResetWorldTransform();   // 0x00988290
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
struct CRenderManagerCoreView {                    // the render manager core @ 0x01436E18
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    CTrackedRenderState  m_state2818;               // +0x2818
    unsigned char        _pad282c[0x2840 - 0x282c];
    CTrackedRenderState  m_state2840;               // +0x2840
    unsigned char        _pad2854[0x2868 - 0x2854];
    CTrackedRenderState  m_state2868;               // +0x2868
    unsigned char        _pad287c[0x28b8 - 0x287c];
    CTrackedRenderState  m_state28b8;               // +0x28B8
    CTrackedRenderState  m_state28cc;               // +0x28CC
    unsigned char        _pad28e0[0x2b9c - 0x28e0];
    CTrackedRenderState  m_state2b9c;               // +0x2B9C
    CTrackedRenderState  m_state2bb0;               // +0x2BB0
    CTrackedRenderState  m_state2bc4;               // +0x2BC4
    CTrackedRenderState  m_state2bd8;               // +0x2BD8
    void DrawAsIndexedTris(const void* vertices, long vertexStride, long vertexCount, const void* indices,
                           long triangleCount);   // 0x00A0AF40
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396f3c;    // 0x01396F3C
extern unsigned long g_renderStateDefault_01396e34;    // 0x01396E34
extern unsigned long g_renderStateDefault_01396f9c;    // 0x01396F9C

static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
{
    if (state.m_value != value) {
        unsigned long mask = core->m_captureMask;
        if ((mask & state.m_captureMask) == 0) {
            state.m_captureMask |= mask;
            core->m_captured[core->m_capturedCount].m_pState = &state;
            core->m_captured[core->m_capturedCount].m_priorValue = state.m_value;
            ++core->m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            core->m_dirty[core->m_dirtyCount] = &state;
            ++core->m_dirtyCount;
        }
    }
}

extern unsigned short g_boxTriangleIndices[];        // 0x0139B488

struct CEnginePrimitiveManagerParticleDecalGroupMethods : CEnginePrimitiveManagerParticleDecalGroup {
    void RenderMeshDecalStencilClear(const C3DVector& minimum, const C3DVector& maximum);
};

void CEnginePrimitiveManagerParticleDecalGroupMethods::RenderMeshDecalStencilClear(const C3DVector& minimum,
                                                                                const C3DVector& maximum)
{
    g_pShaderRenderManager->ApplyVertexShader((CVertexShader*)MeshDecalStencilClearVertexShader, false);
    g_pShaderRenderManager->ApplyPixelShader((CPixelShader*)MeshDecalStencilClearPixelShader);
    SetState(g_pRenderManagerCore->m_state2bc4, g_renderStateDefault_01396f3c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2bb0, g_renderStateDefault_01396f3c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2b9c, g_renderStateDefault_01396f3c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2bd8, g_renderStateDefault_01396e34, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2840, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2868, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28b8, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28cc, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2818, g_renderStateDefault_01396f9c, g_pRenderManagerCore);
    C3DVector corners[8] = {
        { minimum.X, maximum.Y, maximum.Z }, { maximum.X, maximum.Y, maximum.Z },
        { minimum.X, minimum.Y, maximum.Z }, { maximum.X, minimum.Y, maximum.Z },
        { minimum.X, maximum.Y, minimum.Z }, { maximum.X, maximum.Y, minimum.Z },
        { minimum.X, minimum.Y, minimum.Z }, { maximum.X, minimum.Y, minimum.Z },
    };
    g_pShaderRenderManager->PrepareForRender(0);
    g_pRenderManagerCore->DrawAsIndexedTris(corners, sizeof(C3DVector), 8, g_boxTriangleIndices, 12);
}