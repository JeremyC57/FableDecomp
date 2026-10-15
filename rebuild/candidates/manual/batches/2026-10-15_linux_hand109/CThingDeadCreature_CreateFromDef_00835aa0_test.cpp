// Behaviour test for CThingDeadCreature::CreateFromDef (0x00835AA0)
#include <cstdio>
struct CThingBaseDef { unsigned char m_41_0 : 2; unsigned char m_inheritedFlag : 1; unsigned char m_41_3 : 5; };
struct CThingDeadCreature {
    unsigned char m_91_0 : 6; unsigned char m_inheritedFlag : 1; unsigned char m_91_7 : 1;
    void CreateFromDef(const CThingBaseDef& def) { m_inheritedFlag = def.m_inheritedFlag; }
};
#define CHECK(c) if (!(c)) { std::printf("00835aa0_TEST FAIL\n"); return 1; }
int main()
{
    CThingBaseDef d; d.m_41_0 = 3; d.m_inheritedFlag = 1; d.m_41_3 = 0;
    CThingDeadCreature t; t.m_91_0 = 0x2a; t.m_inheritedFlag = 0; t.m_91_7 = 1;
    t.CreateFromDef(d);
    CHECK(t.m_inheritedFlag == 1 && t.m_91_0 == 0x2a && t.m_91_7 == 1);
    d.m_inheritedFlag = 0; d.m_41_0 = 0; d.m_41_3 = 31;
    t.CreateFromDef(d);
    CHECK(t.m_inheritedFlag == 0 && t.m_91_0 == 0x2a && t.m_91_7 == 1);
    std::printf("00835aa0_TEST PASS\n");
    return 0;
}
