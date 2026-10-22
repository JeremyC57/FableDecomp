// CMessageEvent::`scalar deleting destructor' @ 0x006872B0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x50 CBaseIntelligentPointer (~ @ 0x00A01C10), +0x48 CBaseIntelligentPointer (~ @ 0x00A01C10), then the base CBaseObject (~ @ 0x0099A430), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CBaseObject CBaseObjectPdb
#include "engine/CBaseObject.h"
#undef CBaseObject
#pragma optimize("t", on)
struct CBaseObject {                        // vtable 0x0129A7D8 (.?AVCBaseObject@@)
    void* _vptr;
    void* IntelligentPointer;
    ~CBaseObject();                         // 0x0099A430
};
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_006872b0 : CBaseObject {
    unsigned char _pad48[0x40];
    CBaseIntelligentPointer Member48;           // +0x48
    CBaseIntelligentPointer Member50;           // +0x50
};
#pragma pack(pop)

void* __fastcall CMessageEvent_CompositeScalarDeletingDestructor(void* message, int, unsigned int flags)
{
    CObject_006872b0* object = static_cast<CObject_006872b0*>(message);
    object->Member50.~CBaseIntelligentPointer();
    object->Member48.~CBaseIntelligentPointer();
    object->CBaseObject::~CBaseObject();
    if (flags & 1)
        operator delete(object);
    return message;
}
