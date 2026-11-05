// `scalar deleting destructor'(unsigned int) of a 0x10-byte record { long; CBaseClass; CCountedPointer } -- release the counted pointer, run ~CBaseClass on the member at +4, free when bit 0 of the flag is set
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pData->Data, m_pInfo->Info are the header CCountedPointer's (same offsets).
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
#include "engine/CCountedPointer.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass { public:                       // PDB: CBaseClass + CCountedPointer<CScriptThing> PImp (sizeof 0x0c)
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