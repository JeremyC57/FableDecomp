#pragma optimize("s",on)
// Leaf: FinishMove
#include "rebuild_abi.h"

// Retail 0x00458c66 CTCAssassinRush::FinishMove (__fastcall, void, 33 bytes)
// this+0x68: sub-object; a member fn is called on it (lea ecx,[esi+0x68]; call rel32)
// this+0x50: a heap pointer; if non-null it is freed (push eax; call free; pop ecx)
// tail-jmp to base-class FinishMove with ecx=this.

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CTCAssassinRush { public:
    char _pad[0x68];
    // at 0x68: sub-object (opaque)
};

// member fn invoked on this+0x68 (relocation-masked target)
extern "C" void __fastcall AssassinRush_Sub0x68_Method(void* thisptr);

// free-like callee (single stack arg, cdecl, caller pops via 'pop ecx')
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

// base-class FinishMove (tail-called with ecx=this)

void __fastcall CTCAssassinRush_FinishMove(CTCAssassinRush* thisptr)
{
    unsigned char* base = (unsigned char*)thisptr;

    AssassinRush_Sub0x68_Method(base + 0x68);

    void* p = *(void**)(base + 0x50);
    if (p) {
        free(p);
    }

    ((CParentDefClassBase*)(thisptr))->CParentDefClassBase::~CParentDefClassBase();
}