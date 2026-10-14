#include <cstdio>
static int g_ctor = 0, g_dtor = 0, g_which = -1, g_flag = -1;
struct Str { char* p; Str() : p(0) { ++g_ctor; } ~Str() { ++g_dtor; } };
struct Vec;
struct M { virtual void v00() {}
    void Get(Vec& v, bool f) { Unpack(v, false, Str(), f); }
    void Unpack(Vec&, bool d, const Str& s, bool f) { g_which = d; g_flag = f; if (s.p) g_which = 99; } };
int main()
{
    M m; m.Get(*(Vec*)0, true);
    if (g_ctor != 1 || g_dtor != 1 || g_which != 0 || g_flag != 1) { std::printf("00a2f120_TEST FAIL\n"); return 1; }
    std::printf("00a2f120_TEST PASS\n");
    return 0;
}
