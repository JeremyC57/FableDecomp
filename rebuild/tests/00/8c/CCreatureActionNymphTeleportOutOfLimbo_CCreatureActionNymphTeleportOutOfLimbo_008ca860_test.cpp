#include <cstdio>
struct V { float x, y, z; };
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) {} };
static long g_code = 0; static V g_pos = { 7, 8, 9 };
struct Owner { const V* GetPosition() { return &g_pos; } };
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(Owner* o, long c, const CSoundPair& s) { g_code = c; } };
struct Act : Base { V m_dest; Act(Owner* o, const V* d) : Base(o, 150, CSoundPair()), m_dest(*(d ? d : o->GetPosition())) {} };
int main()
{
    Owner o; V v = { 1, 2, 3 }; Act a(&o, &v); Act b(&o, 0);
    if (g_code != 150 || a.m_dest.y != 2 || b.m_dest.z != 9) { std::printf("008ca860_TEST FAIL\n"); return 1; }
    std::printf("008ca860_TEST PASS\n");
    return 0;
}