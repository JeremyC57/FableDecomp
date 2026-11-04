// Behaviour test for CAIStateGroup_TavernKeeper::GetPaymentFinishedMessage (0x00968460): a model of the row.
#include <stdio.h>

struct Msg { int type; int from, to; bool removed; };
static bool GetMessage(bool payeeAlive, int payee, int me, Msg* msgs, int n)
{
    if (!payeeAlive) return false;
    for (int i = 0; i < n; ++i)
        if (msgs[i].type == 48 && msgs[i].from == payee && msgs[i].to == me && !msgs[i].removed) { msgs[i].removed = true; return true; }
    return false;
}
static int run()
{
    Msg m[3] = { { 48, 2, 9, false }, { 48, 1, 9, false }, { 0x2f, 1, 9, false } };
    int ok = GetMessage(true, 1, 9, m, 3) && m[1].removed && !m[0].removed;
    ok = ok && !GetMessage(true, 1, 9, m, 3) && !GetMessage(false, 2, 9, m, 3) && !m[0].removed;
    return ok;
}
int main()
{
    int ok = run();
    printf(ok ? "00968460_TEST PASS\n" : "00968460_TEST FAIL\n");
    return ok ? 0 : 1;
}
