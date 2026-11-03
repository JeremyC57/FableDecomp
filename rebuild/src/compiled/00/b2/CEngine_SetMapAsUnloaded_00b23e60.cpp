// CEngine::SetMapAsUnloaded(long mapIndex) -- tell the world map that the game has unloaded a map (a CIEngine interface forwarder)
// Leaf: ?SetMapAsUnloaded@CEngine@@UAEXJ@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23E60, length 11 bytes: `mov ecx,[0x01436E8C] ; jmp 0x00B40230`.
// Identity: slot 45 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetMapAsUnloaded@CEngine@@UAEXJ@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineWorldMap::OnGameMapUnloaded @ 0x00B40230, Ego_r + FableWin ?OnGameMapUnloaded@CEngineWorldMap@@QAEXJ@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E8C (CEngineWorldMap*) under its landed spelling g_pEngineWorldMap; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineWorldMap { public:
    void OnGameMapUnloaded(long mapIndex);   // 0x00B40230
};
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

class CEngine { public:                      // vtable 0x012A0F3C, slot 45
    virtual void SetMapAsUnloaded(long mapIndex);
};

void CEngine::SetMapAsUnloaded(long mapIndex)
{
    g_pEngineWorldMap->OnGameMapUnloaded(mapIndex);
}