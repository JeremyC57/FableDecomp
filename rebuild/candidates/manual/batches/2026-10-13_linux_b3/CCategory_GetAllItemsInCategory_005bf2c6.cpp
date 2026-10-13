// CCategory::GetAllItemsInCategory(std::vector<std::pair<long,long> >&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF2C6, oracle length 39 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov esi,[edi+8] ; jmp C ; L: cmp dword ptr [esi+4],0 ; je N ; push [esp+0Ch] ; mov ecx,esi ; call CItem::GetItemThings ; N: add esi,14h ; C: cmp esi,[edi+0Ch] ; jb L ; pop edi ; pop esi ; ret 4
// engine/CCategory.h (PDB): +0x08 vector<CItem> ItemsInCategory; CItem is 0x14 bytes with
// +0x04 NumberOfItems. Every non-empty item stack appends its (type, count) entries to the
// caller's vector through CItem::GetItemThings @ 0x005BF03C (__thiscall, one argument).
// The retail vector keeps `first` at +0 and `last` at +4 (see batch README).
#include "engine/CCategory.h"
#include "engine/CItem.h"

struct CItemCalls : CItem {
    void GetItemThings(void* outVector);        // 0x005BF03C
};

struct CItemVector {
    CItemCalls* First;      // +0x00
    CItemCalls* Last;       // +0x04
};

struct CCategory_Methods : CCategory {
    void GetAllItemsInCategory(void* outVector);
};

void CCategory_Methods::GetAllItemsInCategory(void* outVector)
{
    CItemVector* items = reinterpret_cast<CItemVector*>(&this->ItemsInCategory[0]);
    for (CItemCalls* item = items->First; item < items->Last; ++item) {
        if (item->NumberOfItems != 0)
            item->GetItemThings(outVector);
    }
}
