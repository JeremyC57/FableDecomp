#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall AutoTinyCompositeDestructor(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CScriptThing*)(static_cast<unsigned char*>(self) + 60))->CScriptThing::~CScriptThing();
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}