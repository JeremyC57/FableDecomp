#pragma optimize("s",on)
// Leaf: OnDie
#include "rebuild_abi.h"

// CTCInventoryItem::OnDie @ 0x0045855a
// push esi; mov esi,ecx; mov eax,[esi+0x3c]; test eax,eax; je skip;
//   push eax; call <free>; pop ecx; skip: mov ecx,esi; pop esi; jmp <base::OnDie>

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CTCInventoryItem
{ public:
    char pad[0x3c];
    void* m_ptr;   // +0x3c
};

// cdecl free-style callee (relocation-masked target)
extern "C" void __cdecl engine_free_0045855a(void* p);

// base-class OnDie tail-called with __fastcall (this in ecx) -> model as __fastcall(this)

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0)
    {
        engine_free_0045855a(p);
    }
    // tail call to base class OnDie with same this
    ((CParentDefClassBase*)(thisptr))->CParentDefClassBase::~CParentDefClassBase();
}