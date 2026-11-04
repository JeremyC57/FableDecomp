// Behaviour test for CTCEntityEvents::GetEvent<CEventHitBy, Bind3<melee, !blocked, hit by>> (0x00738A80): a model of the row.
#include <stdio.h>

struct Event { int type; bool melee, blocked; int by; };
static const Event* GetEvent(const Event* events, int n, int by)
{
    for (int i = n - 1; i >= 0; --i)
        if (events[i].type == 1 && events[i].melee && !events[i].blocked && events[i].by == by) return &events[i];
    return 0;
}
static int run()
{
    Event e[4] = { { 1, true, false, 7 }, { 2, true, false, 7 }, { 1, true, false, 7 }, { 1, true, true, 7 } };
    return GetEvent(e, 4, 7) == &e[2] && GetEvent(e, 4, 8) == 0 && GetEvent(e, 1, 7) == &e[0];
}
int main()
{
    int ok = run();
    printf(ok ? "00738a80_TEST PASS\n" : "00738a80_TEST FAIL\n");
    return ok ? 0 : 1;
}
