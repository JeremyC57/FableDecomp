// CEngine::SetMapAsLoaded(long mapIndex) -- tell the world map that the game has loaded a map (a CIEngine interface forwarder)
// Leaf: ?SetMapAsLoaded@CEngine@@UAEXJ@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23E30, length 11 bytes: `mov ecx,[0x01436E8C] ; jmp 0x00B401E0`.
// Identity: slot 44 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetMapAsLoaded@CEngine@@UAEXJ@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineWorldMap::OnGameMapLoaded @ 0x00B401E0, Ego_r + FableWin ?OnGameMapLoaded@CEngineWorldMap@@QAEXJ@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E8C (CEngineWorldMap*) under its landed spelling g_pEngineWorldMap; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineWorldMap { public:
    void OnGameMapLoaded(long mapIndex);   // 0x00B401E0
};
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

class CEngine { public:                      // vtable 0x012A0F3C, slot 44
    virtual void SetMapAsLoaded(long mapIndex);
};

void CEngine::SetMapAsLoaded(long mapIndex)
{
    g_pEngineWorldMap->OnGameMapLoaded(mapIndex);
}