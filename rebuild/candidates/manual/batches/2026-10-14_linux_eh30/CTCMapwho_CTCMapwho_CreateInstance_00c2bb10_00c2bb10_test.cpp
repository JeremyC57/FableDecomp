#include <cstdio>
#include <new>
#include <cstdlib>
static int g_destroy = 0, g_unlink = 0, g_ctor = 0; static void* g_fo = 0; static void* g_fq = 0; static void* g_k = 0; static long g_fa = 0, g_fb = 0;
struct V3 { float x, y, z; }; struct Pos { float x, y, z; bool f; Pos() : x(0), y(0), z(0), f(true) {} Pos& operator=(const V3& v) { f = false; x = v.x; y = v.y; z = v.z; return *this; } }; static Pos g_pos;
struct Fk { void* o; void* q; Fk(void* a, void* b) : o(a), q(b) {} ~Fk() {} };
struct M; struct En { unsigned char b[0x60]; En(M* m, void* a, void* k, void* c, void* d, const Pos* p, long fa, long fb) { ++g_ctor; g_pos = *p; g_k = k; g_fa = fa; g_fb = fb; } void Unlink() { ++g_unlink; } }; static En* g_existing = 0;
struct Rg { virtual void s0() {} virtual void s1() {} virtual void s2() {} virtual void s3() {} virtual void s4() {} virtual void s5() {} virtual void s6() {} virtual void s7() {} virtual void s8() {} virtual void s9() {} virtual void s10() {} virtual void s11() {} virtual void s12() {} virtual void s13() {} virtual void s14() {} virtual void s15() {} virtual void s16() {} virtual En* Find(const Fk* k) { g_fo = k->o; g_fq = k->q; return g_existing; } };
struct M { unsigned char pad[0x34]; Rg r; void Destroy(En* e) { if (e) { e->Unlink(); std::free(e); } ++g_destroy; } void* Create(void* a, void* k, void* c, void* d, const V3* p, long fa, long fb) { { Fk f(a, c); En* e = r.Find(&f); if (e) Destroy(e); } Pos pos; pos = *p; En* n = new En(this, a, k, c, d, &pos, fa, fb); delete n; return k; } };
static bool Check() { M* m = new M; int a, k, c, d; V3 v = { 1, 2, 3 }; if (m->Create(&a, &k, &c, &d, &v, 1, 0) != &k || g_ctor != 1 || g_destroy || g_fo != &a || g_fq != &c || g_pos.x != 1 || g_pos.z != 3 || g_pos.f || g_fa != 1 || g_k != &k) return false; g_existing = (En*)std::malloc(0x60); m->Create(&a, &k, &c, &d, &v, 0, 1); delete m; return g_destroy == 1 && g_unlink == 1 && g_ctor == 2 && g_fb == 1; }

int main()
{
    if (!Check()) { std::printf("00c2bb10_TEST FAIL\n"); return 1; }
    std::printf("00c2bb10_TEST PASS\n");
    return 0;
}
