// CCategory::UpdateItemStatsForThing(CThing*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF16E, oracle length 46 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; movzx eax,word ptr [edi+8Ch] ; push eax ; mov esi,ecx ; call CCategory::GetSlotWithItemsOfType ; cmp eax,-1 ; je R ; mov ecx,[esi+8] ; lea eax,[eax+eax*4] ; push edi ; lea ecx,[ecx+eax*4] ; call CItem::UpdateItemStats ; R: pop edi ; pop esi ; ret 4
// engine/CCategory.h (PDB): +0x08 vector<CItem> ItemsInCategory (retail `first` at +0; CItem is
// 20 bytes). The thing's definition index is the 16-bit value at +0x8C of the thing (Ego_r.pdb
// assigns that offset to a pointer in CThing, so the shipped CThing differs there); the slot
// holding that item type (0x005BE85B, -1 if none) gets its stats refreshed from the thing
// (CItem::UpdateItemStats @ 0x005BEDCB).
#include "engine/CCategory.h"
#include "engine/CItem.h"

struct CThing;

struct CItemCalls : CItem {
    void UpdateItemStats(CThing* thing);                // 0x005BEDCB
};

struct CCategory_Methods : CCategory {
    long GetSlotWithItemsOfType(long itemType);         // 0x005BE85B
    void UpdateItemStatsForThing(CThing* thing);
};

void CCategory_Methods::UpdateItemStatsForThing(CThing* thing)
{
    long slot = GetSlotWithItemsOfType(*(unsigned short*)((char*)thing + 0x8C));   // thing's def index
    if (slot != -1) {
        CItemCalls* items = *(CItemCalls**)&this->ItemsInCategory[0];              // vector first
        items[slot].UpdateItemStats(thing);
    }
}
