// CItem::IsSameItemTypeAsDefIndex(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC450, oracle length 24 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  cmp dword ptr [ecx+4],0 ; jne L ; xor al,al ; jmp R ; L: mov edx,[esp+4] ; xor eax,eax ; cmp edx,[ecx] ; sete al ; R: ret 4
// engine/CItem.h (PDB): +0x00 long ItemType, +0x04 long NumberOfItems. An empty stack never
// matches; otherwise compare the item's definition index with the requested one.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
    bool IsSameItemTypeAsDefIndex(long defIndex);
};

bool CItem_Methods::IsSameItemTypeAsDefIndex(long defIndex)
{
    if (this->NumberOfItems == 0)
        return false;
    return defIndex == this->ItemType;
}
