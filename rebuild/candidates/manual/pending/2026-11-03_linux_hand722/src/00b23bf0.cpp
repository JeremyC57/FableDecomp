// CEngine::SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& position) -- set a primitive's position through the primitive renderer (a CIEngine interface forwarder)
// Leaf: ?SetPrimitivePosition@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23BF0, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B31EC0`.
// Identity: slot 25 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetPrimitivePosition@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::SetPrimitivePosition @ 0x00B31EC0, Ego_r + FableWin ?SetPrimitivePosition@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitivePosition@@@Z;
//   its `ret 8h` pops the same 2 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pPrimitiveRenderer_01436e80; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CEnginePrimitive;
class CEnginePrimitiveHandle;
class CEnginePrimitivePosition;

class CEnginePrimitiveRenderer { public:
    void SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& position);   // 0x00B31EC0
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 25
    virtual void SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& position);
};

void CEngine::SetPrimitivePosition(CEnginePrimitiveHandle& handle, const CEnginePrimitivePosition& position)
{
    g_pPrimitiveRenderer_01436e80->SetPrimitivePosition(handle, position);
}
