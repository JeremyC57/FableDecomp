#include <cstdio>
struct S { const char* p; long n; S(const char* t, long l) : p(t), n(l) {} };
struct M { S Get() { return S("ABC", -1); } };
int main()
{
    M m; S s = m.Get(); if (s.n != -1 || s.p[0] != 'A') { std::printf("005d877d_TEST FAIL\n"); return 1; }

    std::printf("005d877d_TEST PASS\n");
    return 0;
}
