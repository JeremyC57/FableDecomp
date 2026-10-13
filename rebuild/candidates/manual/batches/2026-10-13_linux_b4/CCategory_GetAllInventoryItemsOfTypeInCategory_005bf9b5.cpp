// CCategory::GetAllInventoryItemsOfTypeInCategory(long, std::vector<CItem>&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF9B5, oracle length 46 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov esi,[edi+8] ; jmp C ; L: cmp dword ptr [esi+4],0 ; je N ; mov eax,[esi] ; cmp eax,[esp+0Ch] ; jne N ; mov ecx,[esp+10h] ; push esi ; call <vector push_back> ; N: add esi,14h ; C: cmp esi,[edi+0Ch] ; jb L ; pop edi ; pop esi ; ret 8
// engine/CCategory.h (PDB): +0x08 vector<CItem> ItemsInCategory (retail `first`/`last` at +0/+4,
// 20-byte CItem with ItemType at +0 and NumberOfItems at +4). Copies every non-empty stack of the
// requested item type into the caller's vector (push_back @ 0x005C2996, __thiscall on the
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
