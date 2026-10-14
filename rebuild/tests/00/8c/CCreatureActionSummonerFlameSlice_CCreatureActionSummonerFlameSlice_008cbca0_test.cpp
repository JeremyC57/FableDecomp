#include <cstdio>
struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };
static long g_code = -1, g_ptrs = 0; static void* g_t = 0;
struct Owner { }; struct Thing { };
struct BIP { virtual ~BIP() {} BIP() { ++g_ptrs; } };
struct IP : BIP { IP() : BIP() {} };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, Thing* t, long c, const CSoundPair& s) { g_code = c + s.a; g_t = t; } };
struct Act : Base { IP P; Act(Owner* o, Thing* t) : Base(o, t, 150, CSoundPair()), P() {} };
int main()
{
    Owner o; Thing t; Act a(&o, &t);
    if (g_code != 150 || g_ptrs != 1 || g_t != &t) { std::printf("008cbca0_TEST FAIL\n"); return 1; }
    std::printf("008cbca0_TEST PASS\n");
    return 0;
}