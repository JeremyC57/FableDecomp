// behaviour test for 0x0094D930 CAIStateGroup_TrollBase::IsStillRunnable() (model; the targeting component and the stop flag are stubs)
#include <cstdio>
class CThing { public: unsigned char dead; };
struct CTCTargeting { CThing* target; CThing* GetPTargetedThing() const { return target; } };
struct CGroup {
    CThing* enemy; bool stopping; CTCTargeting* tc;
    bool IsStillRunnable()
    {
        if (!(enemy && !(enemy->dead & 1))) return false;
        if (stopping) return false;
        return tc->GetPTargetedThing() == enemy;
    }
};
int main()
{
    CThing e = { 0 }, other = { 0 }; CTCTargeting tc = { &e }; CGroup g = { &e, false, &tc };
    int ok = g.IsStillRunnable();
    tc.target = &other; ok = ok && !g.IsStillRunnable();
    tc.target = &e; g.stopping = true; ok = ok && !g.IsStillRunnable();
    g.stopping = false; e.dead = 1; ok = ok && !g.IsStillRunnable();
    if (!ok) { std::printf("0094d930_TEST FAIL\n"); return 1; }
    std::printf("0094d930_TEST PASS\n");
    return 0;
}