// CEngine::AddPrimitive(const CEnginePrimitive& primitive) -- add a one-frame primitive through the primitive renderer (a CIEngine interface forwarder)
// Leaf: ?AddPrimitive@CEngine@@UAEXABVCEnginePrimitive@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23BB0, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B32E90`.
// Identity: slot 20 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?AddPrimitive@CEngine@@UAEXABVCEnginePrimitive@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::AddPrimitive @ 0x00B32E90, Ego_r + FableWin ?AddPrimitive@CEnginePrimitiveRenderer@@QAEXABVCEnginePrimitive@@@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pPrimitiveRenderer_01436e80; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CEnginePrimitive;

class CEnginePrimitiveRenderer { public:
    void AddPrimitive(const CEnginePrimitive& primitive);   // 0x00B32E90
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 20
    virtual void AddPrimitive(const CEnginePrimitive& primitive);
};

void CEngine::AddPrimitive(const CEnginePrimitive& primitive)
{
    g_pPrimitiveRenderer_01436e80->AddPrimitive(primitive);
}
