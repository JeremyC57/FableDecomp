// Behaviour test for 0x0093A540: Idle runs DoNothing until the random end frame or until the action stops.
#include <cstdio>
static long g_frame = 100;
static long g_fps = 30;
static float g_rand = 2.0f;      // GFFloatRandom stand-in
struct CGroup {
    int calls, stopAt;
    long DoNothing() { ++calls; g_frame += 10; return calls >= stopAt ? 1 : 4; }
    long Idle()
    {
        long framesPerSecond = g_fps;
        float delay = (g_rand + 5.0f) * framesPerSecond;
        unsigned long endFrame = (unsigned long)(g_frame + delay);
        long result;
        do {
            result = DoNothing();
        } while ((unsigned long)g_frame < endFrame && result == 4);
        return result == 4;
    }
};
int main()
{
    CGroup g = { 0, 1000 };
    int bad = 0;
    // end = 100 + 7*30 = 310 -> 21 calls (frame 310 after the 21st)
    if (g.Idle() != 1 || g.calls != 21) bad = 1;
    CGroup h = { 0, 3 }; g_frame = 100;
    if (h.Idle() != 0 || h.calls != 3) bad = 2;
    if (bad) { std::printf("0093a540_TEST FAIL\n"); return 1; }
    std::printf("0093a540_TEST PASS\n");
    return 0;
}