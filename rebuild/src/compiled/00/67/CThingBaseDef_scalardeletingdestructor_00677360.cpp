#pragma optimize("t", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
// Naming (NF423 gen, 2026-11-25): the callee 0x00450BA0 now binds its PDB spelling ??1CThingComponentSet@@QAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?DestructMember_00450ba0@@YIXPAX@Z.
class CThingComponentSet { public: ~CThingComponentSet(); };
void* __fastcall CThingBaseDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CThingComponentSet*>(static_cast<unsigned char*>(def) + 57)->CThingComponentSet::~CThingComponentSet();
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}