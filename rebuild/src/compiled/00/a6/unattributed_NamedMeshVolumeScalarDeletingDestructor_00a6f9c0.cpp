// _unattributed::NamedMeshVolumeScalarDeletingDestructor(unsigned int) -- `scalar deleting destructor' of a 16-byte {CCharString key; C3DMeshVolume value} record: release the volume's counted pointer, destroy the volume name, then the key, free when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F9C0, length 93 bytes.
// Leaf: ??_G
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

struct CCharString {
    char* m_pString;
    ~CCharString();                                     // 0x0099EAE0
};

struct CCPPointerInfo {                                 // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // called with Data in ecx and nothing on the stack
    void* Data;
};

template<class T> struct CCountedPointer {
    T*              m_pData;                            // PDB: Data
    CCPPointerInfo* m_pInfo;                            // PDB: Info
    void ReleaseInfo()
    {
        if (m_pInfo) {
            --m_pInfo->RefCount;
            if (m_pInfo->RefCount == 0) {
                m_pInfo->DeleteFunc(m_pInfo->Data);
                delete m_pInfo;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); m_pData = 0; m_pInfo = 0; }
};

struct CContainmentVolume;

struct C3DMeshVolume {                                  // 12 bytes
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