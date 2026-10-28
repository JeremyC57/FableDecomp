// Behaviour test: CAIStateGroup_UsesSeat::GetUpFromSeat (0x00938A10) -- pick the bench (mode 9, "ST_STAND_UP_FROM_BENCH")
// or seat (mode 8, "ST_STAND_UP_FROM_SEAT") get-up, clear Sitting, free the seat's action point, clear GotSeatPos and
// the index, perform the get-up action (priority 50); a result other than 2 removes the sitting mode and fails.
#include <cstdio>
#include <cstring>
static int g_freedIndex = -100, g_removedMode = -1, g_actionMode = -1, g_actionPriority;
static char g_actionAnim[32];
static int g_actionResult;
static int PerformActionAndWait(int priority, const char* anim, int mode, bool remove)
{
    g_actionPriority = priority; std::strcpy(g_actionAnim, anim); g_actionMode = mode; (void)remove;
    return g_actionResult;
}
struct CUsesSeat {
    bool GotSeatPos, Sitting; int SeatPosIndex; bool SittingOnBench;
    bool GetUpFromSeat()
    {
        const char* anim; int mode;
        if (SittingOnBench) { anim = "ST_STAND_UP_FROM_BENCH"; mode = 9; }
        else { anim = "ST_STAND_UP_FROM_SEAT"; mode = 8; }
        Sitting = false;
        g_freedIndex = SeatPosIndex;
        GotSeatPos = false;
        SeatPosIndex = -1;
        if (PerformActionAndWait(50, anim, mode, false) != 2) { g_removedMode = mode; return false; }
        return true;
    }
};
int main()
{
    CUsesSeat s = { true, true, 3, false };
    g_actionResult = 2;
    bool ok = s.GetUpFromSeat() && !s.Sitting && !s.GotSeatPos && s.SeatPosIndex == -1 && g_freedIndex == 3
        && g_actionMode == 8 && g_actionPriority == 50 && std::strcmp(g_actionAnim, "ST_STAND_UP_FROM_SEAT") == 0 && g_removedMode == -1;
    CUsesSeat b = { true, true, 1, true };
    g_actionResult = 0;
    ok = ok && !b.GetUpFromSeat() && g_actionMode == 9 && g_removedMode == 9
        && std::strcmp(g_actionAnim, "ST_STAND_UP_FROM_BENCH") == 0;
    if (!ok) { std::printf("00938a10_TEST FAIL\n"); return 1; }
    std::printf("00938a10_TEST PASS\n");
    return 0;
}