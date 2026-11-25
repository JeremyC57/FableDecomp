// CTCInventoryItem::OnDie  @ 0x00529cc0  (__fastcall, void)
// push esi; esi=this; eax=[this+0x10]; if(eax){ free_helper(eax); } Base::OnDie(this) tail-jmp

// Naming (NF450, 2026-11-25): callee-only respell of the tail call to the PDB destructor (T1 tracker; target self row's Leaf),
//   0x0099A2E0 ??1CBaseClass@@UAE@XZ (was ?OnDie@CInventoryItemBase@@QAEXXZ), a qualified call through engine/CBaseClass.h (ruling r2;
//   r449d: the shared header, not a bodiless class).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// cdecl free-style helper at 0x00bfea14
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CInventoryItemBase
{
};

class CTCInventoryItem : public CInventoryItemBase
{ public:
    char pad[0x10];        // 0x00 .. 0x0f
    void* m_ptr;           // 0x10
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    void* p = this->m_ptr;
    if (p != 0)
        free(p);
    reinterpret_cast<CBaseClass*>(this)->CBaseClass::~CBaseClass();   // tail jmp 0x0099A2E0
}