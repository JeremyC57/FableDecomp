// test: NScript::CV_FisticuffsClubScript::SetCompletedThisClub -- clear, then copy the current region's club-completed flag unless the thread is terminating; unknown regions leave it clear
#include <stdio.h>
struct Script {
    long RelevantRegion; bool Terminating;
    bool CompletedThisPunchClub, CompletedKnotholePunchClub, CompletedBowerstonePunchClub, CompletedBanditPunchClub, CompletedOakvalePunchClub;
    bool IsActiveThreadTerminating() { return Terminating; }
    void SetCompletedThisClub() {
        CompletedThisPunchClub = false;
        bool completed;
        switch (RelevantRegion) {
        case 1: completed = CompletedBowerstonePunchClub; break;
        case 4: completed = CompletedKnotholePunchClub; break;
        case 2: completed = CompletedOakvalePunchClub; break;
        case 3: completed = CompletedBanditPunchClub; break;
        default: return;
        }
        if (completed) { if (IsActiveThreadTerminating()) return; CompletedThisPunchClub = true; }
    }
};
static bool run(long region, bool term, bool k, bool b, bool ba, bool o) {
    Script s; s.RelevantRegion = region; s.Terminating = term; s.CompletedThisPunchClub = true;
    s.CompletedKnotholePunchClub = k; s.CompletedBowerstonePunchClub = b; s.CompletedBanditPunchClub = ba; s.CompletedOakvalePunchClub = o;
    s.SetCompletedThisClub();
    return s.CompletedThisPunchClub;
}
int main() {
    bool ok = run(1, false, false, true, false, false) && !run(1, false, true, false, true, true)
           && run(2, false, false, false, false, true) && run(3, false, false, false, true, false)
           && run(4, false, true, false, false, false) && !run(4, true, true, true, true, true)
           && !run(0, false, true, true, true, true) && !run(5, false, true, true, true, true);
    if (!ok) { printf("00e827a0_TEST FAIL\n"); return 1; }
    printf("00e827a0_TEST PASS\n");
    return 0;
}