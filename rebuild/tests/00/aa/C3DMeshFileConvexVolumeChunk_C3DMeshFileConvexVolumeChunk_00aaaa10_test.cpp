#include <cstdio>

struct Arr { void* b; void* e; void* c; Arr() : b(0), e(0), c(0) {} };
struct CV { const char* type; const char* Name; Arr Planes; CV() : type("HCVL"), Name("") {} };
int main()
{
    CV c; if (c.type[1] != 'C' || c.Name[0] || c.Planes.e) { std::printf("00aaaa10_TEST FAIL\n"); return 1; }
    std::printf("00aaaa10_TEST PASS\n");
    return 0;
}