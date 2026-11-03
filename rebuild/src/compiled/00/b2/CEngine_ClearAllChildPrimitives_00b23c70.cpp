// CEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle& handle) -- clear every child primitive of a handle (a CIEngine interface forwarder)
// Leaf: ?ClearAllChildPrimitives@CEngine@@UAEXAAVCEnginePrimitiveHandle@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23C70, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B31FC0`.
// Identity: slot 31 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?ClearAllChildPrimitives@CEngine@@UAEXAAVCEnginePrimitiveHandle@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::ClearAllChildPrimitives @ 0x00B31FC0, Ego_r + FableWin ?ClearAllChildPrimitives@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pEnginePrimitiveRenderer; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CEnginePrimitive;
class CEnginePrimitiveHandle;

class CEnginePrimitiveRenderer { public:
    void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle);   // 0x00B31FC0
};
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 31
    virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle);
};

void CEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle& handle)
{
    g_pEnginePrimitiveRenderer->ClearAllChildPrimitives(handle);
}