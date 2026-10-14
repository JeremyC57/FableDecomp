#include <cstdio>
struct CGroup { long PrimTypeMatchesGroup(long type) { if (type == 0x18 || type == 0x1c || type == 0x1a || type == 0x1b || type == 0x1d || type == 7 || type == 0x14 || type == 0x1e) return 1; return 0; } };
int main()
{
    CGroup g; static const long yes[] = { 0x18, 0x1c, 0x1a, 0x1b, 0x1d, 7, 0x14, 0x1e }; static const long no[] = { 0, 1, 6, 8, 0x13, 0x15, 0x19, 0x1f, -1 };
    for (int i = 0; i < 8; ++i) if (g.PrimTypeMatchesGroup(yes[i]) != 1) { std::printf("00bbbf00_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (g.PrimTypeMatchesGroup(no[i]) != 0) { std::printf("00bbbf00_TEST FAIL\n"); return 1; }
    std::printf("00bbbf00_TEST PASS\n"); return 0;
}