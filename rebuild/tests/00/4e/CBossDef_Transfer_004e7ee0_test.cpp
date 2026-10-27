// behaviour test for 0x004E7EE0 CBossDef::Transfer (model: each member reaches the context once, in order)
#include <cstdio>
static const void* g_seen[8]; static int g_n = 0;
struct Ctx { template <class T> void Transfer(T& v) { g_seen[g_n++] = &v; } };
struct Def { int m[5]; void Transfer(Ctx& c) { c.Transfer(m[0]); c.Transfer(m[1]); c.Transfer(m[2]); c.Transfer(m[3]); c.Transfer(m[4]); } };
int main()
{
    Def d; Ctx c; d.Transfer(c);
    int ok = g_n == 5;
    if (g_seen[0] != &d.m[0]) ok = 0;
    if (g_seen[1] != &d.m[1]) ok = 0;
    if (g_seen[2] != &d.m[2]) ok = 0;
    if (g_seen[3] != &d.m[3]) ok = 0;
    if (g_seen[4] != &d.m[4]) ok = 0;
    if (!ok) { std::printf("004e7ee0_TEST FAIL\n"); return 1; }
    std::printf("004e7ee0_TEST PASS\n");
    return 0;
}