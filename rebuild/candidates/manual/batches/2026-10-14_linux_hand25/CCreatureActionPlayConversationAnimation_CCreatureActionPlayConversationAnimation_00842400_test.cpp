#include <cstdio>
struct CSoundPair { long a, b; };
struct S { long id; };
static S g_groups[4] = { {0}, {1}, {2}, {3} }; static long g_attitude = -1; static bool g_loaded = false; static long g_nameid = -1;
static void* g_o = 0; static float g_a = 0, g_b = 0; static void* g_anim = 0; static const CSoundPair* g_sp = 0; static long g_c = 0, g_d = 0;
static const S& GetAnimGroupName(long a) { g_attitude = a; return g_groups[a]; }
struct Owner { void* GetRandomLoadedAnimFromName(const S& n, bool l) { g_nameid = n.id; g_loaded = l; return (void*)0x55; } };
struct Base { virtual ~Base() {} unsigned char body[0xa4]; Base(Owner* o, float a, float b, void* an, const CSoundPair& s, long c, long d) { g_o = o; g_a = a; g_b = b; g_anim = an; g_sp = &s; g_c = c; g_d = d; } };
struct Act : Base { unsigned char fin; Act(Owner* o, long at, const CSoundPair& s, long c, long d) : Base(o, -1.0f, 1.0f, o->GetRandomLoadedAnimFromName(GetAnimGroupName(at), true), s, c, d), fin(0) {} };
int main()
{
    Owner o; CSoundPair sp = { 5, 6 }; Act x(&o, 2, sp, 7, 8);
    if (g_o != &o || g_attitude != 2 || g_nameid != 2 || !g_loaded || g_a != -1.0f || g_b != 1.0f || g_anim != (void*)0x55 || g_sp != &sp || g_c != 7 || g_d != 8 || x.fin != 0) { std::printf("00842400_TEST FAIL\n"); return 1; }
    std::printf("00842400_TEST PASS\n");
    return 0;
}
