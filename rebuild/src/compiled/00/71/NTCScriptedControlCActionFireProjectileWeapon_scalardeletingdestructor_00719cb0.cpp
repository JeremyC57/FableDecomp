#pragma optimize("t", on)
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
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