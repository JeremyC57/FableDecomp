#include <cstdio>
static const char* g_t = 0; static long g_l = 0;
struct CCharString { void* p; CCharString(const char* t, long l) { g_t = t; g_l = l; p = 0; } };
struct Tag { CCharString Name; Tag() : Name("CTCDParticleEmitter", -1) {} };
int main()
{
    Tag tag; if (!g_t || g_l != -1 || g_t[0] != 'C') { std::printf("004d3dfc_TEST FAIL\n"); return 1; }

    std::printf("004d3dfc_TEST PASS\n");
    return 0;
}