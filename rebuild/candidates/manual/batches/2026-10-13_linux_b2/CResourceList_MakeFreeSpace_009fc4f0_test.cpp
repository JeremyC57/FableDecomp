#include <cstdio>

static int g_ev = 0;
struct L { unsigned char pad[0x24]; unsigned long Allocated; unsigned long Maximum;
    void EvictForSpace(unsigned long) { ++g_ev; }
    bool MakeFreeSpace(unsigned long b) { EvictForSpace(b); return Allocated + b <= Maximum; } };
int main()
{
    L l; l.Allocated = 90; l.Maximum = 100; if (!l.MakeFreeSpace(10) || l.MakeFreeSpace(11) || g_ev != 2) { std::printf("009fc4f0_TEST FAIL\n"); return 1; }
    std::printf("009fc4f0_TEST PASS\n");
    return 0;
}
