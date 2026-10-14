#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; struct CDefClassBase { virtual void v00() { } unsigned char _body[0x24]; CDefClassBase() { } CDefClassBase(const CDefClassBase&) { ++g_base; } };
struct CDef : CDefClassBase { long m_28; bool m_2c, m_2d; long m_30, m_34; CDef() { } CDef(const CDef& o) : CDefClassBase(o) { m_28 = o.m_28; m_2c = o.m_2c; m_2d = o.m_2d; m_30 = o.m_30; m_34 = o.m_34; } };

int main() {
    CDef a; a.m_28 = 1; a.m_2c = true; a.m_2d = false; a.m_30 = 3; a.m_34 = 4; CDef b(a); if (g_base != 1 || b.m_28 != 1 || !b.m_2c || b.m_2d || b.m_30 != 3 || b.m_34 != 4) { std::printf("004eb864_TEST FAIL\n"); return 1; }
    std::printf("004eb864_TEST PASS\n"); return 0;
}