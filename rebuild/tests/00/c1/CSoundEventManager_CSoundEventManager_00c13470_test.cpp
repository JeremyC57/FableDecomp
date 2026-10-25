// Behaviour test for 0x00C13470 CSoundEventManager::~CSoundEventManager: Shutdown under the lock, then the owned parts
// deleted and cleared, the members destroyed.
#include <cstdio>
#include <new>
#include <cstdlib>
static int g_lk = 0, g_sh = 0, g_da = 0, g_db = 0, g_dc = 0, g_dd = 0, g_m2c = 0, g_m7d8 = 0; static unsigned char g_sec[8];
struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } }; struct Pa { unsigned char b[4]; ~Pa() { ++g_da; } }; struct Pb { unsigned char b[4]; ~Pb() { ++g_db; } }; struct Ic { unsigned char b[4]; ~Ic() { ++g_dc; } }; struct Id { unsigned char b[4]; ~Id() { ++g_dd; } }; struct Pc { unsigned char p[0xc]; Ic c; }; struct Pd { unsigned char p[0xc]; Id c; }; struct M2c { unsigned char b[0x788]; ~M2c() { ++g_m2c; } }; struct M7d8 { unsigned char b[4]; ~M7d8() { ++g_m7d8; } };
struct X { virtual ~X() { { Pl l(g_sec); Sh(); } if (pa) { delete pa; pa = 0; } if (pb) { delete pb; pb = 0; } if (pc) { delete pc; pc = 0; } if (pd) { delete pd; pd = 0; } } unsigned char p[0x28]; M2c m2c; Pd* pd; Pc* pc; Pb* pb; long m7c0; Pa* pa; unsigned char q[0x10]; M7d8 m7d8; void Sh() { ++g_sh; } };
static bool Check() { X* x = new X; x->pa = new Pa; x->pb = 0; x->pc = new Pc; x->pd = new Pd; delete x; return g_sh == 1 && g_lk == 0 && g_da == 1 && g_db == 0 && g_dc == 1 && g_dd == 1 && g_m2c == 1 && g_m7d8 == 1; }

int main()
{
    if (!Check()) { std::printf("00c13470_TEST FAIL\n"); return 1; }
    std::printf("00c13470_TEST PASS\n");
    return 0;
}