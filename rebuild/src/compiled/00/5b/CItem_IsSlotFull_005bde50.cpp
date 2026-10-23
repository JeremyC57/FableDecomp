// CItem::IsSlotFull() -- false for an empty slot; else whether the count equals the inventory item def's maximum (+0x40 of the counted def from 0x005BD82E, released after the read)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDE50, length 70 bytes.
// Shape: the EBP frame with the 4-byte counted temporary, `this` in edi, `xor eax,eax ; cmp [edi+4],esi ; sete al`, `leave`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
struct CIVCountedPointeeVirtual {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
};
struct CIVRefCounted {                             // second base: the count at +4 behind the vptr
    unsigned long IVRefCount;
    __forceinline void DecRef() { --IVRefCount; }
    __forceinline unsigned long GetRefCount() const { return IVRefCount; }
};
struct CIVCountedPointeeBase : CIVCountedPointeeVirtual, CIVRefCounted {
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
struct CInventoryItemDef_Retail : CIVCountedPointeeBase {
    unsigned char _pad[0x40 - 8];
    long          MaxItems;                        // +0x40
};
struct CIVCountedPointer_Def {                     // 4 bytes
    CInventoryItemDef_Retail* Object;
    __forceinline ~CIVCountedPointer_Def()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
struct CItem_Methods : CItem {
    CIVCountedPointer_Def GetInventoryItemDef();   // 0x005BD82E (returns its hidden result pointer)
    bool IsSlotFull();
};

bool CItem_Methods::IsSlotFull()
{
    if (NumberOfItems == 0)
        return false;
    long maximum = GetInventoryItemDef().Object->MaxItems;
    return NumberOfItems == maximum;
}