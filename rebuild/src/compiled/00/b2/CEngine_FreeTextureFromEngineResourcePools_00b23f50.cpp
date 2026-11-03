// CEngine::FreeTextureFromEngineResourcePools(CTexture* texture) -- return an outside texture to the resource manager's pools (a CIEngine interface forwarder)
// Leaf: ?FreeTextureFromEngineResourcePools@CEngine@@UAEXPAVCTexture@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23F50, length 11 bytes: `mov ecx,[0x01436E98] ; jmp 0x00B3B5B0`.
// Identity: slot 70 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?FreeTextureFromEngineResourcePools@CEngine@@UAEXPAVCTexture@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineResourceManager::FreeExternalTextureFromResourcePools @ 0x00B3B5B0, Ego_r + FableWin ?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E98 (CEngineResourceManager*) under its landed spelling g_pResourceManager; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CTexture;

class CEngineResourceManager { public:
    void FreeExternalTextureFromResourcePools(CTexture* texture);   // 0x00B3B5B0
};
extern CEngineResourceManager* g_pResourceManager;   // 0x01436E98

class CEngine { public:                      // vtable 0x012A0F3C, slot 70
    virtual void FreeTextureFromEngineResourcePools(CTexture* texture);
};

void CEngine::FreeTextureFromEngineResourcePools(CTexture* texture)
{
    g_pResourceManager->FreeExternalTextureFromResourcePools(texture);
}