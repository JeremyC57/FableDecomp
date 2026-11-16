// CEngineSkyRenderer::RenderSky(float, const C3DVector&, bool) -- render the sky: clear the tracked render state at +0x2854, draw the sun and moon, bind the sky vertex/index buffers, draw the outer sky and the base band, the clouds when enabled, then unbind stream 0 and the index buffer
// Leaf: RenderSky
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore / CRenderStateManager at
//   offset 0): the shared inline setter ZBufferWriteEnable(false) (+0x2854) and the PDB inlines SelectVertexBuffer(0, buffer) /
//   SelectIndexBuffer(buffer, 0) on the shared CVertexBufferWin32 / CIndexBuffer / IDirect3DDevice9 (the sky's opaque VertexBuffer +0x58 /
//   IndexBuffer +0x8C members reinterpreted, as before). Was a local CRenderManagerCoreView / CTrackedRenderState model with SetVertexBuffer /
//   SetIndexBuffer, local IDirect3DDevice9 / CVertexBufferBase / CIndexBuffer and g_pRenderManagerCore.
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
#include "engine/CEngineSkyRenderer.h"

#include "engine/C3DVector.h"

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
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    RenderSun(time, position);
    RenderMoon(time, position);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, reinterpret_cast<CVertexBufferWin32*>(VertexBuffer));
    NEngineInternal::GPRenderManager->SelectIndexBuffer(reinterpret_cast<CIndexBuffer*>(IndexBuffer), 0);
    RenderOuterSky(time);
    RenderBaseBand();
    if (EnableClouds)
        RenderClouds(time);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, 0);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(0, 0);
}