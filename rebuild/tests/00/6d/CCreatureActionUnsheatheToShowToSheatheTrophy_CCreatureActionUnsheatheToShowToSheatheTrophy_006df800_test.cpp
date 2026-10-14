#include <cstdio>
struct Owner { }; struct Thing { }; struct ActB { };
struct Str { const char* p; Str(const char* t) : p(t) {} Str(const Str& o) : p(o.p) {} };
static long g_pri = 0; static void* g_f = &g_pri;
struct BIP { virtual ~BIP() {} void* m_p; BIP() : m_p(0) {} void SetPItem(void* p) { m_p = p; } };
struct IP : BIP { IP() : BIP() {} };
struct Base { virtual ~Base() {} unsigned char body[0xb4]; Base(Owner* o, Thing* t, ActB* f, long pri) { g_pri = pri; g_f = f; } };
struct Act : Base { Str m_name; IP PTrophy; Act(Owner* o, Thing* t, const Str& n) : Base(o, t, 0, 1000), m_name(n), PTrophy() { PTrophy.SetPItem(t); } };
int main()
{
    Owner o; Thing t; Str s("TROPHY"); Act a(&o, &t, s);
    if (g_pri != 1000 || g_f != 0 || a.m_name.p != s.p || a.PTrophy.m_p != &t) { std::printf("006df800_TEST FAIL\n"); return 1; }
    std::printf("006df800_TEST PASS\n");
    return 0;
}