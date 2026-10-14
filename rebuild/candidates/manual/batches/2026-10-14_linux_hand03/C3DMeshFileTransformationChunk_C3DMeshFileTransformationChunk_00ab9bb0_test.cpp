#include <cstdio>

struct M { float m[12]; };
struct T { const char* type; M Transform; T(const M& t) : type("TRFM"), Transform(t) {} };
int main()
{
    M m; for (int i = 0; i < 12; ++i) m.m[i] = (float)i;
    T t(m); if (t.type[0] != 'T' || t.Transform.m[11] != 11.0f || t.Transform.m[0] != 0.0f) { std::printf("00ab9bb0_TEST FAIL\n"); return 1; }
    std::printf("00ab9bb0_TEST PASS\n");
    return 0;
}
