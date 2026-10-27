// behaviour test for 0x0046F203: Transfer visits every member once, in declaration order
#include <cstdio>
static int g_order[16]; static int g_n = 0;
struct Ctx { void Transfer(int& v) { g_order[g_n++] = v; } };
struct D { int m[6]; void Transfer(Ctx& c) { for (int i = 0; i < 6; ++i) c.Transfer(m[i]); } };
int main()
{
    D d; for (int i = 0; i < 6; ++i) d.m[i] = 100 + i;
    Ctx c; d.Transfer(c);
    int ok = g_n == 6;
    for (int i = 0; i < 6; ++i) ok = ok && g_order[i] == 100 + i;
    if (!ok) { std::printf("0046f203_TEST FAIL\n"); return 1; }
    std::printf("0046f203_TEST PASS\n");
    return 0;
}