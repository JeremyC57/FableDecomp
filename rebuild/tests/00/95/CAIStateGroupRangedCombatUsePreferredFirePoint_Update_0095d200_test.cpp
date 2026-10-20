// behaviour test for CAIStateGroup_RangedCombatUsePreferredFirePoint::Update (0x0095D200): logic copy over stubs
#include <cstdio>
struct Group {
    int runnableBudget;          // IsStillRunnable returns true this many times
    long strafeResults[4]; int strafeIndex;
    int attacksLeft;
    int registered, deregistered, terminated, attacks;
    long timeOut, frame;
    bool IsStillRunnable() { if (runnableBudget > 0) { runnableBudget--; return true; } return false; }
    bool AttackEnemy() { if (attacksLeft > 0) { attacksLeft--; attacks++; return true; } return false; }
    long Strafe() { return strafeResults[strafeIndex++]; }
    void Terminate() { terminated++; }
    void Update()
    {
        if (!IsStillRunnable()) { Terminate(); return; }
        registered++;
        long result;
        do { result = Strafe(); } while (result == 4);
        if (result != 2) { Terminate(); return; }
        if (!IsStillRunnable()) { Terminate(); return; }
        timeOut = frame;
        while (AttackEnemy()) { if (!IsStillRunnable()) { Terminate(); return; } }
        if (!IsStillRunnable()) { Terminate(); return; }
        deregistered++;
    }
};
int main()
{
    Group g = { 10, { 4, 4, 2, 0 }, 0, 2, 0, 0, 0, 0, 0, 77 };
    g.Update();
    bool ok = g.registered == 1 && g.strafeIndex == 3 && g.attacks == 2 && g.deregistered == 1 && g.terminated == 0 && g.timeOut == 77;
    Group h = { 10, { 1, 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0, 5 };
    h.Update();
    if (h.terminated != 1 || h.deregistered != 0) ok = false;          // strafe failed
    Group k = { 3, { 2, 0, 0, 0 }, 0, 5, 0, 0, 0, 0, 0, 5 };
    k.Update();
    if (k.terminated != 1 || k.attacks != 2 || k.deregistered != 0) ok = false;   // ran out of runnability mid-attack
    if (!ok) { std::printf("0095d200_TEST FAIL\n"); return 1; }
    std::printf("0095d200_TEST PASS\n");
    return 0;
}