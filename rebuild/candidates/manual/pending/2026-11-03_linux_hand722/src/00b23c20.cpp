// CEngine::AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags) -- add a child primitive under a handle and return its child index (a CIEngine interface forwarder)
// Leaf: ?AddChildPrimitive@CEngine@@UAEKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23C20, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B31EF0`.
// Identity: slot 26 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?AddChildPrimitive@CEngine@@UAEKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::AddChildPrimitive @ 0x00B31EF0, Ego_r + FableWin ?AddChildPrimitive@CEnginePrimitiveRenderer@@QAEKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z;
//   its `ret ch` pops the same 3 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pPrimitiveRenderer_01436e80; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CEnginePrimitive;
class CEnginePrimitiveHandle;

class CEnginePrimitiveRenderer { public:
    unsigned long AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);   // 0x00B31EF0
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 26
    virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);
};

unsigned long CEngine::AddChildPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags)
{
    return g_pPrimitiveRenderer_01436e80->AddChildPrimitive(handle, primitive, flags);
}
