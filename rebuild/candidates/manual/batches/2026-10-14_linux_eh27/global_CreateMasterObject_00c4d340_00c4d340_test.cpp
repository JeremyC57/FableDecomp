#include <cstdio>
#include <new>
#include <cstdlib>
#include <cstring>
static int g_ctor = 0, g_dtor = 0, g_not = 0, g_ref = 0; static long g_init = 0; static bool g_fail = false; static void* g_a = 0;
struct Nt { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void Notify(long f) { ++g_not; } }; struct Pt { virtual void v0() {} virtual void Refresh() { ++g_ref; } unsigned char p[0x18]; long flag; };
struct Rec { unsigned char b[0xa0]; }; static Rec g_rec; static const Rec k_def = { { 7, 7, 7 } };
struct M { virtual ~M() { ++g_dtor; } unsigned char p[0x54]; Pt* part; unsigned char q[0x180 - 0x5c]; long m180; unsigned char r[0x1d4 - 0x184]; Nt* nt; M() { ++g_ctor; part = 0; m180 = 0; nt = 0; } long Init(void* a, void* b) { g_a = a; return g_init; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } }; static M* g_pm = 0;
static long Create(void* a, void* b, M** out) { M* m = new M; if (!m) return -5; g_pm = m; long r = m->Init(a, b); if (r == 0) { *out = m; if (m->nt) m->nt->Notify(1); if (m->m180) { Pt* p = m->part; p->flag = 1; p->Refresh(); } } else { g_rec = k_def; delete m; } return r; }
static bool Check() { M* out = 0; int a; Nt nt; Pt pt; pt.flag = 0; g_fail = true; if (Create(&a, 0, &out) != -5) return false; g_fail = false; g_init = 3; std::memset(&g_rec, 0, sizeof g_rec); if (Create(&a, 0, &out) != 3 || g_dtor != 1 || g_rec.b[0] != 7 || out != 0) return false; g_init = 0; if (Create(&a, 0, &out) != 0 || out == 0 || g_pm != out || g_a != &a) return false; out->nt = &nt; out->part = &pt; out->m180 = 1; M* o2 = 0; if (Create(&a, 0, &o2) != 0) return false; delete out; delete o2; return g_ctor == 3 && g_dtor == 3; }

int main()
{
    if (!Check()) { std::printf("00c4d340_TEST FAIL\n"); return 1; }
    std::printf("00c4d340_TEST PASS\n");
    return 0;
}
