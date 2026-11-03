// CEngine::SetMapVisibilityAllMaps(bool visible) -- show or hide every map (a CIEngine interface forwarder)
// Leaf: ?SetMapVisibilityAllMaps@CEngine@@UAEX_N@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23E50, length 11 bytes: `mov ecx,[0x01436E8C] ; jmp 0x00B423B0`.
// Identity: slot 46 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetMapVisibilityAllMaps@CEngine@@UAEX_N@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineWorldMap::SetMapVisibilityAllMaps @ 0x00B423B0, Ego_r + FableWin ?SetMapVisibilityAllMaps@CEngineWorldMap@@QAEX_N@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E8C (CEngineWorldMap*) under its landed spelling g_pEngineWorldMap; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineWorldMap { public:
    void SetMapVisibilityAllMaps(bool visible);   // 0x00B423B0
};
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

class CEngine { public:                      // vtable 0x012A0F3C, slot 46
    virtual void SetMapVisibilityAllMaps(bool visible);
};

void CEngine::SetMapVisibilityAllMaps(bool visible)
{
    g_pEngineWorldMap->SetMapVisibilityAllMaps(visible);
}