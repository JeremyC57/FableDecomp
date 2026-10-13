#include <cstdio>

static int g_chk = 0;
struct H { unsigned char pad[0x70]; long Level; long Max; long Points;
    void Check() { ++g_chk; }
    void Force(long l) { Level = l; if (Level < 0) Level = 0; else if (Level > Max) Level = Max; Points = 0; Check(); } };
int main()
{
    H h; h.Max = 5; h.Points = 9; h.Force(-3); if (h.Level != 0 || h.Points != 0) { std::printf("0057cb00_TEST FAIL\n"); return 1; }
    h.Force(9); if (h.Level != 5) { std::printf("0057cb00_TEST FAIL\n"); return 1; }
    h.Force(3); if (h.Level != 3 || g_chk != 3) { std::printf("0057cb00_TEST FAIL\n"); return 1; }
    std::printf("0057cb00_TEST PASS\n");
    return 0;
}