// Behaviour test for CPanel::GetItemStatistics (0x00C12300): zero five counters, let each active item count itself, copy the +0x370 total.
#include <stdio.h>
static int g_locked = 0;
struct Stats { long c0, c1, c2, c3, c4, total; };
struct Item { long state; long active; long IsActive() { return active; } void CountInto(Stats* s) { if (state == 0) ++s->c0; else if (state == 1) ++s->c1; else if (state == 2) ++s->c2; else if (state == 3) ++s->c3; else ++s->c4; } };
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Panel {
    Item items[5]; unsigned long count; long m_total;
    unsigned long GetNumItems() { return count; }
    Item* GetItem(unsigned long i) { return &items[i]; }
    long GetTotal() const { return m_total; }
    void GetItemStatistics(Stats* stats)
    {
        Lock lock;
        stats->c3 = 0; stats->c4 = 0; stats->c1 = 0; stats->c2 = 0; stats->c0 = 0;
        for (unsigned long i = 0; i < GetNumItems(); ++i) { Item* item = GetItem(i); if (item->IsActive()) item->CountInto(stats); }
        stats->total = GetTotal();
    }
};
int main()
{
    Panel p; p.count = 5; p.m_total = 77;
    long st[5] = { 0, 1, 1, 3, 4 }, ac[5] = { 1, 1, 0, 1, 1 };
    for (int i = 0; i < 5; i++) { p.items[i].state = st[i]; p.items[i].active = ac[i]; }
    Stats s = { 9, 9, 9, 9, 9, 9 };
    p.GetItemStatistics(&s);
    if (s.c0 != 1 || s.c1 != 1 || s.c2 != 0 || s.c3 != 1 || s.c4 != 1 || s.total != 77 || g_locked != 0) { printf("00c12300_TEST FAIL\n"); return 1; }
    printf("00c12300_TEST PASS\n"); return 0;
}