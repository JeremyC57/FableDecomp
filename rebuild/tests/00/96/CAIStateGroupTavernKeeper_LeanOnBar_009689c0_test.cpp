// Behaviour test for CAIStateGroup_TavernKeeper::LeanOnBar (0x009689C0): same control flow over mocks.
#include <cstdio>
#include <cstring>
struct CKeeper {
    bool walkOk; int paymentAfter; int emptyJugAfter; int actionResult; int maxActions;
    int checks, actions, freed, terminated, pushable; float loopTime; const char* names[3];
    bool PaymentRequest() { return paymentAfter >= 0 && checks >= paymentAfter; }
    bool EmptyJug() { return emptyJugAfter >= 0 && checks >= emptyJugAfter; }
    bool Attention() { bool r = PaymentRequest() || EmptyJug(); ++checks; return r; }
    int Perform(const char* into, const char* loop, const char* out, float t)
    {
        names[0] = into; names[1] = loop; names[2] = out; loopTime = t; ++actions;
        return actions >= maxActions ? actionResult : 4;
    }
    bool LeanOnBar(float random)
    {
        pushable = 0;
        if (!walkOk) { ++freed; pushable = 1; ++terminated; return false; }
        if (Attention()) { ++freed; pushable = 1; return true; }
        float t = random + 1.0f;
        int result;
        do {
            if (Attention()) { ++freed; pushable = 1; return true; }
            result = Perform("BARMAN_LEAN_INTO", "BARMAN_LEAN_LOOP", "BARMAN_LEAN_OUTOF", t);
        } while (result == 4);
        ++freed; pushable = 1;
        return result == 2;
    }
};
static CKeeper Make(bool walk, int pay, int jug, int res, int maxA)
{
    CKeeper k; std::memset(&k, 0, sizeof(k));
    k.walkOk = walk; k.paymentAfter = pay; k.emptyJugAfter = jug; k.actionResult = res; k.maxActions = maxA; k.pushable = 1;
    return k;
}
int main()
{
    bool ok = true;
    CKeeper k = Make(false, -1, -1, 2, 1);
    ok = ok && !k.LeanOnBar(3.0f) && k.terminated == 1 && k.freed == 1 && k.pushable == 1 && k.actions == 0;
    k = Make(true, 0, -1, 2, 1);
    ok = ok && k.LeanOnBar(3.0f) && k.actions == 0 && k.freed == 1 && k.terminated == 0;
    k = Make(true, -1, -1, 2, 3);
    ok = ok && k.LeanOnBar(3.0f) && k.actions == 3 && k.loopTime == 4.0f && std::strcmp(k.names[2], "BARMAN_LEAN_OUTOF") == 0;
    k = Make(true, -1, -1, 1, 2);
    ok = ok && !k.LeanOnBar(0.5f) && k.actions == 2 && k.freed == 1;
    k = Make(true, -1, 3, 1, 10);
    ok = ok && k.LeanOnBar(0.0f) && k.actions == 2 && k.freed == 1 && k.pushable == 1;
    if (!ok) { std::printf("009689c0_TEST FAIL\n"); return 1; }
    std::printf("009689c0_TEST PASS\n");
    return 0;
}