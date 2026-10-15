// Behaviour test for CAIStateGroup_JackDragon::CAIStateGroup_JackDragon (0x00900647): the base is constructed,
// then all seven intelligent pointers are default-constructed empty.
#include <cstdio>

static int g_baseCtors = 0;
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_baseCtors; }
    virtual ~CBaseIntelligentPointer() {}
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
struct CThing {}; struct CThingObject {}; struct CThingBuilding {}; struct CThingAICreature {};

static int g_stateBase = 0;
struct CAIStateGroupBase {
    CAIStateGroupBase() { ++g_stateBase; }
    virtual ~CAIStateGroupBase() {}
};
struct CThingCreatureBase {};
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CIntelligentPointer<CThingCreatureBase> PEnemy;
    CIntelligentPointer<CThing> PLandedMarker, PHoverMarker1, PHoverMarker2, PStrafeMarker1, PStrafeMarker2;
    CIntelligentPointer<CThingAICreature> PInvisibleBodgeCreature;
    CAIStateGroup_JackDragon() : CAIStateGroupBase() { }
};

int main()
{
    CAIStateGroup_JackDragon g;
    if (g_stateBase != 1 || g_baseCtors != 7 || g.PEnemy.m_pItem || g.PLandedMarker.m_pItem || g.PHoverMarker1.m_pItem
        || g.PHoverMarker2.m_pItem || g.PStrafeMarker1.m_pItem || g.PStrafeMarker2.m_pItem || g.PInvisibleBodgeCreature.m_pItem) { std::printf("00900647_TEST FAIL\n"); return 1; }
    std::printf("00900647_TEST PASS\n");
    return 0;
}
