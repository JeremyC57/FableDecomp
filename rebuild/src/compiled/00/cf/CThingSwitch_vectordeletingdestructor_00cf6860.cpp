#pragma optimize("t", on)
// Leaf: ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Naming wave 28 (family sfb): the base destructor is ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0 (chunk 696: the ctor 0x00CDD450 stores vtable
//   0x012C2F9C, whose ??_G 0x00CDD4A0 calls it; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor@@YIXPAX@Z stand-in.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall AutoTinySecondMemberDestructor(void* member);
class CSpawnedFuncBase { public: virtual ~CSpawnedFuncBase(); };   // 0x00CDD4C0 (vtable 0x012C2F9C)
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CScriptThing*)(static_cast<unsigned char*>(self) + 72))->CScriptThing::~CScriptThing();
    AutoTinySecondMemberDestructor(static_cast<unsigned char*>(self) + 52);
    ((CSpawnedFuncBase*)(self))->CSpawnedFuncBase::~CSpawnedFuncBase();
    if (flags & 1)
        operator delete(self);
    return self;
}