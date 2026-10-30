#pragma optimize("s",on)
// Leaf: OnDie
#include "rebuild_abi.h"

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CTCInventoryItem
{ public:
    char pad0[0x54];
    void* field_54; // +0x54
};

// __cdecl free-style cleanup on the +0x54 pointer (one stack arg, caller-cleaned via pop ecx)
extern "C" void __cdecl free_field_54(void* p);

// base-class OnDie, tail-called with this in ecx (__fastcall modeled as __fastcall)

// CTCInventoryItem::OnDie  (retail 0x004588e2)
void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* thisptr)
{
    void* p = thisptr->field_54;
    if (p != 0)
    {
        free_field_54(p);
    }
    ((CParentDefClassBase*)(thisptr))->CParentDefClassBase::~CParentDefClassBase();
}