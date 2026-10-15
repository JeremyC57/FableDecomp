// CTCStockItem::SetPrice(long price) -- store the new price and release the item's price-tag primitive (it is rebuilt on the next draw)
// Leaf: SetPrice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ABBF0, length 67 bytes (`ret 4`).
// Callees: the primitive's virtual slots 3 (+0x0C, the last-handle notification, with the handle) and 1 (+0x04, Destroy
// when the count reaches zero).
// Layout: engine/CTCStockItem.h (Ego_r.pdb, 0x1c bytes): Price +0x10, PrimHandle (CEnginePrimitiveHandle: vptr 0x0122F598 +
// CIVCountedPointer<CEnginePersistentPrimitiveBase> Data) +0x14 -- the header keeps the handle opaque, so it is viewed
// through the CEnginePrimitiveHandle model of the landed CTCEmoteIcon::FrameUpdate 0x0071AF30.
// Shape: the handle's inline Release (`if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0;`, the
// CIVCountedPointer `operator=(0)` = release + a second unconditional zero store) on a handle reference held in esi.
#include "engine/CTCStockItem.h"

class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Release()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CTCStockItem_Methods : CTCStockItem {
    CEnginePrimitiveHandle& GetPrimHandle() { return *reinterpret_cast<CEnginePrimitiveHandle*>(PrimHandle); }
    void SetPrice(long price);
};

void CTCStockItem_Methods::SetPrice(long price)
{
    Price = price;
    GetPrimHandle().Release();
}