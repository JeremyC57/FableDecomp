// Behaviour test for CTCInventoryQuests::MoveToNextTestQuest (0x0061A9D0): a model of the row -- only in test-quest mode, the
// selection steps forward; at (or past) the last displayed quest it wraps to the first and resets the window top; stepping
// to 12 rows below the window top scrolls the window by one.
#include <stdio.h>
struct Quests {
    bool ActiveTestQuestMode; long TestQuestSelectedIndex, TestQuestTopIndex; unsigned long Displayed;
    void MoveToNextTestQuest()
    {
        if (ActiveTestQuestMode) {
            unsigned long count = Displayed;
            if ((unsigned long)TestQuestSelectedIndex >= count - 1) {
                TestQuestSelectedIndex = 0;
                TestQuestTopIndex = 0;
            } else {
                ++TestQuestSelectedIndex;
                if (TestQuestSelectedIndex - TestQuestTopIndex >= 12)
                    ++TestQuestTopIndex;
            }
        }
    }
};
int main()
{
    Quests q = { true, 0, 0, 20 };
    q.MoveToNextTestQuest(); int ok = q.TestQuestSelectedIndex == 1 && q.TestQuestTopIndex == 0;
    q.TestQuestSelectedIndex = 11; q.MoveToNextTestQuest(); ok = ok && q.TestQuestSelectedIndex == 12 && q.TestQuestTopIndex == 1;
    q.TestQuestSelectedIndex = 19; q.MoveToNextTestQuest(); ok = ok && q.TestQuestSelectedIndex == 0 && q.TestQuestTopIndex == 0;
    Quests off = { false, 3, 1, 20 }; off.MoveToNextTestQuest(); ok = ok && off.TestQuestSelectedIndex == 3 && off.TestQuestTopIndex == 1;
    printf(ok ? "0061a9d0_TEST PASS\n" : "0061a9d0_TEST FAIL\n");
    return ok ? 0 : 1;
}