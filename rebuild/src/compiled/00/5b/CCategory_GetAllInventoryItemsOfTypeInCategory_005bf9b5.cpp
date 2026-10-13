#pragma optimize("s",on)
// CCategory::GetAllInventoryItemsOfTypeInCategory(long, std::vector<CItem>&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF9B5, oracle length 46 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov esi,[edi+8] ; jmp C ; L: cmp dword ptr [esi+4],0 ; je N ; mov eax,[esi] ; cmp eax,[esp+0Ch] ; jne N ; mov ecx,[esp+10h] ; push esi ; call <vector push_back> ; N: add esi,14h ; C: cmp esi,[edi+0Ch] ; jb L ; pop edi ; pop esi ; ret 8
// engine/CCategory.h (PDB): +0x08 vector<CItem> ItemsInCategory (retail `first`/`last` at +0/+4,
// 20-byte CItem with ItemType at +0 and NumberOfItems at +4). Copies every non-empty stack of the
// requested item type into the caller's vector (push_back @ 0x005C2996, __fastcall on the
// output vector with a reference to the item).
#include "engine/CCategory.h"
#include "engine/CItem.h"

struct CItemVectorCalls {
    void PushBack(const CItem& item);                   // 0x005C2996 (vector<CItem>::push_back)
};

struct CItemVectorView {
    CItem* First;      // +0x00
    CItem* Last;       // +0x04
};

struct CCategory_Methods : CCategory {
    void GetAllInventoryItemsOfTypeInCategory(long itemType, CItemVectorCalls* outItems);
};

void CCategory_Methods::GetAllInventoryItemsOfTypeInCategory(long itemType, CItemVectorCalls* outItems)
{
    CItemVectorView* items = reinterpret_cast<CItemVectorView*>(&this->ItemsInCategory[0]);
    for (CItem* item = items->First; item < items->Last; ++item) {
        if (item->NumberOfItems != 0 && item->ItemType == itemType)
            outItems->PushBack(*item);
    }
}