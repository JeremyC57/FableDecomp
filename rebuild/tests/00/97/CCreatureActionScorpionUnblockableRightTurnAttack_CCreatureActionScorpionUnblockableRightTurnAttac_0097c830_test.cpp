#include <cstdio>
static long g_k = 0, g_s = 0; struct Str { const char* s; Str(const char* x, long = -1) : s(x) {} ~Str() { ++g_s; } }; struct SP { long a, b; SP() : a(0), b(0) {} };
static SP __fastcall GSP(void*, const Str& n) { SP p; p.a = n.s[0]; p.b = 1; return p; }
struct Base { virtual void v00() {} Base(void*, void*, long k, const SP& p) { g_k = k + p.a + p.b; } };
struct D : Base { virtual void v00() {} D(void* o, void* t) : Base(o, t, 0x96, GSP(o, "A")) {} };
int main()
{
    D d(0, 0); if (g_k != 0x96 + 'A' + 1 || g_s != 1) { std::printf("0097c830_TEST FAIL\n"); return 1; }

    std::printf("0097c830_TEST PASS\n");
    return 0;
}