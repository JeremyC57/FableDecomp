#pragma optimize("s",on)
// Leaf: OnDie
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CTCInventoryItem { public:
    unsigned char _pad_0x0[0x10];
    void* PInventory;
    unsigned char _pad_0x14[0xc];
    char DefaultItem;
    unsigned char _pad_0x21[0x5f];
    void* ptr80; // +0x80
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr80)
        free(self->ptr80);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}