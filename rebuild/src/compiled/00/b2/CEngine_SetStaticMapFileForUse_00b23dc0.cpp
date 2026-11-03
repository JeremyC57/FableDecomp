// CEngine::SetStaticMapFileForUse(const CWideString& fileName) -- select the world map's static map file (a CIEngine interface forwarder)
// Leaf: ?SetStaticMapFileForUse@CEngine@@UAEXABVCWideString@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23DC0, length 11 bytes: `mov ecx,[0x01436E8C] ; jmp 0x00B428E0`.
// Identity: slot 52 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetStaticMapFileForUse@CEngine@@UAEXABVCWideString@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineWorldMap::SetStaticMapFileForUse @ 0x00B428E0, Ego_r + FableWin ?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E8C (CEngineWorldMap*) under its landed spelling g_pEngineWorldMap; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CWideString;

class CEngineWorldMap { public:
    void SetStaticMapFileForUse(const CWideString& fileName);   // 0x00B428E0
};
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

class CEngine { public:                      // vtable 0x012A0F3C, slot 52
    virtual void SetStaticMapFileForUse(const CWideString& fileName);
};

void CEngine::SetStaticMapFileForUse(const CWideString& fileName)
{
    g_pEngineWorldMap->SetStaticMapFileForUse(fileName);
}