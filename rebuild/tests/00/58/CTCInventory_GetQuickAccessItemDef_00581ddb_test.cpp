#include <cstdio>
// Behaviour test for CTCInventory::GetQuickAccessItemDef (retail 0x00581DDB).
class CInventoryItemDef { public: long id; };
static CInventoryItemDef g_defs[300];

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { }
};
static int g_getCalls = 0;
CDefPointer<const CInventoryItemDef> GetPDef(long itemType) { ++g_getCalls; return CDefPointer<const CInventoryItemDef>(&g_defs[itemType]); }

struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};
class CTCInventory { public:
    CLongVector QuickAccessItems;
    CLongVector ContextSensitiveItems;
    long GetQuickAccessItem(unsigned long index)
    {
        if (index < ContextSensitiveItems.size())
            return ContextSensitiveItems[index];
        return QuickAccessItems[index - ContextSensitiveItems.size()];
    }
    CDefPointer<const CInventoryItemDef> GetQuickAccessItemDef(unsigned long index);
};
CDefPointer<const CInventoryItemDef> CTCInventory::GetQuickAccessItemDef(unsigned long index)
{
    long item = GetQuickAccessItem(index);
    if (item > 0)
        return GetPDef(item);
    return CDefPointer<const CInventoryItemDef>();
}
int main()
{
    long cs[1] = { 7 };
    long qa[2] = { 0, 250 };
    CTCInventory inv;
    inv.ContextSensitiveItems.first = cs; inv.ContextSensitiveItems.last = cs + 1; inv.ContextSensitiveItems.end_of_storage = cs + 1;
    inv.QuickAccessItems.first = qa; inv.QuickAccessItems.last = qa + 2; inv.QuickAccessItems.end_of_storage = qa + 2;
    if (inv.GetQuickAccessItemDef(0).Object != &g_defs[7] || inv.GetQuickAccessItemDef(1).Object != 0 ||
        inv.GetQuickAccessItemDef(2).Object != &g_defs[250] || g_getCalls != 2) { std::printf("00581ddb_TEST FAIL\n"); return 1; }
    std::printf("00581ddb_TEST PASS\n");
    return 0;
}