#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall AutoTinySecondMemberDestructor_008ac970(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 100)->CScriptThing::~CScriptThing();
    AutoTinySecondMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 76);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}