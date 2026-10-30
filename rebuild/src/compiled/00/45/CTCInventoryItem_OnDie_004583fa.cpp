#pragma optimize("s",on)
// Leaf: OnDie
// CTCInventoryItem::OnDie @ 004583fa
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CTCInventoryItem { public:
    char pad[0x44];
    void* f44;
};

extern "C" void my_free(void* p);                        // cdecl free @ 0xbfea14

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->f44)
        my_free(self->f44);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}