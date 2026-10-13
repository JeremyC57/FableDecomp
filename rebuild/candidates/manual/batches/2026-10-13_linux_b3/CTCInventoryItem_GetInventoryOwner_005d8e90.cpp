// CTCInventoryItem::GetInventoryOwner()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8E90, oracle length 43 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+10h] ; test eax,eax ; je U ; mov eax,[eax+4] ; ret ; U: mov eax,[ecx+1Ch] ; test eax,eax ; mov edx,[ecx+18h] ; ja F ; test edx,edx ; jbe Z ; F: push eax ; push edx ; call <find map entry by UID> ; mov ecx,eax ; call <entry thing> ; ret ; Z: xor eax,eax ; ret
// engine/CTCInventoryItem.h (PDB): +0x10 PInventory (CTCInventoryBase*; its +0x04 is the owning
// CThing), +0x18 unsigned __int64 InventoryUIDToAttachTo. An item already inside an inventory
// answers that inventory's owner; otherwise a non-zero pending UID is resolved through the global
// thing-map lookup (0x00686AF0, __cdecl, 64-bit UID by value) and the entry's thing accessor
// (0x0051ED60, __thiscall). No inventory and no UID means no owner.
#include "engine/CTCInventoryItem.h"

struct CThing;

struct CTCInventoryBaseCalls {
    void*   __vftable;      // +0x00
    CThing* OwnerThing;     // +0x04 (name inferred)
};

struct CThingMapEntryCalls {
    CThing* GetThing();     // 0x0051ED60
};

extern CThingMapEntryCalls* GFFindThingMapEntryByUID(unsigned __int64 uid);   // 0x00686AF0

struct CTCInventoryItem_Methods : CTCInventoryItem {
    CThing* GetInventoryOwner();
};

CThing* CTCInventoryItem_Methods::GetInventoryOwner()
{
    CTCInventoryBaseCalls* inventory = reinterpret_cast<CTCInventoryBaseCalls*>(this->PInventory);
    if (inventory)
        return inventory->OwnerThing;
    if (this->InventoryUIDToAttachTo == 0)
        return 0;
    return GFFindThingMapEntryByUID(this->InventoryUIDToAttachTo)->GetThing();
}
