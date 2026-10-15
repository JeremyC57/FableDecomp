// Behaviour test for CThingFilter_AICreatureHasProperty::operator() (slot 48) (0x006A75D0)
#include <cstdio>
#include <cstring>

struct CThingAICreature { long m_value; virtual long GetAIProperty() const { return m_value; } };
struct CThingFilter_AICreatureHasProperty {
    long m_property;
    bool operator()(const CThingAICreature& creature) const { return creature.GetAIProperty() == m_property; }
};
#define CHECK(c) if (!(c)) { std::printf("006a75d0_TEST FAIL\n"); return 1; }
int main()
{
    CThingAICreature c; c.m_value = 9;
    CThingFilter_AICreatureHasProperty f = { 9 };
    CHECK(f(c));
    f.m_property = 8;
    CHECK(!f(c));
    std::printf("006a75d0_TEST PASS\n");
    return 0;
}
