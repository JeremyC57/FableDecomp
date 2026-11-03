// CEngine::ReallocateShadowBuffer() -- ask the shadow renderer to rebuild its shadow buffer (an Xbox-only path; the PC callee is a bare `ret`) (a CIEngine interface forwarder)
// Leaf: ?ReallocateShadowBuffer@CEngine@@UAEXXZ
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23F80, length 11 bytes: `mov ecx,[0x01436E60] ; jmp 0x00B4B8A0`.
// Identity: slot 75 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?ReallocateShadowBuffer@CEngine@@UAEXXZ (the virtual CIEngine method, `this` never read).
// Callee: CEngineShadowRenderer::XBoxReallocateShadowBuffer @ 0x00B4B8A0, Ego_r + FableWin ?XBoxReallocateShadowBuffer@CEngineShadowRenderer@@QAEXXZ;
//   its `ret ` pops the same 0 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E60 (CEngineShadowRenderer*) under its landed spelling g_pShadowRenderer_01436e60; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineShadowRenderer { public:
    void XBoxReallocateShadowBuffer();   // 0x00B4B8A0
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;   // 0x01436E60

class CEngine { public:                      // vtable 0x012A0F3C, slot 75
    virtual void ReallocateShadowBuffer();
};

void CEngine::ReallocateShadowBuffer()
{
    g_pShadowRenderer_01436e60->XBoxReallocateShadowBuffer();
}
