// Behaviour test for 0x00E0F8A0 NScript::CQ_WaspBossScript::CWaspHelper::Main: models the helper's lead-phase pacing (stray when the
// hero is not within 9 units and is further from the gate; else run when within 6.5 units or not further, walk between 6.5 and 8
// units), the plea line choice (ReachedWaspHelper timer expired -> the long-time line) and the entrance line choice
// (SavedVillagerCount < 2 -> NOT_MOVED, else NOT_KILLED_WASP_QUEEN).
#include <stdio.h>
#include <string.h>

enum { STRAY, RUN, WALK, KEEP };
static int Pace(float heroDist, float myGoalDist, float heroGoalDist, bool running)
{
    bool heroFurther = myGoalDist < heroGoalDist;
    if (!(heroDist < 9.0f) && heroFurther) return STRAY;
    if (!(heroDist < 6.5f) && heroFurther) {
        if (heroDist < 8.0f && running) return WALK;
        return KEEP;
    }
    return running ? KEEP : RUN;
}
static const char* PleaLine(long timer) { return timer == 0 ? "TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10" : "TEXT_QST_072_HELPER_FOLLOW_ME_10"; }
static const char* EntranceLine(long saved) { return saved < 2 ? "TEXT_QST_072_HELPER_NOT_MOVED_10" : "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10"; }

int main()
{
    int ok = 1;
    ok &= Pace(12.0f, 5.0f, 10.0f, true) == STRAY;
    ok &= Pace(12.0f, 10.0f, 5.0f, false) == RUN;
    ok &= Pace(7.0f, 5.0f, 10.0f, true) == WALK;
    ok &= Pace(7.0f, 5.0f, 10.0f, false) == KEEP;
    ok &= Pace(8.5f, 5.0f, 10.0f, true) == KEEP;
    ok &= Pace(3.0f, 5.0f, 10.0f, false) == RUN;
    ok &= strcmp(PleaLine(0), "TEXT_QST_072_HELPER_LONG_TIME_FOLLOW_ME_10") == 0;
    ok &= strcmp(PleaLine(3), "TEXT_QST_072_HELPER_FOLLOW_ME_10") == 0;
    ok &= strcmp(EntranceLine(1), "TEXT_QST_072_HELPER_NOT_MOVED_10") == 0;
    ok &= strcmp(EntranceLine(2), "TEXT_QST_072_HELPER_NOT_KILLED_WASP_QUEEN_10") == 0;
    if (ok) printf("00e0f8a0_TEST PASS\n"); else printf("00e0f8a0_TEST FAIL\n");
    return ok ? 0 : 1;
}