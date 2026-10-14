#include <cstdio>
struct Owner { };
struct Str { const char* p; Str(const char* t, long l = -1) : p(t) {} };
struct Pair { long a, b; Pair() : a(0), b(0) {} };
static long g_code = -1; static const char* g_name = "x";
Pair GetSoundPair(Owner* o, const Str& s) { g_name = s.p; Pair p; p.b = 5; return p; }
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, long c, const Pair& s) { g_code = c + s.b; } };
struct Act : Base { unsigned char m_flag; Act(Owner* o) : Base(o, 150, GetSoundPair(o, 0)), m_flag(0) {} };
int main()
{
    Owner o; Act a(&o);
    if (g_code != 155 || g_name != 0 || a.m_flag != 0) { std::printf("008b8440_TEST FAIL\n"); return 1; }
    std::printf("008b8440_TEST PASS\n");
    return 0;
}