// CEventBase_Derived_00811d10::`scalar deleting destructor'(unsigned int) -- the list member at +0x8 cleared out of line (@ 0x00A01C10), the CEventBase vtable stored by the inlined base destructor, flag-tested delete
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811D10, oracle length 39 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset CEventBase::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a CEventBase-derived class that adds one list member: the member's
// out-of-line destructor @ 0x00A01C10 runs, the inlined base destructor stores the base vtable
// (0x0124F2C4, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x00BFE9BC. The manifest label is propagated across the family; the derived class
// is named by its base. One of 14 bodies of this shape.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CEventBase {
    virtual ~CEventBase() { }
    unsigned char _body[0x4];
};

struct CEventBase_Derived_00811d10 : CEventBase {
    CBaseIntelligentPointer m_08;        // +0x8
};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
CEventBase_Derived_00811d10* Create_CEventBase_Derived_00811d10()
{
    return new CEventBase_Derived_00811d10;
}

void Destroy_CEventBase_Derived_00811d10(CEventBase_Derived_00811d10* object)
{
    delete object;
}