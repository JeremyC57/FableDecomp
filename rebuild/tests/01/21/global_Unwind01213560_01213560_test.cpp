#include <cstdio>
#include <new>
#include <cstdlib>
struct X; static int g_rec = 0, g_prop = 0; static long g_key = -1; static void* g_ra = 0; static long g_rb = -1; struct Rc { unsigned char b[0x50]; Rc(X* o, long k) { ++g_rec; g_key = k; } static void* operator new(size_t n) throw() { return std::malloc(n); } }; static Rc* g_last = 0; struct Pr { unsigned char b[0x168]; Pr(Rc* r, void* a, long b, void* d, void* e, void* f, void* g) { ++g_prop; g_last = r; g_ra = a; g_rb = b; } static void* operator new(size_t n) throw() { return std::malloc(n); } };
static bool g_known68 = false; static Rc* g_known8 = 0; struct Mp { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual Rc* Find(const long& k) { return 0; } unsigned char p[0x5c]; }; struct Mp68 : Mp { virtual Rc* Find(const long& k) { return g_known68 ? (Rc*)1 : 0; } }; struct Mp8 : Mp { virtual Rc* Find(const long& k) { return g_known8; } };
struct X { long m0; bool dirty; Mp8 recs; Mp68 props; long Add(void* a, long b, long c, void* d, void* e, void* f, void* g) { if (props.Find(b)) return -1; Rc* r = recs.Find(c); if (!r) r = new Rc(this, c); new Pr(r, a, b, d, e, f, g); dirty = true; return 0; } };
static bool Check() { X x; x.dirty = false; int a; g_known68 = true; if (x.Add(&a, 1, 2, 0, 0, 0, 0) != -1 || x.dirty) return false; g_known68 = false; if (x.Add(&a, 1, 2, 0, 0, 0, 0) != 0 || g_rec != 1 || g_key != 2 || g_prop != 1 || g_ra != &a || g_rb != 1 || !x.dirty) return false; Rc r((X*)0, 0); g_rec = 0; g_known8 = &r; return x.Add(&a, 3, 4, 0, 0, 0, 0) == 0 && g_rec == 0 && g_last == &r && g_prop == 2; }

int main()
{
    if (!Check()) { std::printf("00c2a490_TEST FAIL\n"); return 1; }
    std::printf("01213560_TEST PASS\n");
    return 0;
}