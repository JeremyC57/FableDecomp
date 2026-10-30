#pragma optimize("s",on)
// Leaf: OnDie
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct SubA { char x; };
struct SubB { char x; };
#pragma pack(1)
class CTCInventoryItem { public:
    unsigned char _pad_0x0[0x3c];
    SubB sub3c;             // +0x3c
    unsigned char _pad_0x3d[0x13];
    SubA sub50;             // +0x50
    unsigned char _pad_0x51[0x1b];
    void* ptr6c;            // +0x6c
};
#pragma pack()
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
extern void __fastcall SubA_dtor(SubA* self);
extern void __fastcall SubB_dtor(SubB* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr6c)
        free(self->ptr6c);
    SubA_dtor(&self->sub50);
    SubB_dtor(&self->sub3c);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}