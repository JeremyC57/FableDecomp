// Behaviour test for CAIStateGroup_GoToPub::CAIStateGroup_GoToPub (0x00900173): the base is constructed,
// then all five intelligent pointers are default-constructed empty.
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
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PPub;
    CIntelligentPointer<CThingObject> PBar;
    CIntelligentPointer<CThingObject> PTable;
    CIntelligentPointer<CThingAICreature> PBarman;
    CIntelligentPointer<CThing> PTankard;
    CAIStateGroup_GoToPub() : CAIStateGroupBase() { }
};

int main()
{
    CAIStateGroup_GoToPub g;
    if (g_stateBase != 1 || g_baseCtors != 5 || g.PPub.m_pItem || g.PBar.m_pItem || g.PTable.m_pItem
        || g.PBarman.m_pItem || g.PTankard.m_pItem) { std::printf("00900173_TEST FAIL\n"); return 1; }
    std::printf("00900173_TEST PASS\n");
    return 0;
}