// behaviour test for 0x005DB2E0: the two members are written in declaration order
#include <cstdio>
static double g_out[4]; static int g_n = 0;
struct Out { void Write(double v) { g_out[g_n++] = v; } };
struct C { double a, b; void TransferBinaryOut(Out& o) const { o.Write(a); o.Write(b); } };
int main()
{
    C c = { 3, 5 }; Out o; c.TransferBinaryOut(o);
    if (g_n != 2 || g_out[0] != 3 || g_out[1] != 5) { std::printf("005db2e0_TEST FAIL\n"); return 1; }
    std::printf("005db2e0_TEST PASS\n");
    return 0;
}