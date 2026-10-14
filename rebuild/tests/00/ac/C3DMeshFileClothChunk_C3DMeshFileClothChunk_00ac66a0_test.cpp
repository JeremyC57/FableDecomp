#include <cstdio>

struct Arr { void* b; void* e; void* c; Arr() : b(0), e(0), c(0) {} };
struct Cloth { const char* type; const char* Name; long MaterialIndex; const char* ParticleCode; Arr CompiledCode;
    Cloth() : type("CLTH"), Name(""), ParticleCode("") {} };
int main()
{
    Cloth c;
    if (c.type[0] != 'C' || c.Name[0] || c.ParticleCode[0] || c.CompiledCode.b || c.CompiledCode.c) { std::printf("00ac66a0_TEST FAIL\n"); return 1; }
    std::printf("00ac66a0_TEST PASS\n");
    return 0;
}