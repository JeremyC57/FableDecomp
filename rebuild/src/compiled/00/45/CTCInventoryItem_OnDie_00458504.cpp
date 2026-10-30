#pragma optimize("s",on)
// CTCInventoryItem::OnDie @ 0x00458504
// push esi; mov esi,ecx; mov eax,[esi+0x3c]; test eax,eax; jz L;
// push eax; call free (0xbfea14); pop ecx; L: mov ecx,esi; pop esi; jmp base::OnDie (0x430300)

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CTCInventoryItem { public:
    char pad[0x3c];
    void* f3c;
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->f3c)
        free(this->f3c);
    ((CParentDefClassBase*)(this))->CParentDefClassBase::~CParentDefClassBase();
}