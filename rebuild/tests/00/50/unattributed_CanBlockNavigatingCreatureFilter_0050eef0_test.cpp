// Behaviour test for the 0x0050EEF0 thing filter: the physics flag is queried (when a component exists) and the result is always CanBlockNavigatingCreature().
#include <cstdio>

static int g_physicsCalls = 0;
struct CTCPhysicsBase {
    bool m_flag;
    virtual bool IsPhysicsFlagSet() { ++g_physicsCalls; return m_flag; }
};
struct CThingPhysical {
    bool m_blocks;
    CTCPhysicsBase* PhysicsTC;
    virtual bool CanBlockNavigatingCreature() { return m_blocks; }
};
struct CThingFilter_0050EEF0 {
    bool CanBlockNavigatingCreatureFilter(CThingPhysical* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics && physics->IsPhysicsFlagSet())
            return thing->CanBlockNavigatingCreature();
        return thing->CanBlockNavigatingCreature();
    }
};

int main()
{
    CThingFilter_0050EEF0 filter;
    CThingPhysical thing;
    thing.m_blocks = true;
    thing.PhysicsTC = 0;
    if (!filter.CanBlockNavigatingCreatureFilter(&thing) || g_physicsCalls != 0) { std::printf("0050eef0_TEST FAIL\n"); return 1; }
    CTCPhysicsBase physics;
    physics.m_flag = false;
    thing.PhysicsTC = &physics;
    thing.m_blocks = false;
    if (filter.CanBlockNavigatingCreatureFilter(&thing) || g_physicsCalls != 1) { std::printf("0050eef0_TEST FAIL\n"); return 1; }
    physics.m_flag = true;
    thing.m_blocks = true;
    if (!filter.CanBlockNavigatingCreatureFilter(&thing) || g_physicsCalls != 2) { std::printf("0050eef0_TEST FAIL\n"); return 1; }
    std::printf("0050eef0_TEST PASS\n");
    return 0;
}