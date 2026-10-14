#include <cstdio>
struct V { float x, y, z; };
struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };
static long g_g = -1;
struct Owner { };
struct Base { virtual ~Base() {} unsigned char body[0xa4]; Base(Owner* o, const CSoundPair& s, long g, long p) { g_g = g + p; } };
struct Act : Base { V m_dir; Act(Owner* o, const V* d) : Base(o, CSoundPair(), 0, 0), m_dir(*d) {} };
int main()
{
    Owner o; V v = { 1, 2, 3 }; Act a(&o, &v);
    if (g_g != 0 || a.m_dir.x != 1 || a.m_dir.y != 2 || a.m_dir.z != 3) { std::printf("00854610_TEST FAIL\n"); return 1; }
    std::printf("00854610_TEST PASS\n");
    return 0;
}
