#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor_007add50(void* member);
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_007add50(static_cast<unsigned char*>(self) + 88);
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 76)->CScriptThing::~CScriptThing();
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}