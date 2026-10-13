#include <cstdio>
static int g_c = 0; struct Cnt { long r; }; struct B { virtual void v00() {} B() {} };
struct ST : B { virtual void v00() {} void* t; Cnt* c; ST() : t(0), c(0) {} ST(const ST& o) : B(), t(o.t), c(o.c) { if (c) ++c->r; } };
static void __fastcall CB(void*, ST s) { g_c += (int)s.c->r; }
struct X { virtual void v00() {} bool f; void* proc; ST th; void (__fastcall* pfn)(void*, ST); void* obj; void Run() { if (proc) {} else pfn(obj, th); f = true; } };
int main()
{
    Cnt c = { 1 }; X x; x.proc = 0; x.th.c = &c; x.pfn = CB; x.obj = 0; x.f = false; x.Run(); if (!x.f || g_c != 2) { std::printf("00d82320_TEST FAIL\n"); return 1; }

    std::printf("00d82320_TEST PASS\n");
    return 0;
}
