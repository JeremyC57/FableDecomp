#include <cstdio>
// Behaviour of CTCInventoryQuests::vfunc_71 (0x0061A1A0): index steps forward and wraps to 0 after count - 1.
long g_testQuestIndex = 0;
long g_testQuestCount = 0;
struct CTCInventoryQuests {
    void vfunc_71()
    {
        if (++g_testQuestIndex > g_testQuestCount - 1)
            g_testQuestIndex = 0;
    }
};
int main()
{
    CTCInventoryQuests q; g_testQuestCount = 3; g_testQuestIndex = 0;
    q.vfunc_71(); int bad = g_testQuestIndex != 1;
    q.vfunc_71(); bad |= g_testQuestIndex != 2;
    q.vfunc_71(); bad |= g_testQuestIndex != 0;
    g_testQuestCount = 0; g_testQuestIndex = 0; q.vfunc_71(); bad |= g_testQuestIndex != 0;
    if (bad) { std::printf("0061a1a0_TEST FAIL\n"); return 1; }
    std::printf("0061a1a0_TEST PASS\n");
    return 0;
}