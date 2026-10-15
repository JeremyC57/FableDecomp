#include <cstdio>
// MinionSitOnSeat::Initialise: base init, then FrameToSitDown = frame + seedstep % (fps*80)
static unsigned long g_seed = 11; static long g_fps = 10; static long g_frame = 500;
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Group {
    int brain; long FrameToSitDown;
    void BaseInitialise(int b) { brain = b; }
    void Initialise(int b)
    {
        BaseInitialise(b);
        long fps = g_fps; unsigned long* seed = &g_seed; unsigned long range = fps * 80;
        unsigned long next = *seed * 0x24A1U + 0x24DFU; *seed = next;
        unsigned long rotated = ROR13(next); *seed = rotated;
        unsigned long r = range != 0 ? rotated % range : 0;
        FrameToSitDown = g_frame + r;
    }
};
int main()
{
    Group g; g.brain = 0; g.FrameToSitDown = 0; g.Initialise(7);
    unsigned long expect = ROR13(11u * 0x24A1U + 0x24DFU);
    if (g.brain != 7 || g_seed != expect || g.FrameToSitDown != (long)(500 + expect % 800)) { std::printf("0090f690_TEST FAIL\n"); return 1; }
    g_fps = 0; g.Initialise(8); if (g.FrameToSitDown != 500 || g.brain != 8) { std::printf("0090f690_TEST FAIL\n"); return 1; }
    std::printf("0090f690_TEST PASS\n"); return 0;
}