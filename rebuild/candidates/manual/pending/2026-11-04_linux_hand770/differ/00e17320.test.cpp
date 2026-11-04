// Behaviour test for NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard2::Main 00e17320 (parked): a model of the guard script --
// walk to the post, then until the White Balverine quest is finished chase the hero only while the balverine is within 30 m,
// then remove the guard; termination ends the thread early.
#include <cstdio>
#include <cmath>
struct World { float guardX, heroX, balvX, postX; int frame; bool finished; bool removed; int chases; int stopAt; };
static bool Frame(World& w) { ++w.frame; if (w.frame == 30) w.balvX = 10.0f; if (w.frame == 60) w.finished = true; return w.frame >= w.stopAt; }
static void Main(World& w)
{
    if (Frame(w)) return;
    while (std::fabs(w.guardX - w.postX) >= 5.0f) { w.guardX += (w.postX > w.guardX ? 1.0f : -1.0f); if (Frame(w)) return; }
    while (!w.finished) {
        if (Frame(w)) return;
        if (std::fabs(w.guardX - w.balvX) < 30.0f) {
            float heroX = w.heroX;
            while (std::fabs(w.guardX - heroX) > 2.0f) { if (Frame(w)) return; w.guardX = heroX; ++w.chases; }
        }
    }
    w.removed = true;
}
int main()
{
    World w = { 0.0f, 40.0f, 500.0f, 20.0f, 0, false, false, 0, 1000 };
    Main(w);
    if (!w.removed || w.chases != 1 || std::fabs(w.guardX - 40.0f) > 1e-4f) { std::printf("00e17320_TEST FAIL\n"); return 1; }
    World t = { 0.0f, 40.0f, 500.0f, 20.0f, 0, false, false, 0, 5 };
    Main(t);
    if (t.removed) { std::printf("00e17320_TEST FAIL\n"); return 1; }
    std::printf("00e17320_TEST PASS\n");
    return 0;
}
