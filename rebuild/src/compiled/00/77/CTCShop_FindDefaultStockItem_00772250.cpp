// CTCShop::FindDefaultStockItem(long defIndex) -- the shop def's default stock entry for an item def: first by the item's own def index, then by its inventory item's category; 0 when the item has no stock/inventory sub-def or the shop lists neither
// Leaf: FindDefaultStockItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00772250, 378 bytes (`ret 4`, returns const CShopItem*).
// Callees: CTCBase::GetDefinitions() const 0x006869D0; CDefinitionManager::GetPDefFromGlobalIndex(long) 0x009AD9E0
//          (CDefPointer by value, homed in the dead `defIndex` argument slot); CDefClassBase::GetSubDef<CStockItemDef>
//          0x0059350C and GetSubDef<CInventoryItemDef> 0x005BADE1 (manifest "GetSubDef<CAbilityDef>" labels are propagated
//          byte twins; landed spellings); CDefClassBase::GetSubDef<CShopDef> 0x005130A0 (landed GetName 0x00772610); the
//          defs' virtual slot 1 on release.
// Layout: CTCShop ParentThing +0x04; CThing PDef +0x70; CShopDef default stock items +0x40/+0x44 and category stock items
//          +0x4c/+0x50 (0x48-byte records: key +0x28, CShopItem +0x2c); CInventoryItemDef category +0x3c (name unverified).
// Shape: the three sub-defs are null-tested CDefPointer locals with GetSubDef stand-ins (the out pointers re-read once into
//        registers, released `dec [reg+4] ; jne`, the null test dropped after a dereference); the by-value def is the
//        DecRef-then-GetRefCount temporary form; the thing's def is read into a named local before the shop-def lookup.
class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};

class CShopItem { public:                              // 28 bytes
    float Quantity;
    float PriceMultiplier;
    float MaxStock;
    long  RestockPeriod;
    float RestockPeriodRandomness;
    float SalesPerDay;
    float SalesPerDayRandomness;
};
class CStockItemDef { public:                          // 0x48-byte stock record
    unsigned char _pad00[0x28];
    long          Key;                          // +0x28
    CShopItem     Item;                         // +0x2c
};
class CShopDef : public CDefPointeeBase { public:
    unsigned char  _pad08[0x40 - 0x08];
    CStockItemDef* DefaultStockFirst;           // +0x40
    CStockItemDef* DefaultStockLast;            // +0x44
    unsigned char  _pad48[4];
    CStockItemDef* CategoryStockFirst;          // +0x4c
    CStockItemDef* CategoryStockLast;           // +0x50
};
struct CStockDef : CDefPointeeBase { };
class CInventoryItemDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x3c - 0x08];
    long          Category;                     // +0x3c (name unverified)
};

class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef_0059350c(CDefPointer<CStockDef>* out);          // 0x0059350C
    bool GetSubDef_005bade1(CDefPointer<CInventoryItemDef>* out);  // 0x005BADE1
    bool GetSubDef_005130a0(CDefPointer<CShopDef>* out);           // 0x005130A0
};
void* FindDef_GetSubDef_0();                    // stand-in callees of the stand-in bodies below
void* FindDef_GetSubDef_1();
void* FindDef_GetSubDef_2();
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed GetName 0x00772610 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_0059350c(CDefPointer<CStockDef>* out)
{
    void* def = FindDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef_005bade1(CDefPointer<CInventoryItemDef>* out)
{
    void* def = FindDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef_005130a0(CDefPointer<CShopDef>* out)
{
    void* def = FindDef_GetSubDef_2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CDefTemporary {                          // the by-value CDefPointer<CDefClassBase> result
    CDefClassBase* Object;
    CDefTemporary() { }
    ~CDefTemporary() { if (Object) Object->ReleaseTemporary(); }
    CDefClassBase* operator->() const { return Object; }
};
class CDefinitionManager { public:
    CDefTemporary GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
class CTCShop { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    CDefinitionManager* GetDefinitions() const; // 0x006869D0 (CTCBase)
    const CShopItem* FindDefaultStockItem(long defIndex);
};

const CShopItem* CTCShop::FindDefaultStockItem(long defIndex)
{
    CDefTemporary def = GetDefinitions()->GetPDefFromGlobalIndex(defIndex);
    CDefPointer<CStockDef> stockDef;
    CDefPointer<CInventoryItemDef> itemDef;
    CDefPointer<CShopDef> shopDef;
    if (def->GetSubDef_0059350c(&stockDef) && def->GetSubDef_005bade1(&itemDef)) {
        CDefClassBase* thingDef = ParentThing->PDef;
        if (thingDef->GetSubDef_005130a0(&shopDef)) {
        for (CStockItemDef* it = shopDef->DefaultStockFirst; it != shopDef->DefaultStockLast; ++it)
            if (it->Key == defIndex)
                return &it->Item;
        for (CStockItemDef* it2 = shopDef->CategoryStockFirst; it2 != shopDef->CategoryStockLast; ++it2)
            if (it2->Key == itemDef->Category)
                return &it2->Item;
        }
    }
    return 0;
}