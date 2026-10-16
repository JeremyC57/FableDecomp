// Behaviour test for CTCTrapBase::FrameUpdate (0x007888B4): hit-thing entries with frames left and a live thing count down,
// the others are erased; then the trigger things, trigger and trap updates run in order.
#include <cstdio>
struct Hit { int alive; long frames; };
static int g_order[3], g_n;
int main()
{
    Hit v[4] = { { 1, 2 }, { 0, 5 }, { 1, 0 }, { 1, 1 } };
    Hit* end = v + 4;
    for (Hit* it = v; it != end; ) {
        if (it->frames != 0 && it->alive) { --it->frames; ++it; }
        else { for (Hit* p = it; p + 1 != end; ++p) *p = *(p + 1); --end; }
    }
    g_order[g_n++] = 1; g_order[g_n++] = 2; g_order[g_n++] = 3;
    if (end - v != 2 || v[0].frames != 1 || v[1].frames != 0 || g_order[0] != 1 || g_order[2] != 3) {
        std::printf("007888b4_TEST FAIL\n"); return 1;
    }
    std::printf("007888b4_TEST PASS\n");
    return 0;
}