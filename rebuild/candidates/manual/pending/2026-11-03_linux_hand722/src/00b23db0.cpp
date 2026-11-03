// CEngine::CloseStaticMapFile() -- close the world map's static map file (a CIEngine interface forwarder)
// Leaf: ?CloseStaticMapFile@CEngine@@UAEXXZ
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23DB0, length 11 bytes: `mov ecx,[0x01436E8C] ; jmp 0x00B40000`.
// Identity: slot 41 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?CloseStaticMapFile@CEngine@@UAEXXZ (the virtual CIEngine method, `this` never read).
// Callee: CEngineWorldMap::CloseStaticMapFile @ 0x00B40000, Ego_r + FableWin ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ;
//   its `ret ` pops the same 0 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E8C (CEngineWorldMap*) under its landed spelling g_pEngineWorldMap; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineWorldMap { public:
    void CloseStaticMapFile();   // 0x00B40000
};
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

class CEngine { public:                      // vtable 0x012A0F3C, slot 41
    virtual void CloseStaticMapFile();
};

void CEngine::CloseStaticMapFile()
{
    g_pEngineWorldMap->CloseStaticMapFile();
}
