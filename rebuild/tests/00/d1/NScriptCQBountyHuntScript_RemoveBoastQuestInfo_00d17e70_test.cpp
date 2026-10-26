// Behaviour test for 0x00d17e70: NScript::CQ_BountyHuntScript::RemoveBoastQuestInfo -- hide the quest info, clear the
// displayed flag (before the element is removed), remove the boast timer element.
#include <stdio.h>
static int g_seq[2]; static int g_n; static bool g_shown = true; static long g_removed = -1; static bool g_flagAtRemove = true;
struct GSI;
struct Script;
static Script* g_s;
struct GSI {
    void DisplayQuestInfo(bool show) { g_shown = show; g_seq[g_n++] = 1; }
    void RemoveQuestInfoElement(long e);
};
struct Script {
    GSI* gsi; long BoastTimerID; bool BoastQuestInfoDisplayed;
    void RemoveBoastQuestInfo() { gsi->DisplayQuestInfo(false); BoastQuestInfoDisplayed = false; gsi->RemoveQuestInfoElement(BoastTimerID); }
};
void GSI::RemoveQuestInfoElement(long e) { g_removed = e; g_seq[g_n++] = 2; g_flagAtRemove = g_s->BoastQuestInfoDisplayed; }
int main()
{
    GSI gsi; Script s; s.gsi = &gsi; s.BoastTimerID = 77; s.BoastQuestInfoDisplayed = true; g_s = &s;
    s.RemoveBoastQuestInfo();
    bool ok = g_n == 2 && g_seq[0] == 1 && g_seq[1] == 2 && !g_shown && g_removed == 77 && !s.BoastQuestInfoDisplayed && !g_flagAtRemove;
    if (!ok) { printf("00d17e70_TEST FAIL\n"); return 1; }
    printf("00d17e70_TEST PASS\n"); return 0;
}
