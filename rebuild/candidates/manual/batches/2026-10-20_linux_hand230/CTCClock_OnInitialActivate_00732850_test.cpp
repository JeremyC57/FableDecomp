// Behaviour test for CTCClock::OnInitialActivate (0x00732850): the time reset, the def references balanced, and the
// main animation started with the looping "CLOCKHAND_TURN" index.
#include <cstdio>

struct Def { long refs; int destroyed; long animIndex; };
struct Init { long type, arg24, index, mode; bool loop; unsigned long flags; long percent; };
static Init g_started; static int g_starts = 0;
struct Clock {
    float LastTimeUpdated; Def* baseDef; Def* appearance;
    void OnInitialActivate()
    {
        LastTimeUpdated = 0.5f;
        if (baseDef) { ++baseDef->refs; if (--baseDef->refs == 0) baseDef->destroyed = 1; }
        Def* def = appearance;
        if (def) ++def->refs;                 // GetSubDef hands out a counted reference
        Init init = { 0, 0, def->animIndex, 1, true, 0, 100 };
        g_started = init; ++g_starts;
        if (def && --def->refs == 0) def->destroyed = 1;
    }
};

int main()
{
    Def base = { 1, 0, 0 }, app = { 1, 0, 42 };
    Clock c = { 0.0f, &base, &app };
    c.OnInitialActivate();
    if (c.LastTimeUpdated != 0.5f || g_starts != 1 || g_started.index != 42 || !g_started.loop || g_started.percent != 100) {
        std::printf("00732850_TEST FAIL\n"); return 1;
    }
    if (base.refs != 1 || base.destroyed || app.refs != 1 || app.destroyed) { std::printf("00732850_TEST FAIL\n"); return 1; }
    std::printf("00732850_TEST PASS\n");
    return 0;
}
