#pragma optimize("t", on)
// Leaf: ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 28 (family sfb): the base destructor is ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0 (chunk 696: the ctor 0x00CDD450 stores vtable
//   0x012C2F9C, whose ??_G 0x00CDD4A0 calls it; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor@@YIXPAX@Z stand-in.
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall AutoTinySecondMemberDestructor(void* member);
class CSpawnedFuncBase { public: virtual ~CSpawnedFuncBase(); };   // 0x00CDD4C0 (vtable 0x012C2F9C)
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 72)->CScriptThing::~CScriptThing();
    AutoTinySecondMemberDestructor(static_cast<unsigned char*>(self) + 52);
    ((CSpawnedFuncBase*)(self))->CSpawnedFuncBase::~CSpawnedFuncBase();
    if (flags & 1)
        operator delete(self);
    return self;
}