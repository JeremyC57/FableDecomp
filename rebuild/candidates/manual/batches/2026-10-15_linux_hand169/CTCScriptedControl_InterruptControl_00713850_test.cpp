#include <cstdio>
// Behaviour model of CTCScriptedControl::InterruptControl (0x00713850): exit a started front action, clear the queue,
// notify the quest manager, null the handle, unlock.
struct Action { bool activated; int exits; };
struct Control { Action* queue[4]; int count; void* handle; bool locked; int notified;
    void Interrupt() {
        Action* a = count ? queue[0] : 0;
        if (a && a->activated) a->exits++;
        count = 0; notified++; handle = 0; locked = false;
    } };
int main()
{
    Action a = { true, 0 }, b = { false, 0 };
    int h;
    Control c = { { &a, &b, 0, 0 }, 2, &h, true, 0 };
    c.Interrupt();
    if (a.exits != 1 || c.count || c.handle || c.locked || c.notified != 1) { std::printf("00713850_TEST FAIL\n"); return 1; }
    Control d = { { &b, 0, 0, 0 }, 1, 0, true, 0 };
    d.Interrupt();
    if (b.exits != 0 || d.count) { std::printf("00713850_TEST FAIL\n"); return 1; }
    std::printf("00713850_TEST PASS\n");
    return 0;
}
