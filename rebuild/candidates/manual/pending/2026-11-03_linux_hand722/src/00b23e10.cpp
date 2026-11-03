// CEngine::AddMap(long mapIndex, long mapUID) -- add a map to the world map (a CIEngine interface forwarder)
// Leaf: ?AddMap@CEngine@@UAEXJJ@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23E10, length 11 bytes: `mov ecx,[0x01436E8C] ; jmp 0x00B42D10`.
// Identity: slot 42 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?AddMap@CEngine@@UAEXJJ@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineWorldMap::AddMap @ 0x00B42D10, Ego_r + FableWin ?AddMap@CEngineWorldMap@@QAEXJJ@Z;
//   its `ret 8h` pops the same 2 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E8C (CEngineWorldMap*) under its landed spelling g_pEngineWorldMap; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineWorldMap { public:
    void AddMap(long mapIndex, long mapUID);   // 0x00B42D10
};
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

class CEngine { public:                      // vtable 0x012A0F3C, slot 42
    virtual void AddMap(long mapIndex, long mapUID);
};

void CEngine::AddMap(long mapIndex, long mapUID)
{
    g_pEngineWorldMap->AddMap(mapIndex, mapUID);
}
