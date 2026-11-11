// Behaviour test for 0x006CB8F0 CTCCreatureNavigation::IsCurrentlyNavigating() const: true when FrameLastNavigated (+0x50) is
// non-negative and the world frame is at most one past it; the GetWorld() call is made only on the non-negative path.
#include <cstdio>
#include <cstddef>
class CWorld { public: static long GetFrame(); };
static long g_frame = 0;
static int g_getWorldCalls = 0;
long CWorld::GetFrame() { return g_frame; }
static CWorld g_world;
class CTCCreatureNavigation {
public:
    char pad[0x50];
    int  FrameLastNavigated;                                // +0x50
    CWorld& GetWorld() const { ++g_getWorldCalls; return g_world; }
    bool IsCurrentlyNavigating() const { return FrameLastNavigated >= 0 && GetWorld().GetFrame() - FrameLastNavigated <= 1; }
};
int main()
{
    CTCCreatureNavigation o;
    if (offsetof(CTCCreatureNavigation, FrameLastNavigated) != 0x50) { std::printf("FAIL layout\n"); return 1; }
    o.FrameLastNavigated = -5;
    if (o.IsCurrentlyNavigating() || g_getWorldCalls != 0) { std::printf("FAIL neg\n"); return 1; }
    o.FrameLastNavigated = 10; g_frame = 10;
    if (!o.IsCurrentlyNavigating()) { std::printf("FAIL eq\n"); return 1; }
    g_frame = 11;
    if (!o.IsCurrentlyNavigating()) { std::printf("FAIL one\n"); return 1; }
    g_frame = 12;
    if (o.IsCurrentlyNavigating()) { std::printf("FAIL two\n"); return 1; }
    if (g_getWorldCalls != 3) { std::printf("FAIL calls\n"); return 1; }
    std::printf("CTCCreatureNavigation_006cb8f0_TEST PASS\n");
    return 0;
}
