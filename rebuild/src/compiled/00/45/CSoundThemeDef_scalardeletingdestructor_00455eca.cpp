#pragma optimize("s",on)
// scalar deleting destructor for CSoundThemeDef
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CSoundThemeDef { public: char pad[0x58]; };

extern "C" void __fastcall Sub54_dtor(void* ecx);
void operator delete(void* p);

void* __fastcall CSoundThemeDef_scalar_deleting_destructor(CSoundThemeDef* def, int edx, unsigned int flags)
{
    (void)edx;
    Sub54_dtor((char*)def + 0x54);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}