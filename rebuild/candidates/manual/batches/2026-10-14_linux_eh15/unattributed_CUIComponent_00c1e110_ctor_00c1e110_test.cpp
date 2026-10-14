#include <cstdio>
struct X; struct Reg { virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){} virtual void v4(){} virtual void v5(){} virtual void v6(){} virtual void v7(){} virtual void v8(){} virtual void Add(X* c) { added = c; } X* added; Reg() : added(0) {} };
struct Own { unsigned char p[0x38]; Reg r; }; static Own* g_owner = 0; static void* g_ctx = 0;
struct Bs { virtual void v0() {} unsigned char b[0x1c]; Bs(Own* o, void* c) { g_owner = o; g_ctx = c; } };
struct X : Bs { void* d; void* c; void* b; X(Own* o, void* B, void* C, void* D, void* E) : Bs(o, E), d(D), c(C), b(B) { o->r.Add(this); } };
static bool Check() { Own o; int a1, a2, a3, a4; X x(&o, &a1, &a2, &a3, &a4); return g_owner == &o && g_ctx == &a4 && x.b == &a1 && x.c == &a2 && x.d == &a3 && o.r.added == &x; }

int main()
{
    if (!Check()) { std::printf("00c1e110_TEST FAIL\n"); return 1; }
    std::printf("00c1e110_TEST PASS\n");
    return 0;
}
