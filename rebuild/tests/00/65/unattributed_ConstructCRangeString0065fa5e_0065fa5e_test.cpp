#include <cstdio>
static int g_n = 0; struct Tg { Tg() {} }; struct It { const unsigned short* p; It(const It& o) : p(o.p) {} It(const unsigned short* x) : p(x) {} };
struct Str { void* d; void __fastcall RI(const Tg&, It f, It l, void*) { g_n = (int)(l.p - f.p); } Str* C(It f, It l, void* a) { RI(Tg(), f, l, a); return this; } };
int main()
{
    static const unsigned short w[] = { 97, 98, 99, 0 }; Str s; It f(w), l(w + 3); if (s.C(f, l, 0) != &s || g_n != 3) { std::printf("0065fa5e_TEST FAIL\n"); return 1; }

    std::printf("0065fa5e_TEST PASS\n");
    return 0;
}