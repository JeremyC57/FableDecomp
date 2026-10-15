#include <cstdio>
// CAIStateGroup_MinionGossip::InitStateGroup: FrameOfLastGossip = frame - (seed step % (GetTimeToInitiate()*fps))
static unsigned long g_seed = 5; static long g_fps = 10; static long g_frame = 1000;
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Group {
    long FrameOfLastGossip;
    long GetTimeToInitiate() { return 70; }
    unsigned long* GetPWorldSeed() { return &g_seed; }
    void InitStateGroup()
    {
        long fps = g_fps;
        unsigned long* seed = GetPWorldSeed();
        unsigned long range = GetTimeToInitiate() * fps;
        unsigned long next = *seed * 0x24A1U + 0x24DFU; *seed = next;
        unsigned long rotated = ROR13(next); *seed = rotated;
        unsigned long r = range != 0 ? rotated % range : 0;
        FrameOfLastGossip = g_frame - r;
    }
};
int main()
{
    Group g; g.InitStateGroup();
    unsigned long expect = ROR13(5u * 0x24A1U + 0x24DFU);
    if (g_seed != expect || g.FrameOfLastGossip != (long)(1000 - expect % 700)) { std::printf("0090c800_TEST FAIL\n"); return 1; }
    g_fps = 0; g.InitStateGroup();
    if (g.FrameOfLastGossip != 1000) { std::printf("0090c800_TEST FAIL\n"); return 1; }
    std::printf("0090c800_TEST PASS\n"); return 0;
}