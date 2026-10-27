#include <cstdio>
// Behaviour model: frames run until every collected thing is dead; the flag is set unless the thread was terminated.
static int alive = 3, frames = 0; static bool terminating = false;
static bool AllDead() { return alive == 0; }
static void Frame() { ++frames; --alive; }
static bool Run(bool& flag) { while (!AllDead()) { Frame(); if (terminating) return false; } if (!terminating) flag = true; return true; }
int main()
{
    bool flag = false;
    if (!Run(flag) || !flag || frames != 3) { std::printf("00d1c9e0_TEST FAIL\n"); return 1; }
    alive = 2; frames = 0; terminating = true; flag = false;
    if (Run(flag) || flag || frames != 1) { std::printf("00d1c9e0_TEST FAIL\n"); return 1; }
    std::printf("00d1c9e0_TEST PASS\n");
    return 0;
}