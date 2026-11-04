// Behaviour test for NTCScriptedControl::CActionPlayCombatAnimation::Update (0x00712A80): a model of the row.
#include <stdio.h>

struct Action { bool started, finished, wait, idle; int queued; };
static void Update(Action& a)
{
    if (!a.started) { a.started = true; a.queued++; if (!a.wait) a.finished = true; return; }
    if (a.idle) a.finished = true;
}
static int run()
{
    Action a = { false, false, false, false, 0 }, b = { false, false, true, false, 0 };
    Update(a); Update(b); int ok = a.finished && a.queued == 1 && !b.finished;
    Update(b); ok = ok && !b.finished; b.idle = true; Update(b); ok = ok && b.finished && b.queued == 1;
    return ok;
}
int main()
{
    int ok = run();
    printf(ok ? "00712a80_TEST PASS\n" : "00712a80_TEST FAIL\n");
    return ok ? 0 : 1;
}
