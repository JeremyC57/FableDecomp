#include <cstdio>
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) {} };
static long g_code = -1;
struct Owner { };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, long c, const CSoundPair& s) { g_code = c + s.a + s.b; } };
struct Act : Base { long m_mode; Act(Owner* o, long m) : Base(o, 250, CSoundPair()), m_mode(m) {} };
int main()
{
    Owner o; Act a(&o, 7);
    if (g_code != 250 || a.m_mode != 7) { std::printf("008c5b30_TEST FAIL\n"); return 1; }
    std::printf("008c5b30_TEST PASS\n");
    return 0;
}