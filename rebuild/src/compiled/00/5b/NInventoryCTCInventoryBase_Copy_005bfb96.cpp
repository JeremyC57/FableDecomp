// NInventory::CTCInventoryBase::Copy(const CTCInventoryBase* other) -- copies another inventory's state: selection state, serialised item lists, confiscated items, def pointer and indices, open flag, display lists, then re-adds every non-empty item and copies the CTCBase part
// Retail 0x005BFB96, 250 bytes, `ret 4` (vtable slot 12; the derived Copy overrides call it qualified).
// Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CCountedPointer<vector<CItem>>::operator= 0x005C07E4, CCountedPointer<vector<CCharString>>::operator=
// 0x005C0800 (manifest "CCountedPointer<CTexture>::operator=" twins), vector<CItem>::operator= 0x005C1F42,
// vector<long>::operator= 0x00454886 (x2), GetAllNonEmptyInventoryItems(vector<CItem>&, bool) 0x005BFA44 (on the
// other inventory), AddItem(const CItem&) 0x005BF4A3, CTCBase::Copy(const CTCBase&) 0x006868D0 (manifest "$E2"),
// ~vector<CItem> 0x005C1E30, the def's slot 1 on release.
// Shape: assignments in retail order; PDef through the inline CDefPointer operator= (old def released and zeroed, the
// new one add-ref'd when set); the count is taken once (size(), finish loaded first) and a named item
// pointer walks the 20-byte items while the count goes down in the dead `other` slot; only items with a positive count are re-added.
// Layout (retail; Ego_r.pdb names): InventorySelectionState +0x1C, InventoryOpen +0x58, PDef +0x68, GlobalDefIndex
// +0x6C, SelectedCategoryIndex +0x70, SerialisedItems +0x78, SerialisedSelectedItems +0x80, MenuDisplayListItem +0x94,
// MenuDisplayListCategory +0xA0, ConfiscatedItems +0xAC; NInventory::CItem 20 bytes, count at +0.
// Leaf: ?Copy@CTCInventoryBase
#pragma optimize("y", off)
#pragma optimize("s", on)

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CInventoryDef : public CDefPointeeBase { public:};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
};
namespace NInventory {
class CItem { public:                                         // 20 bytes
    long Count;                                        // +0x00
    unsigned char _pad_0x04[0x10];
};
}
struct CItemVector {                                   // vector<NInventory::CItem>
    NInventory::CItem* _M_start;
    NInventory::CItem* _M_finish;
    NInventory::CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CItemVector();                                    // 0x005C1E30
    CItemVector& operator=(const CItemVector& other);  // 0x005C1F42
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    NInventory::CItem& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CLongVector {                                   // vector<long>
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    CLongVector& operator=(const CLongVector& other);  // 0x00454886
};
struct CSerialisedItemsPointer {                       // CCountedPointer<vector<NInventory::CItem>>
    void* Data; void* Info;
    CSerialisedItemsPointer& operator=(const CSerialisedItemsPointer& other);   // 0x005C07E4
};
struct CSerialisedSelectedItemsPointer {               // CCountedPointer<vector<CCharString>>
    void* Data; void* Info;
    CSerialisedSelectedItemsPointer& operator=(const CSerialisedSelectedItemsPointer& other);   // 0x005C0800
};

class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Copy(const CTCBase& other);           // 0x006868D0
};

namespace NInventory {
class CTCInventoryBase : public CTCBase { public:
    unsigned char _pad_0x004[0x1c - 0x04];
    long InventorySelectionState;                      // +0x1c
    unsigned char _pad_0x020[0x58 - 0x20];
    bool InventoryOpen;                                // +0x58
    unsigned char _pad_0x059[0x68 - 0x59];
    CDefPointer<CInventoryDef> PDef;                   // +0x68
    long GlobalDefIndex;                               // +0x6c
    long SelectedCategoryIndex;                        // +0x70
    void* GraphicManager;                              // +0x74
    CSerialisedItemsPointer SerialisedItems;           // +0x78
    CSerialisedSelectedItemsPointer SerialisedSelectedItems;   // +0x80
    unsigned char _pad_0x088[0x94 - 0x88];
    CLongVector MenuDisplayListItem;                   // +0x94
    CLongVector MenuDisplayListCategory;               // +0xa0
    CItemVector ConfiscatedItems;                      // +0xac
    void GetAllNonEmptyInventoryItems(CItemVector& items, bool b) const;   // 0x005BFA44
    void AddItem(const CItem& item);                   // 0x005BF4A3
    virtual void Copy(const CTCInventoryBase* other);
};

void CTCInventoryBase::Copy(const CTCInventoryBase* other)
{
    InventorySelectionState = other->InventorySelectionState;
    SerialisedItems = other->SerialisedItems;
    SerialisedSelectedItems = other->SerialisedSelectedItems;
    ConfiscatedItems = other->ConfiscatedItems;
    PDef = other->PDef;
    GlobalDefIndex = other->GlobalDefIndex;
    SelectedCategoryIndex = other->SelectedCategoryIndex;
    InventoryOpen = other->InventoryOpen;
    MenuDisplayListItem = other->MenuDisplayListItem;
    MenuDisplayListCategory = other->MenuDisplayListCategory;
    CItemVector items;
    other->GetAllNonEmptyInventoryItems(items, true);
    unsigned long count = items.size();
    CItem* item = items._M_start;
    for (unsigned long i = 0; i < count; ++i, ++item)
        if (item->Count > 0)
            AddItem(*item);
    CTCBase::Copy(*other);
}
}