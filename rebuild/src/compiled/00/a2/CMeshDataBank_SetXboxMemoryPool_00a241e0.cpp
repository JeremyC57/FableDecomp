// CMeshDataBank::SetXboxMemoryPool(CCountedPointer<CXboxBufferMemoryPool> pool, long unused) -- adopt the counted pointer into XboxMemoryPool (the parameter copy is released on return)
// Leaf: ?SetXboxMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@VCXboxBufferMemoryPool@@@@J@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a241e0, 106 bytes, thiscall, `ret 0Ch` (the 8-byte counted pointer by value).
// Identity (descriptive): the fourth of the four adjacent CMeshDataBank pool setters (0x00A24050 / 0x00A240E0 = SetClothMemoryPool (Ego_r.pdb) / 0x00A24150 / 0x00A241E0), storing Ego_r.pdb CMeshDataBank::XboxMemoryPool (CCountedPointer<CXboxBufferMemoryPool>, retail +0x378); no caller (the Xbox pool is unused on PC); it pops a second, unread 4-byte argument (`ret 0Ch`); the method name and that parameter are descriptive (only SetClothMemoryPool is in the PDB name sets).
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb XboxMemoryPool at +0x378.
// Header debt: engine/CMeshDataBank.h is a flat layout without methods; modelled locally.
#include "engine/CCountedPointer.h"
class CXboxBufferMemoryPool;
class CMeshDataBank {
public:
    unsigned char _pad00[0x378];
    CCountedPointer<CXboxBufferMemoryPool> XboxMemoryPool;                          // +0x378
    void SetXboxMemoryPool(CCountedPointer<CXboxBufferMemoryPool> pool, long unused);
};
void CMeshDataBank::SetXboxMemoryPool(CCountedPointer<CXboxBufferMemoryPool> pool, long unused)
{
    XboxMemoryPool = pool;
}