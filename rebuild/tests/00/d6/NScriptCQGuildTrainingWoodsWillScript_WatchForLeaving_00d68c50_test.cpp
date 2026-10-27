#include <cstdio>
// Behaviour model: a dead hero fails an open mission; a succeeded mission is left alone.
struct Q { bool alive, succeeded, failed, terminating; int frames;
    void Watch() { while (alive && !failed && !succeeded) { ++frames; if (frames == 2) alive = false; if (terminating) return; } if (!terminating && !succeeded) failed = true; } };
int main()
{
    Q a = { true, false, false, false, 0 }; a.Watch();
    Q b = { true, true, false, false, 0 }; b.Watch();
    if (!a.failed || a.frames != 2 || b.failed || b.frames != 0) { std::printf("00d68c50_TEST FAIL\n"); return 1; }
    std::printf("00d68c50_TEST PASS\n");
    return 0;
}