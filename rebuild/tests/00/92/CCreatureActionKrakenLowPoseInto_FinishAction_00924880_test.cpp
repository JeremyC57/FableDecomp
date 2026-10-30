// behaviour test for 0x00924880 CCreatureAction_KrakenLowPoseInto::FinishAction
#include <cstdio>
class CThing { public: unsigned char Dead : 1; bool IsDead() const { return Dead; } };
static int g_baseFinished, g_next;   // 1 = charge loop (loop 0), 2 = attack out-of
struct Action {
    CThing* target;
    bool IsTargetAlive() const { return target && !target->IsDead(); }
    void FinishAction() { ++g_baseFinished; if (IsTargetAlive()) g_next = 1; else g_next = 2; }
};
int main()
{
    CThing alive = { 0 }, dead = { 1 };
    Action a = { &alive }, b = { &dead }, c = { 0 };
    bool ok = true;
    a.FinishAction(); ok = ok && g_next == 1;
    b.FinishAction(); ok = ok && g_next == 2;
    c.FinishAction(); ok = ok && g_next == 2 && g_baseFinished == 3;
    std::printf(ok ? "00924880_TEST PASS\n" : "00924880_TEST FAIL\n");
    return ok ? 0 : 1;
}