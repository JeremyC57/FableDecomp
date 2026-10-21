// CEngineSkyRenderer::RenderSky(float, const C3DVector&, bool) -- render the sky: clear the tracked render state at +0x2854, draw the sun and moon, bind the sky vertex/index buffers, draw the outer sky and the base band, the clouds when enabled, then unbind stream 0 and the index buffer
// Leaf: RenderSky
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B66190, length 352 bytes (with int3 padding; body 0x154), `ret 0Ch`.
// Callees: RenderSun(float, const C3DVector&) @ 0x00B63C00, RenderMoon @ 0x00B640E0, RenderOuterSky(float) @ 0x00B65A20,
//   RenderBaseBand() @ 0x00B63010, RenderClouds(float) @ 0x00B64FA0 (manifest names, same TU); IDirect3DDevice9::
//   SetStreamSource (slot 100, +0x190) and SetIndices (slot 104, +0x1A0) on the render core's PD3DDevice.
// Globals: the render manager core @ 0x01436E18 (PD3DDevice +0x3CF0, PSelectedIndexBuffer +0x3CF4, Ego_r.pdb offsets) whose
//   tracked render-state block (capture stack +0x08, dirty list +0x2008, counts +0x2808/+0x280C, capture mask +0x2814,
//   0x14-byte states) is the model of the landed CEngineLightingManager::EnableFog 0x00B46890; the state here is +0x2854.
// Layout: engine/CEngineSkyRenderer.h (VertexBuffer +0x58, IndexBuffer +0x8c, EnableClouds +0x18c); the buffers are read
//   through the PDB CVertexBufferBase (PVertexBuffer +4, Stride +0x10) / CIndexBuffer (PD3DIndexBuffer +4) views.
// Tracked state: `state.Set(v)` tests the value on the state and forwards to an inline manager ChangeState(state, v)
//   (core in eax, state in ecx, the manager mask in edx as retail); a pure state-side or manager-side setter swaps registers.
// Shape: two `__forceinline` render-core helpers taking the buffer pointer: SetVertexBuffer's null/non-null arms tail-merge
//   into one SetStreamSource call, SetIndexBuffer stores the selection then passes a ternary; called with 0 at the end they
//   fold to the all-zero arguments. The third parameter is unused.
#include "engine/CEngineSkyRenderer.h"

struct C3DVector { float X, Y, Z; };
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40(); virtual void __stdcall v44(); virtual void __stdcall v48(); virtual void __stdcall v4c();
    virtual void __stdcall v50(); virtual void __stdcall v54(); virtual void __stdcall v58(); virtual void __stdcall v5c();
    virtual void __stdcall v60(); virtual void __stdcall v64(); virtual void __stdcall v68(); virtual void __stdcall v6c();
    virtual void __stdcall v70(); virtual void __stdcall v74(); virtual void __stdcall v78(); virtual void __stdcall v7c();
    virtual void __stdcall v80(); virtual void __stdcall v84(); virtual void __stdcall v88(); virtual void __stdcall v8c();
    virtual void __stdcall v90(); virtual void __stdcall v94(); virtual void __stdcall v98(); virtual void __stdcall v9c();
    virtual void __stdcall va0(); virtual void __stdcall va4(); virtual void __stdcall va8(); virtual void __stdcall vac();
    virtual void __stdcall vb0(); virtual void __stdcall vb4(); virtual void __stdcall vb8(); virtual void __stdcall vbc();
    virtual void __stdcall vc0(); virtual void __stdcall vc4(); virtual void __stdcall vc8(); virtual void __stdcall vcc();
    virtual void __stdcall vd0(); virtual void __stdcall vd4(); virtual void __stdcall vd8(); virtual void __stdcall vdc();
    virtual void __stdcall ve0(); virtual void __stdcall ve4(); virtual void __stdcall ve8(); virtual void __stdcall vec();
    virtual void __stdcall vf0(); virtual void __stdcall vf4(); virtual void __stdcall vf8(); virtual void __stdcall vfc();
    virtual void __stdcall v100(); virtual void __stdcall v104(); virtual void __stdcall v108(); virtual void __stdcall v10c();
    virtual void __stdcall v110(); virtual void __stdcall v114(); virtual void __stdcall v118(); virtual void __stdcall v11c();
    virtual void __stdcall v120(); virtual void __stdcall v124(); virtual void __stdcall v128(); virtual void __stdcall v12c();
    virtual void __stdcall v130(); virtual void __stdcall v134(); virtual void __stdcall v138(); virtual void __stdcall v13c();
    virtual void __stdcall v140(); virtual void __stdcall v144(); virtual void __stdcall v148(); virtual void __stdcall v14c();
    virtual void __stdcall v150(); virtual void __stdcall v154(); virtual void __stdcall v158(); virtual void __stdcall v15c();
    virtual void __stdcall v160(); virtual void __stdcall v164(); virtual void __stdcall v168(); virtual void __stdcall v16c();
    virtual void __stdcall v170(); virtual void __stdcall v174(); virtual void __stdcall v178(); virtual void __stdcall v17c();
    virtual void __stdcall v180(); virtual void __stdcall v184(); virtual void __stdcall v188(); virtual void __stdcall v18c();
    virtual long __stdcall SetStreamSource(unsigned long stream, IDirect3DVertexBuffer9* data, unsigned long offset, unsigned long stride);   // slot 100 (+0x190)
    virtual void __stdcall v194(); virtual void __stdcall v198(); virtual void __stdcall v19c();
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indices);   // slot 104 (+0x1A0)
};

