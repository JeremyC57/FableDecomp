// CTCShop::FindDefaultStockItem(long defIndex) -- the shop def's default stock entry for an item def: first by the item's own def index, then by its inventory item's category; 0 when the item has no stock/inventory sub-def or the shop lists neither
// Leaf: FindDefaultStockItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00772250, 378 bytes (`ret 4`, returns const CShopItem*).
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0; CDefinitionManager::GetPDefFromGlobalIndex(long) 0x009AD9E0
//          (CDefPointer by value, homed in the dead `defIndex` argument slot); CDefClassBase::GetSubDef<CStockItemDef>
//          0x0059350C (pushes "CStockItemDef") and GetSubDef<CInventoryItemDef> 0x005BADE1 (manifest "GetSubDef<CAbilityDef>"
//          labels are propagated byte twins); CDefClassBase::GetSubDef<CShopDef> 0x005130A0 (landed GetName 0x00772610); the
//          defs' virtual slot 1 on release.
// Layout: CTCShop ParentThing +0x04; CThing PDef +0x70; CShopDef DefaultStock +0x40 and StockCategoryDefaults +0x4c (STLport vectors of
//          0x48-byte CShopItemDef: Type +0x28, CShopItem +0x2c); CInventoryItemDef InventoryCategory +0x3c (Ego_r.pdb).
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
    CDefPointeeBase* Object;                    // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase;
template <> class CDefPointer<const CDefClassBase> { public:   // the manager's by-value lookup result (released temporary)
    CDefPointeeBase* Object;
    CDefPointer() { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    const CDefClassBase* operator->() const;
};

class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // Ego_r / FableWin member template
};
inline const CDefClassBase* CDefPointer<const CDefClassBase>::operator->() const { return static_cast<const CDefClassBase*>(Object); }

class CShopItem { public:                              // 28 bytes
    float Quantity;
    float PriceMultiplier;
    float MaxStock;
    long  RestockPeriod;
    float RestockPeriodRandomness;
    float SalesPerDay;
    float SalesPerDayRandomness;
};
class CShopItemDef : public CDefClassBase { public:   // 0x48-byte stock record (Ego_r.pdb CShopItemDef)
    unsigned char _pad08[0x28 - 0x08];
    long          Type;                         // +0x28 (item def index or inventory category)
    CShopItem     Item;                         // +0x2c
};
struct CShopItemDefVector {                     // STLport vector<CShopItemDef> (12 bytes)
    CShopItemDef* _M_start;
    CShopItemDef* _M_finish;
    CShopItemDef* _M_end_of_storage;
};
class CShopDef : public CDefClassBase { public:
    unsigned char      _pad08[0x40 - 0x08];
    CShopItemDefVector DefaultStock;            // +0x40
    CShopItemDefVector StockCategoryDefaults;   // +0x4c
};
class CStockItemDef : public CDefClassBase { public: };
class CInventoryItemDef : public CDefClassBase { public:
    unsigned char _pad08[0x3c - 0x08];
    long          InventoryCategory;            // +0x3c (Ego_r.pdb)
};

void* FindDef_GetSubDef_0();                    // stand-in callees of the stand-in bodies below
void* FindDef_GetSubDef_1();
void* FindDef_GetSubDef_2();
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed GetName 0x00772610 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>* out) const   // 0x0059350C
{
    void* def = FindDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<const CInventoryItemDef>* out) const   // 0x005BADE1
{
    void* def = FindDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CShopDef>(CDefPointer<const CShopDef>* out) const   // 0x005130A0
{
    void* def = FindDef_GetSubDef_2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (Ego_r.pdb QBE)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
class CTCBase { public:                         // the component base (members this row reads)
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
class CTCShop : public CTCBase { public:
    const CShopItem* FindDefaultStockItem(long defIndex) const;
};

const CShopItem* CTCShop::FindDefaultStockItem(long defIndex) const
{
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(defIndex);
    CDefPointer<const CStockItemDef> stockDef;
    CDefPointer<const CInventoryItemDef> itemDef;
    CDefPointer<const CShopDef> shopDef;
    if (def->GetSubDef(&stockDef) && def->GetSubDef(&itemDef)) {
        CDefClassBase* thingDef = ParentThing->PDef;
        if (thingDef->GetSubDef(&shopDef)) {
        for (CShopItemDef* it = shopDef->DefaultStock._M_start; it != shopDef->DefaultStock._M_finish; ++it)
            if (it->Type == defIndex)
                return &it->Item;
        for (CShopItemDef* it2 = shopDef->StockCategoryDefaults._M_start; it2 != shopDef->StockCategoryDefaults._M_finish; ++it2)
            if (it2->Type == itemDef->InventoryCategory)
                return &it2->Item;
        }
    }
    return 0;
}
