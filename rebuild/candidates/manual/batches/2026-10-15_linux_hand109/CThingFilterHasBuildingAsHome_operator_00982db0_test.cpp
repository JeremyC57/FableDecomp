// Behaviour test for CThingFilter_HasBuildingAsHome::operator() (definition word) (0x00982DB0)
#include <cstdio>
#include <cstring>

struct CDef { long m_value; };
struct CThingAICreature { CDef* m_pDef; long GetDefinitionValue() const { return m_pDef->m_value; } };
struct CThingFilter_HasBuildingAsHome {
    long m_value;
    bool operator()(const CThingAICreature& creature) const { return creature.GetDefinitionValue() == m_value; }
};
#define CHECK(c) if (!(c)) { std::printf("00982db0_TEST FAIL\n"); return 1; }
int main()
{
    CDef d = { 12 }; CThingAICreature c; c.m_pDef = &d;
    CThingFilter_HasBuildingAsHome f = { 12 };
    CHECK(f(c));
    d.m_value = 1;
    CHECK(!f(c));
    std::printf("00982db0_TEST PASS\n");
    return 0;
}
