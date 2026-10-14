#include <cstdio>
#include <cstddef>
// Behaviour model of CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase @ 0x00902FE8.
static int g_order[8]; static int g_n = 0;
struct CAIStateGroupBase { virtual void v00() {} CAIStateGroupBase() { g_order[g_n++] = 1; } };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { g_order[g_n++] = 2; } };
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() {} };
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) { g_order[g_n++] = 3; } };
struct CThing; struct CThingCreatureBase;
struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    unsigned char _base[0x10];
    CIntelligentPointer<CThing> PSwordEmitter, PArenaCentre;
    CIntelligentPointer<CThingCreatureBase> PTargetThing;
    long EnemyKnockDownFrame, SpeechCounter;
    CRetailVector<CIntelligentPointer<CThing> > PillarMarkers, Pillars;
    virtual void v00() {}
    CAIStateGroup_JackOfBladesBase() : CAIStateGroupBase(), PSwordEmitter(), PArenaCentre(), PTargetThing(), PillarMarkers(), Pillars() {}
};
int main()
{
    CAIStateGroup_JackOfBladesBase g;
    bool ok = g_n == 6 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 2 && g_order[3] == 2 && g_order[4] == 3 && g_order[5] == 3
        && g.PSwordEmitter.m_pItem == 0 && g.PArenaCentre.m_pItem == 0 && g.PTargetThing.m_pItem == 0
        && g.PillarMarkers.first == 0 && g.PillarMarkers.end_of_storage == 0 && g.Pillars.first == 0 && g.Pillars.end_of_storage == 0;
    if (sizeof(void*) == 4) ok = ok && offsetof(CAIStateGroup_JackOfBladesBase, PillarMarkers) == 0x34 && offsetof(CAIStateGroup_JackOfBladesBase, Pillars) == 0x40;
    if (!ok) { std::printf("00902fe8_TEST FAIL\n"); return 1; }
    std::printf("00902fe8_TEST PASS\n");
    return 0;
}