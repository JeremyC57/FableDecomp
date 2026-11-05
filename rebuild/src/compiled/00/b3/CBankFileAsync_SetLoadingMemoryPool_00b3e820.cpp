// CBankFileAsync::SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool) -- adopt the counted pointer into LoadingMemoryPool (the parameter copy is released on return)
// Leaf: ?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b3e820, 106 bytes, thiscall, `ret 8` (the 8-byte counted pointer by value).
// Identity (proven): Ego_r.pdb ?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z; CEngineResourceManager's constructor 0x00B3CB30 passes its LoadingMemoryPool (Ego_r.pdb, the first pool) to the banks at +0x1C etc., and CEngineWorldMap::OpenStaticMaps 0x00B42750 to its static maps (CStaticMapBankFile : CBankFileAsync); bound in reloc_bindings as the stand-in ?SetMemoryPool@CStaticMapBankFile@@.
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb LoadingMemoryPool at +0x118.
// Header debt: engine/CBankFileAsync.h is a flat layout without methods; modelled locally.
#include "engine/CCountedPointer.h"
class CMovableResourceMemoryPool;
class CBankFileAsync {
public:
    unsigned char _pad00[0x118];
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;                          // +0x118
    void SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool);
};
void CBankFileAsync::SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool)
{
    LoadingMemoryPool = pool;
}