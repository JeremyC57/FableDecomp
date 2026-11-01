// CCountedPointer<CLevelLoader::CObject>::DeleteData(void* data) -- the counted pointer's per-type deleter: ~CObject (0x005195B0, out of line), then operator delete
// Leaf: ?DeleteData@?$CCountedPointer@VCObject@CLevelLoader@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051AF00, 24 bytes (static __fastcall: the object in ecx; plain `ret`).
// Identity (naming wave 20, chunk 688): CWorldMap::LoadLevelFromFiles 0x00501150 stores this address as the DeleteFunc of the CCPPointerInfo
//   it builds for `new CLevelLoader::CObject` (engine/CCountedPointer.h: DeleteFunc = CCountedPointer<T>::DeleteData, SIXPAX@Z); FableWin.pdb
//   and Ego_r.pdb both carry ?DeleteData@?$CCountedPointer@VCObject@CLevelLoader@@@@SIXPAX@Z. Was a CTCMapwho::OnDie stand-in (the catalog's
//   DeleteObject_CLevelLoader__CObject_0051af00 is descriptive).
// Callees: CLevelLoader::CObject::~CObject 0x005195B0 (landed), operator delete 0x00BFE9BC.
// Shape: `delete static_cast<T*>(data)` -- the null test, the out-of-line non-virtual destructor, operator delete.
#include "engine/CCountedPointer.h"

class CLevelLoader {
public:
    class CObject {
    public:
        ~CObject();                                    // 0x005195B0
    };
};

template void FABLE_FASTCALL CCountedPointer<CLevelLoader::CObject>::DeleteData(void*);
