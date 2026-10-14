#include <stdio.h>
#include <stddef.h>
static int g_baseCalls;
struct CShoveHaPennyRegionDef { virtual void v00() {} unsigned char _body[0x24]; CShoveHaPennyRegionDef() { ++g_baseCalls; } };
struct CDefString { long m_index; CDefString() : m_index(-1) { } };
struct CBedDef : CShoveHaPennyRegionDef {
    CDefString GetInBedAnimName, SleepInBedAnimName, GetUpFromBedAnimName, GetUpFromBedFastAnimName;
    virtual void v00() {}
    CBedDef();
};
CBedDef::CBedDef() : CShoveHaPennyRegionDef(), GetInBedAnimName(), SleepInBedAnimName(), GetUpFromBedAnimName(), GetUpFromBedFastAnimName() { }
int main()
{
    CBedDef d;
    bool ok = g_baseCalls == 1 && d.GetInBedAnimName.m_index == -1 && d.SleepInBedAnimName.m_index == -1 && d.GetUpFromBedAnimName.m_index == -1 && d.GetUpFromBedFastAnimName.m_index == -1 && offsetof(CBedDef, GetUpFromBedFastAnimName) == 0x34;
    if (!ok) { printf("004d7a25_TEST FAIL\n"); return 1; }
    printf("004d7a25_TEST PASS\n"); return 0;
}
