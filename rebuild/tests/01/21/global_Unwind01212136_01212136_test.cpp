#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tb { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual long Get(unsigned long i) { return (long)i * 10; } virtual unsigned long Count() { return 3; } }; static Tb g_tb; struct En { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual Tb* GetTable() { return &g_tb; } };
static long Q(En* e, unsigned long i) { L l(&g_g); if (i >= e->GetTable()->Count()) return -1; return e->GetTable()->Get(i); }
static bool Check() { En e; return Q(&e, 3) == -1 && Q(&e, 2) == 20 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c198c0_TEST FAIL\n"); return 1; }
    std::printf("01212136_TEST PASS\n");
    return 0;
}