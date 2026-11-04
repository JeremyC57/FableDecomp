// Behaviour test for CAIStateGroupBehaviourBase::UpdateFocusAttentionOnThingActive (0x009578A0, parked DIFFER row): a model of the row -- while attentive and focused keep walking (running); on arrival turn to face, else false.
#include <stdio.h>
enum { RUNNING = 4, SUCCEEDED = 2, FAILED = 1 };
static int g_steps[4]; static int g_step; static bool g_turned;
static bool Update(bool attentive) { for (;;) { if (!attentive) return false; int r = g_steps[g_step++]; if (r == RUNNING) continue; if (r == SUCCEEDED) { g_turned = true; return true; } return false; } }
static int Check() { g_steps[0] = RUNNING; g_steps[1] = SUCCEEDED; g_step = 0; int ok = Update(true) && g_turned && g_step == 2; g_steps[0] = FAILED; g_step = 0; g_turned = false; return ok && !Update(true) && !g_turned && !Update(false); }
int main()
{
    int ok = Check();
    printf(ok ? "009578a0_TEST PASS\n" : "009578a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
