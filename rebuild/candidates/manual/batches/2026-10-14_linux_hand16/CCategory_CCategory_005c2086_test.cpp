#include <cstdio>
static int g_ipCopies = 0, g_vecCopies = 0;
struct CIntelligentPointer { void* m_pObject; void* m_pInfo; CIntelligentPointer() : m_pObject(0), m_pInfo(0) {} CIntelligentPointer(const CIntelligentPointer& o) : m_pObject(o.m_pObject), m_pInfo(o.m_pInfo) { ++g_ipCopies; } };
struct CItemVector { void* first; void* last; void* end_of_storage; CItemVector() : first(0), last(0), end_of_storage(0) {} CItemVector(const CItemVector& o) : first(o.first), last(o.last), end_of_storage(o.end_of_storage) { ++g_vecCopies; } };
struct CDefObj { long m_refCount; void AddRef() { ++m_refCount; } };
struct CDefPointer { CDefObj* m_pObject; CDefPointer(CDefObj* p) : m_pObject(p) {} CDefPointer(const CDefPointer& o) : m_pObject(o.m_pObject) { if (m_pObject) m_pObject->AddRef(); } };
struct CCategory_Retail { CIntelligentPointer PAssociatedThing; CItemVector ItemsInCategory; CDefPointer PDef; void* PInventory; long GlobalDefIndex; long SelectedItemIndex; long HighlightedItemIndex; bool Selected; bool DisplayItems;
    CCategory_Retail(CDefObj* d) : PDef(d), PInventory(0), GlobalDefIndex(1), SelectedItemIndex(2), HighlightedItemIndex(3), Selected(true), DisplayItems(false) {}
    CCategory_Retail(const CCategory_Retail& o) : PAssociatedThing(o.PAssociatedThing), ItemsInCategory(o.ItemsInCategory), PDef(o.PDef), PInventory(o.PInventory), GlobalDefIndex(o.GlobalDefIndex), SelectedItemIndex(o.SelectedItemIndex), HighlightedItemIndex(o.HighlightedItemIndex), Selected(o.Selected), DisplayItems(o.DisplayItems) {} };

int main()
{
    CDefObj def = { 1 }; int x; CCategory_Retail a(&def); a.PAssociatedThing.m_pObject = &x; a.ItemsInCategory.first = &x;
    CCategory_Retail b(a);
    if (g_ipCopies != 1 || g_vecCopies != 1 || def.m_refCount != 2 || b.PAssociatedThing.m_pObject != &x || b.ItemsInCategory.first != &x || b.PDef.m_pObject != &def || b.GlobalDefIndex != 1 || b.SelectedItemIndex != 2 || b.HighlightedItemIndex != 3 || !b.Selected || b.DisplayItems) { std::printf("005c2086_TEST FAIL\n"); return 1; }
    CCategory_Retail n((CDefObj*)0); CCategory_Retail m(n); if (m.PDef.m_pObject != 0 || sizeof(CCategory_Retail) != 0x2c) { std::printf("005c2086_TEST FAIL\n"); return 1; }
    std::printf("005c2086_TEST PASS\n");
    return 0;
}
