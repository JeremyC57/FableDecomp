// _unattributed::NamedMeshVolumeScalarDeletingDestructor(unsigned int) -- `scalar deleting destructor' of a 16-byte {CCharString key; C3DMeshVolume value} record: release the volume's counted pointer, destroy the volume name, then the key, free when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F9C0, length 93 bytes.
// Leaf: ??_G
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pData->Data, m_pInfo->Info are the header CCountedPointer's (same offsets).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x2), operator delete @ 0x00BFE9BC (block and record),
// the counted block's delete function through `call [eax+4]` with its Data in ecx.
// Identity: the manifest label "CPSCEmitterGeneric::`vector_deleting_destructor'" is propagated and rejected
// (CPSCEmitterGeneric is polymorphic and 0x50 bytes; this destructor stores no vtable and only tests bit 0).
// The body has no callers and no vtable reference. The member at +4 is destroyed like C3DMeshVolume
// (PDB: CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; landed constructor
// 0x00A0A3D0) and the only PDB record of this shape is pair<CCharString, C3DMeshVolume> (16 bytes); a
// non-polymorphic class deleted with `delete` gets the non-virtual `??_G`, so the record is modelled with
// a `delete` helper. Same release shape as the landed 0x00AF69D0 (counted pointer cleared Data, Info
// after the release; the block pointer re-read after the decrement).
#include "rebuild_abi.h"

void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString { public:
    char* m_pString;
    ~CCharString();                                     // 0x0099EAE0
};

#include "engine/CCountedPointer.h"


class CContainmentVolume;

class C3DMeshVolume { public:                                  // 12 bytes
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

struct CNamedMeshVolume {                               // pair<CCharString, C3DMeshVolume>, 16 bytes, no vtable
    CCharString   first;                                // +0x00
    C3DMeshVolume second;                               // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CNamedMeshVolume) == 0x10);

void DestroyNamedMeshVolume(CNamedMeshVolume* record)
{
    delete record;
}