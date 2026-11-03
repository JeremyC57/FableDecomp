// CEngine::DeallocateShadowBuffer() -- ask the shadow renderer to free its shadow buffer (an Xbox-only path; the PC callee returns false) (a CIEngine interface forwarder)
// Leaf: ?DeallocateShadowBuffer@CEngine@@UAE_NXZ
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23F70, length 11 bytes: `mov ecx,[0x01436E60] ; jmp 0x00B4B890`.
// Identity: slot 74 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?DeallocateShadowBuffer@CEngine@@UAE_NXZ (the virtual CIEngine method, `this` never read).
// Callee: CEngineShadowRenderer::XBoxDeallocateShadowBuffer @ 0x00B4B890, Ego_r + FableWin ?XBoxDeallocateShadowBuffer@CEngineShadowRenderer@@QAE_NXZ;
//   its `ret ` pops the same 0 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E60 (CEngineShadowRenderer*) under its landed spelling g_pShadowRenderer_01436e60; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineShadowRenderer { public:
    bool XBoxDeallocateShadowBuffer();   // 0x00B4B890
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;   // 0x01436E60

class CEngine { public:                      // vtable 0x012A0F3C, slot 74
    virtual bool DeallocateShadowBuffer();
};

bool CEngine::DeallocateShadowBuffer()
{
    return g_pShadowRenderer_01436e60->XBoxDeallocateShadowBuffer();
}
