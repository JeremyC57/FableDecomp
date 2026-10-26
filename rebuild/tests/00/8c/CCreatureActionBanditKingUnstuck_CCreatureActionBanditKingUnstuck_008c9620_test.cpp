#include <cstdio>
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) {} };
static long g_code = -1; static long g_snd = 1;
struct Owner { };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, long c, const CSoundPair& s) { g_code = c; g_snd = s.a + s.b; } };
struct Act : Base { unsigned char m_114; unsigned char m_done; Act(Owner* o) : Base(o, 150, CSoundPair()), m_done(1) {} };
int main()
{
    Owner o; Act a(&o);
    if (g_code != 150 || g_snd != 0 || a.m_done != 1) { std::printf("008c9620_TEST FAIL\n"); return 1; }
    std::printf("008c9620_TEST PASS\n");
    return 0;
}