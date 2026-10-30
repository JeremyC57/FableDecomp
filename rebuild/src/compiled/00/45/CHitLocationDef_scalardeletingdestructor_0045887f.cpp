#pragma optimize("s", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern void __fastcall DestructMember_0044f3b2(void* member);
void* __fastcall CHitLocationDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_0044f3b2(static_cast<unsigned char*>(def) + 60);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}