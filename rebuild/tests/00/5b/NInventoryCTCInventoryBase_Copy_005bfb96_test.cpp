// Behaviour test for NInventory::CTCInventoryBase::Copy 0x005BFB96 (model copy with simplified members)
#include <cstdio>

static int g_destroyed = 0, g_added = 0, g_baseCopied = 0;
class CDefPointeeBase { public:
    long RefCount;
    virtual void Destroy() { ++g_destroyed; }
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = other.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
};
struct CItem { long Count; long Def; };
struct CItemVector {
    CItem* _M_start; CItem* _M_finish;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
static CItem g_source[3] = { { 2, 7 }, { 0, 8 }, { 5, 9 } };
class CTCBase { public: void Copy(const CTCBase&) { ++g_baseCopied; } };
struct CTCInventoryBase : CTCBase {
    long InventorySelectionState;
    bool InventoryOpen;
    CDefPointer<CDefPointeeBase> PDef;
    long GlobalDefIndex, SelectedCategoryIndex;
    long AddedDefs[4];
    void GetAllNonEmptyInventoryItems(CItemVector& items, bool) const { items._M_start = g_source; items._M_finish = g_source + 3; }
    void AddItem(const CItem& item) { AddedDefs[g_added++] = item.Def; }
    void Copy(const CTCInventoryBase* other);
};

void CTCInventoryBase::Copy(const CTCInventoryBase* other)
{
    InventorySelectionState = other->InventorySelectionState;
    PDef = other->PDef;
    GlobalDefIndex = other->GlobalDefIndex;
    SelectedCategoryIndex = other->SelectedCategoryIndex;
    InventoryOpen = other->InventoryOpen;
    CItemVector items;
    other->GetAllNonEmptyInventoryItems(items, true);
    unsigned long count = items.size();
    CItem* item = items._M_start;
    for (unsigned long i = 0; i < count; ++i, ++item)
        if (item->Count > 0)
            AddItem(*item);
    CTCBase::Copy(*other);
}

int main()
{
    CDefPointeeBase oldDef; oldDef.RefCount = 1;
    CDefPointeeBase newDef; newDef.RefCount = 1;
    CTCInventoryBase a, b;
    a.PDef.Object = &oldDef; b.PDef.Object = &newDef;
    b.InventorySelectionState = 3; b.GlobalDefIndex = 11; b.SelectedCategoryIndex = 2; b.InventoryOpen = true;
    a.Copy(&b);
    if (a.PDef.Object != &newDef || newDef.RefCount != 2 || g_destroyed != 1 || a.GlobalDefIndex != 11 ||
        a.SelectedCategoryIndex != 2 || !a.InventoryOpen || a.InventorySelectionState != 3 ||
        g_added != 2 || a.AddedDefs[0] != 7 || a.AddedDefs[1] != 9 || g_baseCopied != 1) {
        std::printf("005bfb96_TEST FAIL\n"); return 1;
    }
    std::printf("005bfb96_TEST PASS\n");
    return 0;
}