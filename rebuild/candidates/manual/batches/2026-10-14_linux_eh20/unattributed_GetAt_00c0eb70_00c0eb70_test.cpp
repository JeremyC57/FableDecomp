#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_f = 0; struct It { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual unsigned long Count() { return 3; } };
struct X { unsigned char p[0x50]; It it; void* Fetch(unsigned long i) { ++g_f; return (void*)&g_f; } void* GetAt(unsigned long i) { L l(&g_g); if (i >= it.Count()) return 0; return Fetch(i); } };
static bool Check() { X* x = new X; bool ok = x->GetAt(3) == 0 && x->GetAt(2) == (void*)&g_f && g_f == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0eb70_TEST FAIL\n"); return 1; }
    std::printf("00c0eb70_TEST PASS\n");
    return 0;
}
