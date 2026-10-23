#include <cstdio>
#include <new>
#include <cstdlib>
struct X; static int g_ctor = 0, g_dtor = 0, g_rel = 0; static long g_init = 0; static X* g_owner = 0; static bool g_fail = false;
struct Rn { virtual ~Rn() { ++g_dtor; } virtual long Init(X* o) { g_owner = o; return g_init; } virtual void Release() { ++g_rel; } unsigned char b[0x820]; Rn() { ++g_ctor; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
static Rn* g_made = 0; struct Fa { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual Rn* Create() { return g_made; } };
struct X { unsigned char p[0x17c]; Rn* rn; unsigned char q[0x54]; Fa* fa; long Init(Rn** out) { if (fa) rn = fa->Create(); else rn = new Rn; if (!rn) return -5; long r = rn->Init(this); if (r) { rn->Release(); delete rn; rn = 0; return r; } *out = rn; return 0; } };
static bool Check() { X* x = new X; x->fa = 0; x->rn = 0; Rn* out = 0; g_fail = true; if (x->Init(&out) != -5 || x->rn != 0) return false; g_fail = false; g_init = 9; if (x->Init(&out) != 9 || g_ctor != 1 || g_owner != x || g_rel != 1 || g_dtor != 1 || x->rn != 0) return false; g_init = 0; if (x->Init(&out) != 0 || out == 0 || x->rn != out || g_ctor != 2) return false; delete out; Fa fa; Rn* made = new Rn; g_made = made; x->fa = &fa; bool ok = x->Init(&out) == 0 && out == made && x->rn == made && g_ctor == 3; delete made; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c3e640_TEST FAIL\n"); return 1; }
    std::printf("01215100_TEST PASS\n");
    return 0;
}