// Behaviour test for CMainGameComponent::UpdateFromEventPackageSet (0x0041726D): a model of the row.
#include <stdio.h>

struct Game { long InputServerFrame, CurrentServerFrame; bool CaughtUp; int processed; long reported; };
static void Update(Game& g, const long* stamps, int n)
{
    for (int i = 0; i < n; ++i) {
        if (stamps[i] > g.InputServerFrame) {
            g.processed++;
            if (g.InputServerFrame == g.CurrentServerFrame) g.CaughtUp = true;
            long frame = stamps[i];
            g.InputServerFrame = stamps[i];
            g.CurrentServerFrame = frame > g.CurrentServerFrame ? frame : g.CurrentServerFrame;
            g.reported = g.CurrentServerFrame;
        }
    }
}
static int run()
{
    Game g = { 5, 5, false, 0, 0 };
    long stamps[4] = { 4, 6, 6, 9 };
    Update(g, stamps, 4);
    return g.processed == 2 && g.InputServerFrame == 9 && g.CurrentServerFrame == 9 && g.CaughtUp && g.reported == 9;
}
int main()
{
    int ok = run();
    printf(ok ? "0041726d_TEST PASS\n" : "0041726d_TEST FAIL\n");
    return ok ? 0 : 1;
}
