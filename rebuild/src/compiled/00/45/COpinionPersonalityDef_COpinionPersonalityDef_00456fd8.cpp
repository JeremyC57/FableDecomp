#pragma optimize("s",on)
// ~COpinionPersonalityDef
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
void operator delete[](void*);   // call 0x7a7b44 (operator delete / free, __cdecl 1-arg)
extern void __fastcall member_dtor(void* self);    // dtor of member at +0x44 (__fastcall)
extern const int g_vtbl_01233f8c;                            // 0x1233f8c vtable

class COpinionPersonalityDef { public:
    void* vtbl;        // +0x00
    char pad[0x3c-4];  // +0x04 .. +0x3b
    void* ptr;         // +0x3c  freed field
    char member[8];    // +0x44  subobject (dtor'd)
};

void __fastcall COpinionPersonalityDef_dtor(COpinionPersonalityDef* self)
{
    self->vtbl = (void*)&g_vtbl_01233f8c;
    operator delete[](self->ptr);
    self->ptr = 0;
    member_dtor((char*)self + 0x44);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}