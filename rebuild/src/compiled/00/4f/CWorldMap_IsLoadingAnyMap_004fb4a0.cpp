// CWorldMap::IsLoadingAnyMap() const -- whether the level loader still has objects queued: PLevelLoader->IsBusy()
// Leaf: ?IsLoadingAnyMap@CWorldMap@@QBE_NXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FB4A0, 11 bytes: mov ecx,[ecx+0BCh] ; jmp CLevelLoader::IsBusy (0x006C20A0)
// Identity (FINDINGS chunk 388 callee; was the stand-in `T::Run` forwarder catalogued as
//   _global::Forward_IsActionPointFree_via0xbc_004fb4a0 with a propagated _Cons_val manifest label): Ego_r.pdb / FableWin.pdb
//   name ?IsLoadingAnyMap@CWorldMap@@QBE_NXZ; struct_layouts CWorldMap {..., CCountedPointer<CLevelLoader> PLevelLoader}
//   (PDB +0xE0; retail +0xBC with the 12-byte STLport containers before it), whose Data word is loaded into ecx before
//   the tail jump to CLevelLoader::IsBusy (0x006C20A0, renamed in this pass); xrefs: callers are CWorld::UpdateRegionLoad
//   0x004A4CFB and the CTCDParticleEmitter::Create family 0x006E08A0 / 0x006E09CE / 0x006E1211 (which skip work while a
//   map is loading).
// Layout: the shared engine/CWorldMap.h carries the PDB offsets (PLevelLoader_Data +0xE0), so a local model is used.
class CLevelLoader {
public:
    bool IsBusy() const;                                                // 0x006C20A0
};

#include "engine/CCountedPointer.h"

class CWorldMap {
public:
    bool IsLoadingAnyMap() const;
protected:
    unsigned char _pad00[0xbc];
    CCountedPointer<CLevelLoader> PLevelLoader;                         // +0xBC (retail)
};

bool CWorldMap::IsLoadingAnyMap() const
{
    return PLevelLoader->IsBusy();
}
