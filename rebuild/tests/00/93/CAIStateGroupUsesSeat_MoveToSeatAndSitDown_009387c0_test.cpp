#include <cstdio>
// Model of CAIStateGroup_UsesSeat::MoveToSeatAndSitDown @ 0x009387C0: MoveToSeat(type) && SitInSeat().
static bool g_move, g_sit; static long g_type; static int g_sitCalls;
struct Group {
    bool MoveToSeat(long t) { g_type = t; return g_move; }
    bool SitInSeat() { ++g_sitCalls; return g_sit; }
    bool MoveToSeatAndSitDown(long t) { return MoveToSeat(t) && SitInSeat(); }
};
int main()
{
    Group g; g_move = true; g_sit = true; g_sitCalls = 0;
    if (!g.MoveToSeatAndSitDown(3) || g_type != 3 || g_sitCalls != 1) { std::printf("009387c0_TEST FAIL\n"); return 1; }
    g_move = false;
    if (g.MoveToSeatAndSitDown(4) || g_sitCalls != 1) { std::printf("009387c0_TEST FAIL\n"); return 1; }
    g_move = true; g_sit = false;
    if (g.MoveToSeatAndSitDown(5) || g_sitCalls != 2) { std::printf("009387c0_TEST FAIL\n"); return 1; }
    std::printf("009387c0_TEST PASS\n"); return 0;
}