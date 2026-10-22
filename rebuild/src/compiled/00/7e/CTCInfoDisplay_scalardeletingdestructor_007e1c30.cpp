// CThingSwitch::`vector_deleting_destructor' @ 0x007E1C30 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x14 CCharString (~ @ 0x0099EAE0), +0x10 CCharString (~ @ 0x0099EAE0), then the base CTCBase (~ @ 0x00686830), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
#pragma optimize("t", on)
struct CTCBase {                            // vtable 0x0125BE0C (.?AVCTCBase@@)
    void* _vptr;
    ~CTCBase();                             // 0x00686830 (stores the vtable, tail-jumps ~CBaseClassNonCopyable)
};
struct CCharString {                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_007e1c30 : CTCBase {
    unsigned char _pad10[0xc];
    CCharString Member10;                       // +0x10
    CCharString Member14;                       // +0x14
};
#pragma pack(pop)

void* __fastcall CTCInfoDisplay_CompositeScalarDeletingDestructor(void* display, int, unsigned int flags)
{
    CObject_007e1c30* object = static_cast<CObject_007e1c30*>(display);
    object->Member14.~CCharString();
    object->Member10.~CCharString();
    object->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(object);
    return display;
}
