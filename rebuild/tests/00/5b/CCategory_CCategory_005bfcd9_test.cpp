#include <cstdio>
#define FAIL { std::printf("005bfcd9_TEST FAIL\n"); return 1; }
struct CThing { int id; };
static int g_destroyed = 0, g_assigns = 0;
struct CInventoryCategoryDef_Retail { long m_refCount; long NumberOfSlots; void Destroy() { ++g_destroyed; } };
struct CDefPointer {
    CInventoryCategoryDef_Retail* Object;
    CDefPointer(CInventoryCategoryDef_Retail* p) : Object(p) { if (Object) ++Object->m_refCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
};
struct CIntelligentPointerThing {
    CThing* m_pItem;
    CIntelligentPointerThing() : m_pItem((CThing*)1) { }
    CIntelligentPointerThing(CThing* t) : m_pItem(t) { }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& o) { ++g_assigns; m_pItem = o.m_pItem; return *this; }
};
struct CItemVector { long size; CItemVector() : size(0) { } void resize(unsigned long n) { size = (long)n; } };
struct CTCInventoryBase { int id; };
struct CCategory_Retail {
    CIntelligentPointerThing PAssociatedThing; CItemVector ItemsInCategory; CDefPointer PDef; CTCInventoryBase* PInventory;
    long GlobalDefIndex, SelectedItemIndex, HighlightedItemIndex; bool Selected;
    CCategory_Retail(CDefPointer def, CTCInventoryBase* inventory, long globalDefIndex)
        : PAssociatedThing(), ItemsInCategory(), PDef(def), PInventory(inventory), GlobalDefIndex(globalDefIndex),
          SelectedItemIndex(-1), HighlightedItemIndex(0), Selected(false)
    {
        ItemsInCategory.resize(PDef.Object->NumberOfSlots);
        PAssociatedThing = (CThing*)0;
    }
};
int main()
{
    CInventoryCategoryDef_Retail def; def.m_refCount = 0; def.NumberOfSlots = 6;
    CTCInventoryBase inv;
    {
        CDefPointer p(&def);
        CCategory_Retail c(p, &inv, 17);
        if (def.m_refCount != 2 || c.ItemsInCategory.size != 6 || c.PInventory != &inv || c.GlobalDefIndex != 17 ||
            c.SelectedItemIndex != -1 || c.HighlightedItemIndex != 0 || c.Selected || c.PAssociatedThing.m_pItem != 0 || g_assigns != 1)
            FAIL;
    }
    if (def.m_refCount != 0 || g_destroyed != 1) FAIL;
    std::printf("005bfcd9_TEST PASS\n");
    return 0;
}