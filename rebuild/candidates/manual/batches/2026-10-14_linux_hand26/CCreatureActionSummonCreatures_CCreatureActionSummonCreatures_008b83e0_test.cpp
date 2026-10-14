#include <cstdio>
struct Owner { }; struct Thing { };
struct Str { const char* p; Str(const char* t, long l = -1) : p(t) {} };
struct Pair { long a, b; Pair() : a(0), b(0) {} };
static long g_code = -1; static const char* g_name = "x";
Pair GetSoundPair(Owner* o, const Str& s) { g_name = s.p; Pair p; p.a = 7; return p; }
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, Thing* t, long c, const Pair& s) { g_code = c + s.a; } };
struct Act : Base { unsigned char m_flag; Act(Owner* o, Thing* t, bool f) : Base(o, t, 150, GetSoundPair(o, 0)), m_flag(f) {} };
int main()
{
    Owner o; Thing t; Act a(&o, &t, true);
    if (g_code != 157 || g_name != 0 || a.m_flag != 1) { std::printf("008b83e0_TEST FAIL\n"); return 1; }
    std::printf("008b83e0_TEST PASS\n");
    return 0;
}
