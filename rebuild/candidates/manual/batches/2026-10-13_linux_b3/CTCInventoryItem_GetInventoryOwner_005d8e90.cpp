// CTCInventoryItem::GetInventoryOwner()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8E90, oracle length 43 bytes.
// Grade: candidate -- graded under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
//
// Retail bytes:  mov eax,[ecx+10h] ; test eax,eax ; je U ; mov eax,[eax+4] ; ret ; U: mov eax,[ecx+1Ch] ; test eax,eax ; mov edx,[ecx+18h] ; ja F ; test edx,edx ; jbe Z ; F: push eax ; push edx ; call <find map entry by UID> ; mov ecx,eax ; call <entry thing> ; ret ; Z: xor eax,eax ; ret
// engine/CTCInventoryItem.h (PDB): +0x10 PInventory (CTCInventoryBase*; its +0x04 is the owning
// CThing), +0x18 unsigned __int64 InventoryUIDToAttachTo. An item already inside an inventory
// answers that inventory's owner; otherwise a non-zero pending UID is resolved through the global
// thing-map lookup (0x00686AF0, __cdecl, 64-bit UID by value) and the entry's thing accessor
// (0x0051ED60, __thiscall). No inventory and no UID means no owner.
//
// Lane feedback: STILL DIFFER(42v43). `__stdcall` lookup and the 64-bit `> 0` test now match; the one
// remaining difference is that retail ends with `call GetThing ; ret` where our build emits the
// sibling-call `jmp`. Open: see the batch README.
#include "engine/CTCInventoryItem.h"

struct CThing;

struct CTCInventoryBaseCalls {
    void*   __vftable;      // +0x00
    CThing* OwnerThing;     // +0x04 (name inferred)
};

struct CThingMapEntryCalls {
    CThing* GetThing();     // 0x0051ED60
};

// 64-bit UID by value on the stack, callee-cleaned (retail has no `add esp,8` after the call).
extern CThingMapEntryCalls* __stdcall GFFindThingMapEntryByUID(unsigned __int64 uid);   // 0x00686AF0

struct CTCInventoryItem_Methods : CTCInventoryItem {
    CThing* GetInventoryOwner();
};

CThing* CTCInventoryItem_Methods::GetInventoryOwner()
{
    // Inside an inventory: that inventory's owner. Otherwise a positive pending UID is resolved
    // through the global thing map (`ja`/`jbe` pair = unsigned 64-bit `> 0`).
    CTCInventoryBaseCalls* inventory = reinterpret_cast<CTCInventoryBaseCalls*>(this->PInventory);
    if (inventory)
        return inventory->OwnerThing;
    if (this->InventoryUIDToAttachTo > 0)
        return GFFindThingMapEntryByUID(this->InventoryUIDToAttachTo)->GetThing();
    return 0;
}
