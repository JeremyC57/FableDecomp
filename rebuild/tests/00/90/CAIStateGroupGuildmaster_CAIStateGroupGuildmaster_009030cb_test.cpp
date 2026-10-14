#include <cstdio>
#include <cstddef>
// Behaviour model of CAIStateGroup_Guildmaster::CAIStateGroup_Guildmaster @ 0x009030CB: base, then the two
// intelligent pointers, then the vector zeroed, in declaration order.
static int g_order[8]; static int g_n = 0;
struct CAIStateGroupBase { virtual void v00() {} CAIStateGroupBase() { g_order[g_n++] = 1; } };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { g_order[g_n++] = 2; } };
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() {} };
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) { g_order[g_n++] = 3; } };
struct CThing; struct CThingCreatureBase;
struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    unsigned char _base[0x10];
    long AttackNum;
    CIntelligentPointer<CThingCreatureBase> PEnemy;
    CIntelligentPointer<CThing> PLastTeleportMarker;
    CRetailVector<CIntelligentPointer<CThing> > TeleportMarkers;
    long LastWillFrame, LastTeleportFrame;
    virtual void v00() {}
    CAIStateGroup_Guildmaster() : CAIStateGroupBase(), PEnemy(), PLastTeleportMarker(), TeleportMarkers() {}
};
int main()
{
    CAIStateGroup_Guildmaster g;
    bool ok = g_n == 4 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 2 && g_order[3] == 3
        && g.PEnemy.m_pItem == 0 && g.PLastTeleportMarker.m_pItem == 0
        && g.TeleportMarkers.first == 0 && g.TeleportMarkers.last == 0 && g.TeleportMarkers.end_of_storage == 0;
    if (sizeof(void*) == 4) ok = ok && offsetof(CAIStateGroup_Guildmaster, PEnemy) == 0x18 && offsetof(CAIStateGroup_Guildmaster, TeleportMarkers) == 0x28;
    if (!ok) { std::printf("009030cb_TEST FAIL\n"); return 1; }
    std::printf("009030cb_TEST PASS\n");
    return 0;
}