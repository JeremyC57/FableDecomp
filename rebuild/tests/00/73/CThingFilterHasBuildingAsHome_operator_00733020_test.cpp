// Behaviour test for 00733020: body copied from src/00733020.cpp (CThingFilter_HasBuildingAsHome::operator()), GetHomeBuilding stubbed.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CThingBuilding { int id; };
static CThingBuilding* g_home; static int g_calls;
struct CThingAICreature {
    CThingBuilding* GetHomeBuilding() const { ++g_calls; return g_home; }
};
struct CThingFilter_HasBuildingAsHome {
    const CThingBuilding* m_building;
    bool operator()(const CThingAICreature& creature) const;
};
bool CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature& creature) const
{
    const CThingBuilding* home = creature.GetHomeBuilding();
    return home == m_building;
}
int main()
{
    CThingBuilding a, b; CThingAICreature creature;
    CThingFilter_HasBuildingAsHome f; f.m_building = &a;
    // 1. same building: true
    g_home = &a; g_calls = 0; CHECK(f(creature) == true); CHECK(g_calls == 1);
    // 2. other building: false
    g_home = &b; CHECK(f(creature) == false);
    // 3. homeless creature: false
    g_home = 0; CHECK(f(creature) == false);
    // 4. filter with no building matches the homeless creature
    f.m_building = 0; CHECK(f(creature) == true);
    CHECK(g_calls == 4);
    if (fails == 0) printf("00733020_TEST PASS\n");
    else printf("00733020_TEST FAIL (%d)\n", fails);
    return fails;
}