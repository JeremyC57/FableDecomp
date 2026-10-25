// behaviour test for 0x00C20F60 CPanelItem::CountInto: bump the statistics counter that belongs to the item state
#include <cstdio>

struct CPanelItemStats {
    long m_count0, m_count1, m_count2, m_count3, m_count4, m_total;
};

static int g_stateChecks = 0;

struct CPanelItem {
    unsigned short m_state;
    void ChangeState(long) { ++g_stateChecks; }                   // stand-in for 0x00C20720
    void CountInto(CPanelItemStats* stats);
};

void CPanelItem::CountInto(CPanelItemStats* stats)
{
    ChangeState(-1);
    switch (m_state) {
    case 6: ++stats->m_count3; return;
    case 3: ++stats->m_count0; return;
    case 5: ++stats->m_count4; return;
    case 4: ++stats->m_count2; return;
    case 2: ++stats->m_count1; return;
    }
}

int main()
{
    CPanelItemStats s = { 0, 0, 0, 0, 0, 0 };
    CPanelItem item;
    const unsigned short states[] = { 0, 1, 2, 3, 3, 4, 5, 5, 5, 6, 7, 9 };
    for (int i = 0; i < 12; ++i) { item.m_state = states[i]; item.CountInto(&s); }
    // state 3 -> +0, 2 -> +4, 4 -> +8, 6 -> +0xC, 5 -> +0x10 (retail jump table)
    if (s.m_count0 != 2 || s.m_count1 != 1 || s.m_count2 != 1 || s.m_count3 != 1 || s.m_count4 != 3 || s.m_total != 0 || g_stateChecks != 12) {
        std::printf("00c20f60_TEST FAIL\n"); return 1;
    }
    std::printf("00c20f60_TEST PASS\n");
    return 0;
}