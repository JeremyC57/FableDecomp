// NInventory::CItem::GetMaximumNumberOfItems() const -- zero for an empty slot; else the inventory item def's maximum (+0x40 of the counted def from 0x005BD82E, released after the read)
// Leaf: ?GetMaximumNumberOfItems@CItem@NInventory@@QBEJXZ
// Identity (naming wave 11): Ego_r.pdb 0x00620834 + FableWin.pdb (public const, NInventory::CItem); the callee 0x005BD82E is Ego_r's
//   private const GetInventoryItemDef returning CDefPointer<const CInventoryItemDef>.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD931, length 58 bytes.
// Shape: the EBP frame with the 4-byte counted temporary at [ebp-4], the maximum in esi across the release, `leave`.
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
class CIVCountedPointeeBase : public CIVCountedPointeeVirtual, public CIVRefCounted { public:
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
class CInventoryItemDef : public CIVCountedPointeeBase { public:
    unsigned char _pad[0x40 - 8];
    long          MaxItems;                        // +0x40
};
template <class T> class CDefPointer {             // 4 bytes: the counted def pointer
public:
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object)
            const_cast<CInventoryItemDef*>(Object)->ReleaseTemporary();
    }
};
namespace NInventory {
class CItem : public ::CItem {                     // NInventory::CItem (Ego_r.pdb / FableWin.pdb) over the shared engine/CItem.h layout
public:
    long GetMaximumNumberOfItems() const;
private:
    CDefPointer<const CInventoryItemDef> GetInventoryItemDef() const;   // 0x005BD82E (returns its hidden result pointer)
};
}

long NInventory::CItem::GetMaximumNumberOfItems() const
{
    if (NumberOfItems == 0)
        return 0;
    long maximum = GetInventoryItemDef().Object->MaxItems;
    return maximum;
}