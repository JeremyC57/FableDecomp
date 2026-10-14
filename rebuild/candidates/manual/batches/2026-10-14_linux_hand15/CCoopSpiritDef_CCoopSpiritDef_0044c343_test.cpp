#include <stdio.h>
#include <stddef.h>
static int g_baseCalls;
struct CShoveHaPennyRegionDef { virtual void v00() {} unsigned char _body[0x24]; CShoveHaPennyRegionDef() { ++g_baseCalls; } };
struct CDefString { long m_index; CDefString() : m_index(-1) { } };
struct CCoopSpiritDef : CShoveHaPennyRegionDef {
    CDefString m_name0, m_name1, m_name2, m_name3;
    virtual void v00() {}
    CCoopSpiritDef();
};
CCoopSpiritDef::CCoopSpiritDef() : CShoveHaPennyRegionDef(), m_name0(), m_name1(), m_name2(), m_name3() { }
int main()
{
    CCoopSpiritDef d;
    bool ok = g_baseCalls == 1 && d.m_name0.m_index == -1 && d.m_name1.m_index == -1 && d.m_name2.m_index == -1 && d.m_name3.m_index == -1 && offsetof(CCoopSpiritDef, m_name3) == 0x34;
    if (!ok) { printf("0044c343_TEST FAIL\n"); return 1; }
    printf("0044c343_TEST PASS\n"); return 0;
}
