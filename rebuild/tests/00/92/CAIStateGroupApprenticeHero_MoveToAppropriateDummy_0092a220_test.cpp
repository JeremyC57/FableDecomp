// Behaviour test for CAIStateGroup_ApprenticeHero::MoveToAppropriateDummy (0x0092A220): choose point/proximity by action,
// retry while the move returns 4, fail when stopping, when the type is in use, or when no ranged target exists.
#include <cstdio>
static bool g_inUse[3];
static int g_moveResults[8]; static int g_moveIndex; static bool g_stopping; static int g_terminated; static float g_lastProximity;
struct Group {
    int Action; int hasRanged;
    int Move(float proximity) { g_lastProximity = proximity; return g_moveResults[g_moveIndex++]; }
    static bool IsDummyTypeLegal(int t) { switch (t) { case 0: return !g_inUse[0]; case 1: return !g_inUse[1]; case 2: return !g_inUse[2]; } return false; }
    bool MoveToAppropriateDummy()
    {
        float proximity;
        switch (Action) {
        case 0: proximity = 0.25f + 1.0f; break;
        case 1: if (!hasRanged) return false; proximity = 0.0f; break;
        case 2: proximity = 0.5f + 2.0f; break;
        default: return false;
        }
        int result;
        do {
            result = Move(proximity);
            if (g_stopping) { ++g_terminated; return false; }
            int action = Action;
            if (!IsDummyTypeLegal(action)) return false;
        } while (result == 4);
        return result == 2;
    }
};
int main()
{
    int ok = 1;
    Group g; g.hasRanged = 0;
    g.Action = 0; g_moveIndex = 0; g_moveResults[0] = 4; g_moveResults[1] = 4; g_moveResults[2] = 2;
    if (!g.MoveToAppropriateDummy() || g_moveIndex != 3 || g_lastProximity != 1.25f) ok = 0;
    g.Action = 1; g_moveIndex = 0;
    if (g.MoveToAppropriateDummy() || g_moveIndex != 0) ok = 0;      // no ranged target
    g.Action = 2; g_moveIndex = 0; g_moveResults[0] = 1;
    if (g.MoveToAppropriateDummy() || g_lastProximity != 2.5f) ok = 0;  // not arrived
    g_inUse[2] = true; g_moveIndex = 0; g_moveResults[0] = 2;
    if (g.MoveToAppropriateDummy()) ok = 0;                          // type in use
    g_inUse[2] = false; g_stopping = true; g_moveIndex = 0;
    if (g.MoveToAppropriateDummy() || g_terminated != 1) ok = 0;     // stopping terminates
    g.Action = 7; if (g.MoveToAppropriateDummy()) ok = 0;
    std::printf(ok ? "0092a220_TEST PASS\n" : "0092a220_TEST FAIL\n");
    return ok ? 0 : 1;
}