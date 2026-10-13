#include <cstdio>
static int g_g = 0; struct G { void* a; void* b; G(void* o) : a(o), b(0) { ++g_g; } ~G() { --g_g; } };
struct Q { virtual void v00() {} virtual void s1() {} virtual long Query() { return 7; } }; struct X { unsigned char pad[4]; Q* q; long Get() { G g(this); return q->Query(); } };
int main()
{
    Q q; X x; x.q = &q; if (x.Get() != 7 || g_g != 0) { std::printf("00c49c30_TEST FAIL\n"); return 1; }

    std::printf("00c49c30_TEST PASS\n");
    return 0;
}