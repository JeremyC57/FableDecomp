// CEngineSubPrimitiveText::`scalar_deleting_destructor' @ 0x0073D770 (retail, 40 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: ?CEngineSubPrimitiveText_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x8 CBaseIntelligentPointer (~ @ 0x00A01C10), then +0x0 CBaseIntelligentPointer (~ @ 0x00A01C10), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_0073d770 {
    CBaseIntelligentPointer Member0;
    CBaseIntelligentPointer Member8;            // +0x8
};
#pragma pack(pop)

void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_0073d770* object = static_cast<CObject_0073d770*>(self);
    object->Member8.CBaseIntelligentPointer::~CBaseIntelligentPointer();
    object->Member0.CBaseIntelligentPointer::~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(object);
    return self;
}
