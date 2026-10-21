// Behaviour test for 00439f20 (CPlayerGui::UpdateMorality): an uninitialised bar is set to the pending change; an
// initialised bar is animated with a step count from its remaining distance (at least 1) and its graphic frame
// advanced; a change whose sign differs from the bar's old value hides the bar; in GUI mode 1 the change is consumed.
#include <cstdio>
#include <cmath>

struct Bar { bool init; float cur, old, lastOld; long mode; unsigned long frame, oldFrame; };
struct Gui {
    Bar bar; long pending; bool heroAlive; float lastValue, lastSteps; int updates;
    void UpdateStat(float value, float rate, float steps) { (void)rate; lastValue = value; lastSteps = steps; ++updates; }
    void UpdateMorality()
    {
        if (!heroAlive) return;
        long change = pending;
        float value;
        if (bar.init) {
            long steps = (long)((std::fabs(bar.old - bar.cur) - 0.2f) * 0.5f);
            if (steps < 1) steps = 1;
            value = (float)change;
            UpdateStat(value, 0.01f, (float)steps);
            bar.oldFrame = bar.frame; ++bar.frame;
        } else {
            value = (float)change;
            bar.cur = value; bar.old = value; bar.lastOld = value; bar.init = true;
        }
        if (change != 0) {
            if ((value < 0.0f ? -1 : 1) != (bar.old < 0.0f ? -1 : 1)) bar.init = false;
            if (bar.mode == 1) { pending = 0; bar.init = false; }
        }
    }
};
int main()
{
    Gui g = {}; g.heroAlive = true; g.pending = 5;
    g.UpdateMorality();
    int ok = g.bar.init && g.bar.cur == 5.0f && g.updates == 0;
    g.bar.old = 10.0f; g.bar.cur = 2.0f; g.UpdateMorality();
    if (g.updates != 1 || g.lastSteps != 3.0f || g.lastValue != 5.0f || g.bar.frame != 1 || !g.bar.init) ok = 0;
    g.pending = -3; g.UpdateMorality();
    if (g.bar.init) ok = 0;
    g.bar.init = true; g.bar.mode = 1; g.pending = 4; g.bar.old = 1.0f; g.UpdateMorality();
    if (g.pending != 0 || g.bar.init) ok = 0;
    if (!ok) { std::printf("00439f20_TEST FAIL\n"); return 1; }
    std::printf("00439f20_TEST PASS\n");
    return 0;
}