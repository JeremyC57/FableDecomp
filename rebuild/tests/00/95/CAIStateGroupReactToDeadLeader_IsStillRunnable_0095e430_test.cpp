// behaviour test for 0x0095E430 CAIStateGroup_ReactToDeadLeader::IsStillRunnable
#include <cstdio>
class CThing { public: unsigned char Dead : 1; bool IsDead() const { return Dead; } };
static long g_budget = 100;
struct Group {
    bool stopping; CThing* enemy; long frames;
    bool IsStillRunnable() const
    {
        if (!stopping && (enemy && !enemy->IsDead())) {
            if (frames < g_budget) return true;
        }
        return false;
    }
};
int main()
{
    CThing alive = { 0 }, dead = { 1 };
    Group a = { false, &alive, 5 }, b = { true, &alive, 5 }, c = { false, &dead, 5 }, d = { false, 0, 5 }, e = { false, &alive, 100 };
    bool ok = a.IsStillRunnable() && !b.IsStillRunnable() && !c.IsStillRunnable() && !d.IsStillRunnable() && !e.IsStillRunnable();
    std::printf(ok ? "0095e430_TEST PASS\n" : "0095e430_TEST FAIL\n");
    return ok ? 0 : 1;
}