#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
// Naming wave 28 (family esb): the base destructor is ??1CEntityScriptBase@@UAE@XZ 0x00F35B40 (chunk 696: stores vtable 0x012C3224, the
//   engine/CEntityScriptBase.h class; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z stand-in.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall AutoTinySecondMemberDestructor_008ac970(void* member);
class CEntityScriptBase { public: virtual ~CEntityScriptBase(); };   // 0x00F35B40 (vtable 0x012C3224)
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 40)->CScriptThing::~CScriptThing();
    AutoTinySecondMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 28);
    ((CEntityScriptBase*)(self))->CEntityScriptBase::~CEntityScriptBase();
    if (flags & 1)
        operator delete(self);
    return self;
}