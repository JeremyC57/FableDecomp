// CEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle) -- fetch the handle of a child primitive (a CIEngine interface forwarder)
// Leaf: ?GetChildPrimitiveHandle@CEngine@@UAEXAAVCEnginePrimitiveHandle@@K0@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23C40, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B31F50`.
// Identity: slot 29 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?GetChildPrimitiveHandle@CEngine@@UAEXAAVCEnginePrimitiveHandle@@K0@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::GetChildPrimitiveHandle @ 0x00B31F50, Ego_r + FableWin ?GetChildPrimitiveHandle@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@K0@Z;
//   its `ret ch` pops the same 3 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pPrimitiveRenderer_01436e80; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CEnginePrimitive;
class CEnginePrimitiveHandle;

class CEnginePrimitiveRenderer { public:
    void GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle);   // 0x00B31F50
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 29
    virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle);
};

void CEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle, unsigned long childIndex, CEnginePrimitiveHandle& childHandle)
{
    g_pPrimitiveRenderer_01436e80->GetChildPrimitiveHandle(handle, childIndex, childHandle);
}
