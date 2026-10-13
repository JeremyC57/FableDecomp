#include <cstdio>
struct G { unsigned char p[0x24]; void* f; }; static G g_g; static G* g_p = &g_g; struct X { void* Get() { return g_p->f; } };
int main()
{
    int v; g_g.f = &v; X x; if (x.Get() != &v) { std::printf("00640010_TEST FAIL\n"); return 1; }

    std::printf("00640010_TEST PASS\n");
    return 0;
}