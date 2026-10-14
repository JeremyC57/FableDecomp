#include <cstdio>
static long g_framesPerSecond = 30; static unsigned long g_minSecondsBetweenBuys = 10; static long g_frame;
static long GFGetFrameCount() { return g_frame; }
static bool g_open, g_hasSeller; static int g_acquires;
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; };
struct CAIStateGroup_WorkBase : CAIStateGroupBase { bool IsInWorkHours() { return g_open; } bool HasSeller() { return g_hasSeller; } void AcquireSeller() { ++g_acquires; g_hasSeller = true; } };
struct CAIStateGroup_BuyFromSeller : CAIStateGroup_WorkBase {
    long FrameLastBought;
    virtual bool IsRunnable()
    {
        if ((GFGetFrameCount() - FrameLastBought) / g_framesPerSecond > g_minSecondsBetweenBuys && IsInWorkHours()) { if (!HasSeller()) AcquireSeller(); return HasSeller() ? true : false; }
        return false;
    }
};
int main()
{
    CAIStateGroup_BuyFromSeller g; g.FrameLastBought = 0;
    g_frame = 300; g_open = true; if (g.IsRunnable()) { std::printf("0091a390_TEST FAIL\n"); return 1; }      // exactly 10 s: not > 10
    g_frame = 331; g_open = false; if (g.IsRunnable()) { std::printf("0091a390_TEST FAIL\n"); return 1; }
    g_open = true; g_hasSeller = false; if (!g.IsRunnable() || g_acquires != 1) { std::printf("0091a390_TEST FAIL\n"); return 1; }
    if (!g.IsRunnable() || g_acquires != 1) { std::printf("0091a390_TEST FAIL\n"); return 1; }
    std::printf("0091a390_TEST PASS\n"); return 0;
}