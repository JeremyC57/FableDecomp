// COwner_00782c80::`scalar deleting destructor'(unsigned int) -- non-virtual: the CMember_00a01c10 member at +0x8 destroyed out of line, flag-tested delete
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782C80, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; call CMember_00a01c10::~CMember_00a01c10 ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic class (no vtable store anywhere): VC7.1 emits a scalar deleting
// destructor for any class with a non-trivial destructor that a translation unit deletes. The
// implicit destructor is inlined: the CMember_00a01c10 member at +0x8 is destroyed through its
// out-of-line destructor @ 0x00A01C10, then bit 0 of the flag argument selects the flag-tested
// operator delete @ 0x00BFE9BC. The manifest label (`vector deleting destructor` of
// CLandscapeBackgroundPatch) is propagated across the family; the owner is named descriptively.
// One of 10 bodies of this shape.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct COwner_00782c80 {
    unsigned char _pad[0x8];
    CBaseIntelligentPointer m_08;            // +0x8
};

// The delete-expression that makes the compiler emit COwner_00782c80::`scalar deleting destructor'.
void Destroy_COwner_00782c80(COwner_00782c80* object)
{
    delete object;
}