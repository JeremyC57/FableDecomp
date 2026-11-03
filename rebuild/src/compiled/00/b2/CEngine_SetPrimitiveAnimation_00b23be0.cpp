// CEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& animation) -- set a primitive's animation through the primitive renderer (a CIEngine interface forwarder)
// Leaf: ?SetPrimitiveAnimation@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23BE0, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B31E90`.
// Identity: slot 24 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetPrimitiveAnimation@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::SetPrimitiveAnimation @ 0x00B31E90, Ego_r + FableWin ?SetPrimitiveAnimation@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z;
//   its `ret 8h` pops the same 2 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pEnginePrimitiveRenderer; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CEnginePrimitive;
class CEnginePrimitiveHandle;
class CEnginePrimitiveAnimation;

class CEnginePrimitiveRenderer { public:
    void SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& animation);   // 0x00B31E90
};
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 24
    virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& animation);
};

void CEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle& handle, const CEnginePrimitiveAnimation& animation)
{
    g_pEnginePrimitiveRenderer->SetPrimitiveAnimation(handle, animation);
}