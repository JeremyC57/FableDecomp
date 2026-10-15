// Behaviour test for CTCStockItem::SetPrice (0x008ABBF0): the price is stored; the price-tag primitive is notified when
// this is its last handle, its reference dropped and the handle emptied.
#include <cstdio>

class CEnginePrimitiveHandle;
static int g_notified = 0, g_destroyed = 0;
static CEnginePrimitiveHandle* g_notifiedHandle = 0;

class CEnginePersistentPrimitiveBase {
public:
    long RefCount;
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual void Destroy() { ++g_destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle) { ++g_notified; g_notifiedHandle = handle; }
    void Release() { if (--RefCount == 0) Destroy(); }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

class CEnginePrimitiveHandle {
public:
    CIVCountedPointer Data;
    void Release()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CTCStockItem {
    bool                   ForSale;
    long                   Price;
    CEnginePrimitiveHandle PrimHandle;
    CEnginePrimitiveHandle& GetPrimHandle() { return PrimHandle; }
    void SetPrice(long price);
};

void CTCStockItem::SetPrice(long price)
{
    Price = price;
    GetPrimHandle().Release();
}

int main()
{
    CEnginePersistentPrimitiveBase prim; prim.RefCount = 2;
    CTCStockItem item; item.Price = 0; item.PrimHandle.Data.Object = &prim;
    item.SetPrice(150);                                     // shared primitive: no notification
    if (item.Price != 150 || g_notified || g_destroyed || prim.RefCount != 1 || item.PrimHandle.Data.Object) { std::printf("008abbf0_TEST FAIL\n"); return 1; }
    item.PrimHandle.Data.Object = &prim;                    // now the last handle
    item.SetPrice(75);
    if (item.Price != 75 || g_notified != 1 || g_notifiedHandle != &item.PrimHandle || g_destroyed != 1 || item.PrimHandle.Data.Object) {
        std::printf("008abbf0_TEST FAIL\n"); return 1;
    }
    item.SetPrice(5);                                       // empty handle
    if (item.Price != 5 || g_notified != 1) { std::printf("008abbf0_TEST FAIL\n"); return 1; }
    std::printf("008abbf0_TEST PASS\n");
    return 0;
}