#include <cstdio>
#include <cstring>
// Behaviour test for CTCInventory::RemoveContextSensitivePreferredItem (retail 0x005829CD).
class CCharString { public: const char* m_pData; };
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name)
    {
        if (std::strcmp(name.m_pData, "OBJECT_APPLE") == 0) return 40;
        if (std::strcmp(name.m_pData, "OBJECT_POTION") == 0) return 55;
        return 0;
    }
};
static CDefinitionManager g_defs;
struct CPreferredItem { long ItemType; long Reserved; long ButtonIndex; };
struct CPreferredItemVector {
    CPreferredItem* first; CPreferredItem* last; CPreferredItem* end_of_storage;
    CPreferredItem* begin() { return first; }
    CPreferredItem* end() { return last; }
    CPreferredItem* erase(CPreferredItem* where)
    {
        for (CPreferredItem* p = where; p + 1 != last; ++p) *p = *(p + 1);
        --last;
        return where;
    }
};
class CTCInventory { public:
    static CPreferredItemVector ContextSensitivePreferredItems;
    CDefinitionManager* GetDefinitions() { return &g_defs; }
    bool RemoveContextSensitivePreferredItem(const CCharString& itemName);
};
CPreferredItemVector CTCInventory::ContextSensitivePreferredItems;
bool CTCInventory::RemoveContextSensitivePreferredItem(const CCharString& itemName)
{
    long item = GetDefinitions()->GetDefGlobalIndexFromName(itemName);
    if (item > 0) {
        for (CPreferredItem* it = ContextSensitivePreferredItems.begin(); it != ContextSensitivePreferredItems.end(); ++it) {
            if (it->ItemType > 0 && it->ItemType == item) {
                ContextSensitivePreferredItems.erase(it);
                return true;
            }
        }
    }
    return false;
}
static int Fail() { std::printf("005829cd_TEST FAIL\n"); return 1; }
int main()
{
    CPreferredItem recs[3] = { { 12, 0, 1 }, { 40, 0, 2 }, { 7, 0, 3 } };
    CTCInventory::ContextSensitivePreferredItems.first = recs;
    CTCInventory::ContextSensitivePreferredItems.last = recs + 3;
    CTCInventory::ContextSensitivePreferredItems.end_of_storage = recs + 3;
    CTCInventory inv;
    CCharString apple = { "OBJECT_APPLE" }, potion = { "OBJECT_POTION" }, none = { "NOTHING" };
    if (!inv.RemoveContextSensitivePreferredItem(apple)) return Fail();
    if (CTCInventory::ContextSensitivePreferredItems.last != recs + 2 || recs[1].ItemType != 7) return Fail();
    if (inv.RemoveContextSensitivePreferredItem(apple)) return Fail();
    if (inv.RemoveContextSensitivePreferredItem(potion)) return Fail();
    if (inv.RemoveContextSensitivePreferredItem(none)) return Fail();
    if (CTCInventory::ContextSensitivePreferredItems.last != recs + 2) return Fail();
    std::printf("005829cd_TEST PASS\n");
    return 0;
}