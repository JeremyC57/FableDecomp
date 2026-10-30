#pragma optimize("s",on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct Member { int pad[16]; };
class COpinionReactionMaskDef { public:
    char pad[0x3c];
    Member m;
};

extern "C" void __fastcall Member_dtor(Member* def);

void* __fastcall COpinionReactionMaskDef_scalar_deleting_destructor(COpinionReactionMaskDef* def, int, unsigned int flags)
{
    Member_dtor(&def->m);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}