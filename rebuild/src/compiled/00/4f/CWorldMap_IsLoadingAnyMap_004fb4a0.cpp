// CWorldMap::IsLoadingAnyMap() const -- whether the level loader still has objects queued: PLevelLoader->IsBusy()
// Leaf: ?IsLoadingAnyMap@CWorldMap@@QBE_NXZ
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

template <class T> class CCountedPointer {
public:
    T*    Data;                                                         // +0x00
    void* Info;                                                         // +0x04
    T* operator->() const { return Data; }
};

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
