
// Leaf: CSymbolInfo>_>
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct Sub { char pad[32]; };
struct Owner { char pad[0x50]; };

extern "C" void __fastcall Sub_dtor_00414ea0(Sub* s);

void __fastcall _Dest_val(Owner* self)
{
    Sub_dtor_00414ea0((Sub*)((char*)self + 0x48));
    Sub_dtor_00414ea0((Sub*)((char*)self + 0x3c));
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}