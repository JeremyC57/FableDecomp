#include <cstdio>
struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };
static long g_g = -1, g_p = -1, g_ptrs = 0;
struct Owner { };
struct BIP { virtual ~BIP() {} BIP() { ++g_ptrs; } };
struct IP : BIP { IP() : BIP() {} };
struct Base { virtual ~Base() {} unsigned char body[0xa4]; Base(Owner* o, const CSoundPair& s, long g, long p) { g_g = g; g_p = p; } };
struct Act : Base { IP PTarget; IP PSpirit; Act(Owner* o) : Base(o, CSoundPair(), 0, 0), PTarget(), PSpirit() {} };
int main()
{
    Owner o; Act a(&o);
    if (g_g != 0 || g_p != 0 || g_ptrs != 2) { std::printf("008546c0_TEST FAIL\n"); return 1; }
    std::printf("008546c0_TEST PASS\n");
    return 0;
}
