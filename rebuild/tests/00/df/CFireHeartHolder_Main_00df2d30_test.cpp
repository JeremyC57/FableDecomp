// Behaviour test for 00df2d30 CFireHeartHolder::Main: frame loop until the heart is taken, then idle until done.
#include <cstdio>
struct World { bool taken; int frames; int usedAtFrame; int doneAtFrame; int movies; int pauses; int unpauses; int taken_objects; };
static bool IsDone(World& w) { return w.frames >= w.doneAtFrame; }
static void Main(World& w)
{
    while (!w.taken) {
        ++w.frames;
        if (IsDone(w)) return;
        bool used = w.frames == w.usedAtFrame;
        if (used) {
            if (IsDone(w)) return;
            ++w.movies; ++w.pauses; w.taken = true; ++w.taken_objects; ++w.unpauses;
        }
    }
    while (!IsDone(w)) ++w.frames;
}
int main()
{
    World a = { false, 0, 3, 10, 0, 0, 0, 0 }; Main(a);
    World b = { false, 0, 30, 5, 0, 0, 0, 0 }; Main(b);
    if (!a.taken || a.movies != 1 || a.frames != 10 || a.unpauses != 1 || b.taken || b.movies || b.frames != 5) { std::printf("00df2d30_TEST FAIL\n"); return 1; }
    std::printf("00df2d30_TEST PASS\n");
    return 0;
}