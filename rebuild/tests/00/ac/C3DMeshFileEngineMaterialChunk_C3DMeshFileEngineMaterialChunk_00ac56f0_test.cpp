#include <cstdio>

struct EM { const char* type; long MaterialIndex; const char* Name; void* MapFilenames; unsigned long d, b, r, i, flags, self; bool t1, t2, t3, t4;
    EM() : type("EMTL"), MaterialIndex(0), Name(""), MapFilenames(0), d(0), b(0), r(0), i(0), flags(0), self(0), t1(false), t2(false), t3(false), t4(false) {} };
int main()
{
    EM e; if (e.type[0] != 'E' || e.MaterialIndex || e.MapFilenames || e.self || e.t4) { std::printf("00ac56f0_TEST FAIL\n"); return 1; }
    std::printf("00ac56f0_TEST PASS\n");
    return 0;
}