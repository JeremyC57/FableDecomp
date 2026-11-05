// CMeshDataBank::SetClothMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool) -- adopt the counted pointer into ClothMemoryPool (the parameter copy is released on return)
// Leaf: ?SetClothMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a240e0, 106 bytes, thiscall, `ret 8` (the 8-byte counted pointer by value).
// Identity (proven): Ego_r.pdb ?SetClothMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z; the second of four adjacent setters 0x00A24050 / 0x00A240E0 / 0x00A24150 / 0x00A241E0 over Ego_r.pdb CMeshDataBank AnimationMemoryPool / ClothMemoryPool / MeshDataMemoryPool / XboxMemoryPool (retail +0x360/+0x368/+0x370/+0x378); callers CEngineResourceManager's constructor 0x00B3CB30 passing its ClothMemoryPool.
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb ClothMemoryPool at +0x368.
// Header debt: engine/CMeshDataBank.h is a flat layout without methods; modelled locally.
#include "engine/CCountedPointer.h"
class CMovableResourceMemoryPool;
class CMeshDataBank {
public:
    unsigned char _pad00[0x368];
    CCountedPointer<CMovableResourceMemoryPool> ClothMemoryPool;                          // +0x368
    void SetClothMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool);
};
void CMeshDataBank::SetClothMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool)
{
    ClothMemoryPool = pool;
}