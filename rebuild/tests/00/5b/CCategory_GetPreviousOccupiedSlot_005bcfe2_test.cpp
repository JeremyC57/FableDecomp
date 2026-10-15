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
#define FAIL { std::printf("005bcfe2_TEST FAIL\n"); return 1; }
struct CCategory_Retail {
    CItemVector ItemsInCategory; CDefPointer PDef;
    long F(long index)
    {
    for (long i = index - 1; i >= 0; --i) { if (ItemsInCategory[i].NumberOfItems != 0) return i; }
    if (PDef->WrapHighlightCursor) { for (long j = (long)ItemsInCategory.size() - 1; j >= index; --j) { if (ItemsInCategory[j].NumberOfItems != 0) return j; } }
    return index;
    }
};
int main()
{
    CItem items[5];
    for (int i = 0; i < 5; ++i) { items[i].ItemType = 10 + i; items[i].NumberOfItems = 0; }
    items[0].NumberOfItems = 1; items[2].NumberOfItems = 3; items[4].NumberOfItems = 1;
    CInventoryCategoryDef_Retail def; def.m_refCount = 1; def.WrapHighlightCursor = false; def.SelectEmptySlots = false;
    CCategory_Retail c; c.ItemsInCategory.first = items; c.ItemsInCategory.last = items + 5; c.ItemsInCategory.end_of_storage = items + 5;
    c.PDef.m_pObject = &def;
    if (c.F(4) != 2 || c.F(2) != 0 || c.F(0) != 0) FAIL; def.WrapHighlightCursor = true; if (c.F(0) != 4 || c.F(3) != 2) FAIL;
    if (def.m_refCount != 1 || g_destroyed != 0) FAIL;
    std::printf("005bcfe2_TEST PASS\n");
    return 0;
}