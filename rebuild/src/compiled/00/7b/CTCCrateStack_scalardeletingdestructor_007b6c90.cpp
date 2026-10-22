// CThingSwitch::`vector_deleting_destructor' @ 0x007B6C90 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x14 CBaseIntelligentPointer (~ @ 0x00A01C10), +0xc CBaseIntelligentPointer (~ @ 0x00A01C10), then the base CTCBase (~ @ 0x00686830), and frees the block with operator delete
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
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_007b6c90 : CTCBase {
    unsigned char _padc[0x8];
    CBaseIntelligentPointer Memberc;            // +0xc
    CBaseIntelligentPointer Member14;           // +0x14
};
#pragma pack(pop)

void* __fastcall CTCCrateStack_CompositeScalarDeletingDestructor(void* stack, int, unsigned int flags)
{
    CObject_007b6c90* object = static_cast<CObject_007b6c90*>(stack);
    object->Member14.~CBaseIntelligentPointer();
    object->Memberc.~CBaseIntelligentPointer();
    object->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(object);
    return stack;
}
