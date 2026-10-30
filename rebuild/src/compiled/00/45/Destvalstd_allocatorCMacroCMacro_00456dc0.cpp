// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct MA { char pad[0x4c]; char a; };
// Leaf: allocator<CMacro>,CMacro>
struct MB { char pad[0x54-0x4c]; char b; };
struct MC { char pad[0x64-0x54]; char c; };
struct Obj {
    char base[0x4c];
    MA ma;   /* +0x4c */
};

extern "C" void __fastcall sub_A(void* p);   /* dtor for +0x64 */
extern "C" void __fastcall sub_B(void* p);   /* dtor for +0x54 */
extern "C" void __fastcall sub_C(void* p);   /* dtor for +0x4c */
 /* dtor for this */

void __fastcall Dest_val(char* self)
{
    sub_A(self + 0x64);
    sub_B(self + 0x54);
    sub_C(self + 0x4c);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}