#include <cstdio>
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) {} };
static long g_code = -1;
struct Owner { };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, long c, const CSoundPair& s) { g_code = c + s.a + s.b; } };
struct Act : Base { long m_mode; Act(Owner* o) : Base(o, 0, CSoundPair()), m_mode(2) {} };
int main()
{
    Owner o; Act a(&o);
    if (g_code != 0 || a.m_mode != 2) { std::printf("008b8590_TEST FAIL\n"); return 1; }
    std::printf("008b8590_TEST PASS\n");
    return 0;
}