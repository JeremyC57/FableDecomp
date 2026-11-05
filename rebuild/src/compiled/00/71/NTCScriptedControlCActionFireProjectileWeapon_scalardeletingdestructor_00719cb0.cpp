// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
#pragma optimize("t", on)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall DestructFirstMember_0066a1c0(void* member);
extern void __fastcall CActionFireProjectileWeapon_Destruct(void* action);
void* __fastcall CActionFireProjectileWeapon_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember_0066a1c0(static_cast<unsigned char*>(action) + 24);
    ((CScriptThing*)(static_cast<unsigned char*>(action) + 12))->CScriptThing::~CScriptThing();
    CActionFireProjectileWeapon_Destruct(action);
    if (flags & 1)
        operator delete(action);
    return action;
}