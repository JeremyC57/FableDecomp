#include <cstdio>
struct WS { const wchar_t* p; WS(const wchar_t* t) : p(t) {} };
struct I { bool a; WS n; float s, e, d; long k; I() : a(false), n(L""), s(0.0f), e(0.0f), d(0.0f) {} };
int main()
{
    I i;
    if (i.a || i.n.p[0] != 0 || i.s != 0.0f || i.e != 0.0f || i.d != 0.0f || sizeof(I) != 24) { std::printf("0070bd30_TEST FAIL\n"); return 1; }
    std::printf("0070bd30_TEST PASS\n");
    return 0;
}