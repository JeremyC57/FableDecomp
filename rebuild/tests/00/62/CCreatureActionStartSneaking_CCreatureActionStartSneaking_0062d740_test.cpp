// behaviour test for 0x0062D740: the action is set up with zero timings, the named animation and flags 0 / 1
#include <cstdio>
#include <cstring>
struct Base { float a, b; const char* anim; long c, d; Base(float a_, float b_, const char* n, long c_, long d_) : a(a_), b(b_), anim(n), c(c_), d(d_) { } };
struct Action : Base { Action() : Base(0.0f, 0.0f, "ST_SNEAK", 0, 1) { } };
int main()
{
    Action x;
    if (x.a != 0.0f || x.b != 0.0f || std::strcmp(x.anim, "ST_SNEAK") || x.c != 0 || x.d != 1) { std::printf("0062d740_TEST FAIL\n"); return 1; }
    std::printf("0062d740_TEST PASS\n");
    return 0;
}