#include <cstdio>
// Behaviour model of CSystemManager::Initialise's control flow: window failure -> 4, drive failure -> its code, else 1; the once-only part is skipped when already initialised.
struct Sys { bool initialised; bool external; bool windowOk; long driveResult; unsigned long flags; int uninit; int managers; bool active; bool ever; bool restore; };
static long Initialise(Sys& s)
{
    if (!s.initialised) {
        s.initialised = true;
        if (s.flags & 4) {
            if (!s.external) {
                if (!s.windowOk) { ++s.uninit; return 4; }
            }
        }
        if (s.driveResult != 1) { ++s.uninit; return s.driveResult; }
        s.managers += 3;
    }
    s.managers += 1;
    if (!s.active && s.ever) s.restore = true;
    s.active = true; s.ever = true;
    return 1;
}
int main()
{
    Sys a = { false, false, false, 1, 4, 0, 0, false, false, false };
    Sys b = { false, true, false, 7, 4, 0, 0, false, false, false };
    Sys c = { false, false, true, 1, 4, 0, 0, false, true, false };
    Sys d = { true, false, false, 1, 4, 0, 0, false, true, false };
    if (Initialise(a) != 4 || a.uninit != 1 || Initialise(b) != 7 || b.uninit != 1 || Initialise(c) != 1 || c.managers != 4 || !c.restore
        || Initialise(d) != 1 || d.managers != 1 || !d.restore)
    { std::printf("009a6610_TEST FAIL\n"); return 1; }
    std::printf("009a6610_TEST PASS\n");
    return 0;
}
