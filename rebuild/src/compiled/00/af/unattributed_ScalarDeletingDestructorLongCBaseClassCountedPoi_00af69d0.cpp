// `scalar deleting destructor'(unsigned int) of a 0x10-byte record { long; CBaseClass; CCountedPointer } -- release the counted pointer, run ~CBaseClass on the member at +4, free when bit 0 of the flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF69D0, length 86 bytes.
// Callees: 0x00BFE9BC operator delete x2, 0x0099A2E0 CBaseClass::~CBaseClass (stores the CBaseClass vtable 0x01231710), CCPPointerInfo::DeleteFunc through the block
// The manifest label (C3DMeshFilePartitionChunk::vector_deleting_destructor) is propagated. The PDB shapes that fit
// (4 bytes, then a CScriptThing = CBaseClass + CCountedPointer<CScriptThing> PImp at +4, sizeof 0x10) are the
// script predicates CIsThingAlive / CIsThingConscious; the bytes cannot tell them apart and the member at +4
// stores no vtable of its own before ~CBaseClass, so it is modelled as the CBaseClass subobject followed by the
// counted pointer, destroyed as one member object. Landed as _unattributed; the leaf names the shape.
//
#include "rebuild_abi.h"

void __cdecl operator delete(void* p);                  // 0x00BFE9BC (retail CRT)
struct CCPPointerInfo {                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // called with Data in ecx and nothing on the stack
    void* Data;
};
template<class T> struct CCountedPointer {
    T*             m_pData;                              // PDB: Data
    CCPPointerInfo* m_pInfo;                             // PDB: Info
    CCountedPointer() : m_pData(0), m_pInfo(0) {}
    // The shared block is released through a re-read of m_pInfo after the decrement (the store through
    // the block pointer may alias the member), the block's delete function is called on its Data and
    // the block itself is freed with a plain (untested) operator delete.
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
    void Reset()       { ReleaseInfo(); m_pInfo = 0; m_pData = 0; }
};

struct CBaseClass {
    virtual void v0();
    CBaseClass();
    ~CBaseClass();                                        // 0x0099A2E0 (vtable store only)
};
struct CScriptThing : CBaseClass {                       // PDB: CBaseClass + CCountedPointer<CScriptThing> PImp (sizeof 0x0c)
    CCountedPointer<CScriptThing> PImp;                  // +0x04 of the member
};
struct CThingPredicateRecord {                           // non-polymorphic: no vtable store in the destructor
    long         m_key;                                  // +0x00
    CScriptThing Thing;                                  // +0x04 (destroyed through its own base pointer in esi)
};
FABLE_STATIC_ASSERT(sizeof(CThingPredicateRecord) == 0x10);
void DestroyThingPredicateRecord(CThingPredicateRecord* record)
{
    delete record;
}