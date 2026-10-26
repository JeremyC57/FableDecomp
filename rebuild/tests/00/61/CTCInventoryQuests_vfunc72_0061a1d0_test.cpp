#include <cstdio>
// Behaviour of CTCInventoryQuests::vfunc_72 (0x0061A1D0): index steps back and wraps to count - 1 below 0.
long g_testQuestIndex = 0;
long g_testQuestCount = 0;
class CTCInventoryQuests { public:
    void vfunc_72()
    {
        if (--g_testQuestIndex < 0)
            g_testQuestIndex = g_testQuestCount - 1;
    }
};
int main()
{
    CTCInventoryQuests q; g_testQuestCount = 3; g_testQuestIndex = 1;
    q.vfunc_72(); int bad = g_testQuestIndex != 0;
    q.vfunc_72(); bad |= g_testQuestIndex != 2;
    q.vfunc_72(); bad |= g_testQuestIndex != 1;
    if (bad) { std::printf("0061a1d0_TEST FAIL\n"); return 1; }
    std::printf("0061a1d0_TEST PASS\n");
    return 0;
}