// Behaviour test for 0x005BF32D CTCInventoryBase::AddItemToInventory
#include <cstdio>
static long g_augAdded = -1, g_catAdded = -1, g_notified = -1;
static bool g_addOk = true;
static bool g_hasAugComp = true;
struct CCategory { long ident; bool AddItem(long t, bool, long, long) { if (g_addOk) g_catAdded = t * 100 + ident; return g_addOk; } };
static bool IsAugmentation(long t) { return t >= 1000; }
static long GetInventoryCategory(long t) { return t % 3; }
static CCategory* FindCategory(CCategory* f, CCategory* l, long id) { for (; f != l; ++f) if (f->ident == id) break; return f; }
struct CInv {
    CCategory cats[3];
    void OnItemAdded(long t, bool, bool) { g_notified = t; }
    bool AddItemToInventory(long itemType, long count, bool silent, bool notify, long price, bool flag)
    {
        if (IsAugmentation(itemType)) {
            if (g_hasAugComp) g_augAdded = itemType;
        } else {
            CCategory* category = FindCategory(cats, cats + 3, GetInventoryCategory(itemType));
            if (category->AddItem(itemType, silent, price, count)) {
                OnItemAdded(itemType, notify, flag);
                return true;
            }
        }
        return false;
    }
};
int main()
{
    CInv inv; for (int i = 0; i < 3; ++i) inv.cats[i].ident = i;
    bool ok = true;
    if (inv.AddItemToInventory(1001, 1, false, true, -1, false)) ok = false;
    if (g_augAdded != 1001 || g_catAdded != -1) ok = false;
    if (!inv.AddItemToInventory(5, 1, false, true, -1, false)) ok = false;
    if (g_catAdded != 502 || g_notified != 5) ok = false;
    g_addOk = false; g_notified = -1;
    if (inv.AddItemToInventory(4, 1, false, true, -1, false) || g_notified != -1) ok = false;
    if (!ok) { std::printf("005bf32d_TEST FAIL\n"); return 1; }
    std::printf("005bf32d_TEST PASS\n");
    return 0;
}