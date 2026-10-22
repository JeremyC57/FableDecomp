#include <cstdio>
// Behaviour model of CHistoryBookcase::Main (0x00EA7B40): bookcase flow over a simulated frame clock.
struct World { int frame; int usedFrame; int terminateFrame; bool bookRead; int usable; int markers; int objectives; int entries; int clickEvery; };
static bool Terminating(World& w) { return w.frame >= w.terminateFrame; }
static void NewFrame(World& w) { ++w.frame; }
static bool Clicked(World& w) { return w.frame % w.clickEvery == 0; }
static void Main(World& w)
{
    if (!w.bookRead) {
        if (Terminating(w)) return;
        ++w.usable; ++w.markers;
        while (w.frame < w.usedFrame) { NewFrame(w); if (Terminating(w)) return; }
        if (Terminating(w)) return;
        ++w.objectives; --w.markers; ++w.markers;
        w.bookRead = true;
        for (int i = 0; i < 4; ++i) {
            ++w.entries;
            while (!Clicked(w)) { NewFrame(w); if (Terminating(w)) return; }
            if (Terminating(w)) return;
            NewFrame(w);
        }
    }
    do { NewFrame(w); } while (!Terminating(w));
}
int main()
{
    World a = { 0, 3, 100, false, 0, 0, 0, 0, 5 }; Main(a);
    int ok = a.bookRead && a.entries == 4 && a.objectives == 1 && a.markers == 1 && a.frame == 100;
    World b = { 0, 3, 50, true, 0, 0, 0, 0, 5 }; Main(b);
    ok = ok && b.entries == 0 && b.usable == 0 && b.frame == 50;
    World c = { 0, 30, 10, false, 0, 0, 0, 0, 5 }; Main(c);
    ok = ok && !c.bookRead && c.usable == 1 && c.frame == 10;
    if (!ok) { std::printf("00ea7b40_TEST FAIL\n"); return 1; }
    std::printf("00ea7b40_TEST PASS\n");
    return 0;
}