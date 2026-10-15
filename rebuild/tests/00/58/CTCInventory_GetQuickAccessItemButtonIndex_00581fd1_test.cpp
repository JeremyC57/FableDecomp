#include <cstdio>
// Behaviour test for CTCInventory::GetQuickAccessItemButtonIndex (retail 0x00581FD1).
static int g_countCalls = 0;
struct CItemList { unsigned long GetCount() { ++g_countCalls; return 3; } };
struct CPlayer { CItemList* m_pItems; };
static CItemList g_list;
static CPlayer g_player;
struct CWorld { CPlayer* GetMainPlayer() { return &g_player; } };
static CWorld g_world;
CWorld* GetWorld() { return &g_world; }

struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventory {
    CLongVector QuickAccessItems;
    long GetQuickAccessItemButtonIndex(long item);
};
long CTCInventory::GetQuickAccessItemButtonIndex(long item)
{
    GetWorld()->GetMainPlayer()->m_pItems->GetCount();
    for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
        if (QuickAccessItems[i] == item)
            return i;
    }
    return -1;
}
int main()
{
    g_player.m_pItems = &g_list;
    long items[3] = { 5, 9, 0 };
    CTCInventory inv;
    inv.QuickAccessItems.first = items; inv.QuickAccessItems.last = items + 3; inv.QuickAccessItems.end_of_storage = items + 3;
    if (inv.GetQuickAccessItemButtonIndex(9) != 1 || inv.GetQuickAccessItemButtonIndex(5) != 0 ||
        inv.GetQuickAccessItemButtonIndex(77) != -1 || g_countCalls != 3) { std::printf("00581fd1_TEST FAIL\n"); return 1; }
    std::printf("00581fd1_TEST PASS\n");
    return 0;
}