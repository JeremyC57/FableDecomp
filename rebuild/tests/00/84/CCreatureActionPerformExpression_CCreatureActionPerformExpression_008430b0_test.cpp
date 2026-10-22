// Behaviour test for CCreatureAction_PerformExpression ctor (0x008430B0): the base takes the def's Animation and the
// duration; zero duration zeroes TotalTime; otherwise an extendable def prefers ToOtherInto (with a target), then Into
// (both mark Extended), else Animation; a non-extendable def switches to ToOther only with a target.
#include <cstdio>
#include <cstring>

struct Def { const char* anim; const char* into; const char* toOther; const char* toOtherInto; bool extendable; };
struct Action { const char* current; float duration; int totalTime; bool extended; };

static void Construct(Action& a, const Def& d, bool hasTarget, int duration)
{
    a.current = d.anim; a.duration = (float)duration; a.totalTime = 99; a.extended = false;
    if (duration == 0.0f) { a.totalTime = 0; return; }
    if (d.extendable) {
        if (hasTarget && std::strcmp(d.toOtherInto, "") != 0) { a.current = d.toOtherInto; a.extended = true; }
        else if (std::strcmp(d.into, "") != 0) { a.current = d.into; a.extended = true; }
        else a.current = d.anim;
    } else if (hasTarget && std::strcmp(d.toOther, "") != 0) {
        a.current = d.toOther;
    }
}

int main()
{
    Def d = { "WAVE", "WAVE_INTO", "WAVE_AT", "WAVE_AT_INTO", true };
    Action a;
    Construct(a, d, true, 0);
    int ok = a.totalTime == 0 && std::strcmp(a.current, "WAVE") == 0;
    Construct(a, d, true, 2);
    ok = ok && std::strcmp(a.current, "WAVE_AT_INTO") == 0 && a.extended && a.totalTime == 99;
    Construct(a, d, false, 2);
    ok = ok && std::strcmp(a.current, "WAVE_INTO") == 0 && a.extended;
    d.into = "";
    Construct(a, d, false, 2);
    ok = ok && std::strcmp(a.current, "WAVE") == 0 && !a.extended;
    d.extendable = false;
    Construct(a, d, true, 3);
    ok = ok && std::strcmp(a.current, "WAVE_AT") == 0 && !a.extended;
    Construct(a, d, false, 3);
    ok = ok && std::strcmp(a.current, "WAVE") == 0;
    std::printf(ok ? "008430b0_TEST PASS\n" : "008430b0_TEST FAIL\n");
    return ok ? 0 : 1;
}