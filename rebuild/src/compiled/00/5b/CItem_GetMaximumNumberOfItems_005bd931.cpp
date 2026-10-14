// CItem::GetMaximumNumberOfItems() -- zero for an empty slot; else the inventory item def's maximum (+0x40 of the counted def from 0x005BD82E, released after the read)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD931, length 58 bytes.
// Shape: the EBP frame with the 4-byte counted temporary at [ebp-4], the maximum in esi across the release, `leave`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CInventoryItemDef_Retail : CIVCountedPointeeBase {
    unsigned char _pad[0x40 - 8];
    long          MaxItems;                        // +0x40
};
struct CIVCountedPointer_Def {                     // 4 bytes
    CInventoryItemDef_Retail* Object;
    __forceinline ~CIVCountedPointer_Def()
    {
        CInventoryItemDef_Retail* old = Object;
        if (old) {
            CInventoryItemDef_Retail* object = old;     // kept for the virtual call (ecx)
            unsigned long* count = &old->IVRefCount;    // the count by address: `add eax,4 ; dec [eax] ; cmp [eax],0`
            --*count;
            if (*(volatile unsigned long*)count == 0)   // re-read after the decrement
                object->Delete();
        }
    }
};
struct CItem_Methods : CItem {
    CIVCountedPointer_Def GetInventoryItemDef();   // 0x005BD82E (returns its hidden result pointer)
    long GetMaximumNumberOfItems();
};

long CItem_Methods::GetMaximumNumberOfItems()
{
    if (NumberOfItems == 0)
        return 0;
    long maximum = GetInventoryItemDef().Object->MaxItems;
    return maximum;
}