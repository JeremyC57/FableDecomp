// CEngine::AddCustomMorphTexture(CTexture& texture, const CCharString& name) const -- register a custom morph texture with the surface-composition manager (a CIEngine interface forwarder)
// Leaf: ?AddCustomMorphTexture@CEngine@@UBEKAAVCTexture@@ABVCCharString@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23CE0, length 11 bytes: `mov ecx,[0x01436EAC] ; jmp 0x00B2C4E0`.
// Identity: slot 36 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?AddCustomMorphTexture@CEngine@@UBEKAAVCTexture@@ABVCCharString@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineSurfaceCompositionManager::AddCustomMorphTexture @ 0x00B2C4E0, Ego_r + FableWin ?AddCustomMorphTexture@CEngineSurfaceCompositionManager@@QAEKAAVCTexture@@ABVCCharString@@@Z;
//   its `ret 8h` pops the same 2 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436EAC (CEngineSurfaceCompositionManager*) under its landed spelling g_pEngineSurfaceCompositionManager; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CTexture;
class CCharString;

class CEngineSurfaceCompositionManager { public:
    unsigned long AddCustomMorphTexture(CTexture& texture, const CCharString& name);   // 0x00B2C4E0
};
extern CEngineSurfaceCompositionManager* g_pEngineSurfaceCompositionManager;   // 0x01436EAC

class CEngine { public:                      // vtable 0x012A0F3C, slot 36
    virtual unsigned long AddCustomMorphTexture(CTexture& texture, const CCharString& name) const;
};

unsigned long CEngine::AddCustomMorphTexture(CTexture& texture, const CCharString& name) const
{
    return g_pEngineSurfaceCompositionManager->AddCustomMorphTexture(texture, name);
}