#include <cstdio>
struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };
static long g_code = -1;
struct Owner { };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, long c, const CSoundPair& s) { g_code = c + s.a + s.b; } };
struct Mid : Base { unsigned char m_isDone; Mid(Owner* o) : Base(o, 0, CSoundPair()), m_isDone(0) {} };
struct Act : Mid { Act(Owner* o) : Mid(o) {} };
int main()
{
    Owner o; Act a(&o);
    if (g_code != 0 || a.m_isDone != 0) { std::printf("008cbf00_TEST FAIL\n"); return 1; }
    std::printf("008cbf00_TEST PASS\n");
    return 0;
}
