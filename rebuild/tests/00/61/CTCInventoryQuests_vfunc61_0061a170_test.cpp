#include <cstdio>
// Behaviour of CTCInventoryQuests::vfunc_61 (0x0061A170): toggles show-all and rewinds both indices in test mode only.
struct CTCInventoryQuests {
    bool ActiveTestQuestMode; long TestQuestSelectedIndex; long TestQuestTopIndex; bool TestQuestShowAll;
    void vfunc_61()
    {
        if (ActiveTestQuestMode) {
            TestQuestShowAll = !TestQuestShowAll;
            TestQuestSelectedIndex = 0;
            TestQuestTopIndex = 0;
        }
    }
};
int main()
{
    CTCInventoryQuests q; q.ActiveTestQuestMode = false; q.TestQuestSelectedIndex = 4; q.TestQuestTopIndex = 2; q.TestQuestShowAll = false;
    q.vfunc_61();
    int bad = (q.TestQuestShowAll || q.TestQuestSelectedIndex != 4 || q.TestQuestTopIndex != 2);
    q.ActiveTestQuestMode = true; q.vfunc_61();
    bad |= (!q.TestQuestShowAll || q.TestQuestSelectedIndex != 0 || q.TestQuestTopIndex != 0);
    q.TestQuestSelectedIndex = 3; q.vfunc_61();
    bad |= (q.TestQuestShowAll || q.TestQuestSelectedIndex != 0);
    if (bad) { std::printf("0061a170_TEST FAIL\n"); return 1; }
    std::printf("0061a170_TEST PASS\n");
    return 0;
}