struct CVertexBufferBase {                         // PDB CVertexBufferBase view of CVertexBufferWin32
    void*                   _vptr;                 // +0x00
    IDirect3DVertexBuffer9* PVertexBuffer;         // +0x04
    unsigned long           Length;                // +0x08
    long                    Type;                  // +0x0c
    unsigned char           Stride;                // +0x10
};
struct CIndexBuffer {
    void*                  _vptr;                  // +0x00
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
};

struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
    __forceinline void Set(unsigned long value);
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
    unsigned char        _pad2818[0x2854 - 0x2818];
    CTrackedRenderState  m_state2854;              // +0x2854
    unsigned char        _pad2868[0x3cf0 - 0x2868];
    IDirect3DDevice9*    PD3DDevice;               // +0x3CF0
    CIndexBuffer*        PSelectedIndexBuffer;     // +0x3CF4

    __forceinline void ChangeState(CTrackedRenderState& state, unsigned long value)
    {
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
    __forceinline void SetVertexBuffer(CVertexBufferBase* buffer)
    {
        if (buffer)
            PD3DDevice->SetStreamSource(0, buffer->PVertexBuffer, 0, buffer->Stride);
        else
            PD3DDevice->SetStreamSource(0, 0, 0, 0);
    }
    __forceinline void SetIndexBuffer(CIndexBuffer* buffer)
    {
        PSelectedIndexBuffer = buffer;
        if (buffer)
            PD3DDevice->SetIndices(buffer->PD3DIndexBuffer);
        else
            PD3DDevice->SetIndices(0);
    }
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}

struct CEngineSkyRenderer_Methods : CEngineSkyRenderer {
    void RenderSun(float time, const C3DVector& position);    // 0x00B63C00
    void RenderMoon(float time, const C3DVector& position);   // 0x00B640E0
    void RenderOuterSky(float time);                           // 0x00B65A20
    void RenderBaseBand();                                     // 0x00B63010
    void RenderClouds(float time);                             // 0x00B64FA0
    void RenderSky(float time, const C3DVector& position, bool reflection);
};

void CEngineSkyRenderer_Methods::RenderSky(float time, const C3DVector& position, bool reflection)
{
    g_pRenderManagerCore->m_state2854.Set(0);
    RenderSun(time, position);
    RenderMoon(time, position);
    g_pRenderManagerCore->SetVertexBuffer((CVertexBufferBase*)VertexBuffer);
    g_pRenderManagerCore->SetIndexBuffer((CIndexBuffer*)IndexBuffer);
    RenderOuterSky(time);
    RenderBaseBand();
    if (EnableClouds)
        RenderClouds(time);
    g_pRenderManagerCore->SetVertexBuffer(0);
    g_pRenderManagerCore->SetIndexBuffer(0);
}