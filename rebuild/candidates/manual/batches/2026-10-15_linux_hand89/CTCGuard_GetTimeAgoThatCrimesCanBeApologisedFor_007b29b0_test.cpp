#include <cstdio>

// Model of CTCGuard::GetTimeAgoThatCrimesCanBeApologisedFor @ 0x007B29B0:
// clamp((now - lastApology - fps*15) / fps, 0, 10).
static long g_frame = 0;
static long g_FramesPerSecond = 30;
static long GetCurrentFrame() { return g_frame; }
const long MAX_APOLOGY_SECONDS = 10;
template<class T> inline const T& Clamp(const T& value, const T& low, const T& high)
{
    return (value < low) ? low : ((high < value) ? high : value);
}
struct CTCGuard {
    long FrameLastReceivedApology;
    long GetTimeAgoThatCrimesCanBeApologisedFor() const
    {
        long frames = GetCurrentFrame() - FrameLastReceivedApology;
        long seconds = (frames - g_FramesPerSecond * 15) / g_FramesPerSecond;
        return Clamp(seconds, 0L, MAX_APOLOGY_SECONDS);
    }
};

int main()
{
    CTCGuard g; g.FrameLastReceivedApology = 100;
    g_frame = 100 + 30 * 15 + 30 * 4 + 7;            // 4 seconds past the grace period
    if (g.GetTimeAgoThatCrimesCanBeApologisedFor() != 4) { std::printf("007b29b0_TEST FAIL\n"); return 1; }
    g_frame = 100 + 30 * 3;                          // inside the grace period: negative -> 0
    if (g.GetTimeAgoThatCrimesCanBeApologisedFor() != 0) { std::printf("007b29b0_TEST FAIL\n"); return 1; }
    g_frame = 100 + 30 * 15 + 30 * 100;              // long ago: clamped to 10
    if (g.GetTimeAgoThatCrimesCanBeApologisedFor() != 10) { std::printf("007b29b0_TEST FAIL\n"); return 1; }
    std::printf("007b29b0_TEST PASS\n");
    return 0;
}
