#include <cstdio>
struct CItem { long ItemType; long NumberOfItems; void* Data; void* Info; long AverageBoughtPrice; };
static int g_destroyed = 0;
struct CInventoryCategoryDef_Retail {
    long m_refCount; bool SelectEmptySlots; bool WrapHighlightCursor; long CategoryName;
    void Destroy() { ++g_destroyed; }
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CDefRef {
    CInventoryCategoryDef_Retail* m_p;
    CDefRef(CInventoryCategoryDef_Retail* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    ~CDefRef() { m_p->Release(); }
    CInventoryCategoryDef_Retail* operator->() const { return m_p; }
};
struct CDefPointer { CInventoryCategoryDef_Retail* m_pObject; CDefRef operator->() const { return CDefRef(m_pObject); } };
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem& operator[](unsigned long n) { return first[n]; }
};
#define FAIL { std::printf("005bdac5_TEST FAIL\n"); return 1; }
struct CThing { long nameId; };
struct CWideString { long id; CWideString(long i) : id(i) { } CWideString(const CWideString& o) : id(o.id) { } };
CWideString g_EmptyWideString(-1);
struct CTCInventoryBase {
    virtual ~CTCInventoryBase() { }
    virtual CWideString GetThingName(CThing* thing) { return CWideString(1000 + thing->nameId); }
    CWideString GetInventoryText(unsigned long textId) { return CWideString((long)textId); }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCategory_Retail {
    CBaseIntelligentPointer PAssociatedThing; CDefPointer PDef; CTCInventoryBase* PInventory;
    CWideString GetCategoryName()
    {
        if (PAssociatedThing.GetPItem() == 0) {
            if (PDef->CategoryName != 0) return PInventory->GetInventoryText(PDef->CategoryName);
            return g_EmptyWideString;
        }
        return PInventory->GetThingName(PAssociatedThing.GetPItem());
    }
};
int main()
{
    CInventoryCategoryDef_Retail def; def.m_refCount = 1; def.CategoryName = 0;
    CTCInventoryBase inv; CThing t; t.nameId = 7;
    CCategory_Retail c; c.PAssociatedThing.m_pItem = 0; c.PDef.m_pObject = &def; c.PInventory = &inv;
    if (c.GetCategoryName().id != -1) FAIL;
    def.CategoryName = 42;
    if (c.GetCategoryName().id != 42 || def.m_refCount != 1) FAIL;
    c.PAssociatedThing.m_pItem = &t;
    if (c.GetCategoryName().id != 1007) FAIL;
    std::printf("005bdac5_TEST PASS\n");
    return 0;
}
