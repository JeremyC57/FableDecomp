// Behaviour test for 009492a0: body copied from src/009492a0.cpp (CAIStateGroup_GoToPub::GetPayPoint), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};

static int g_pickup = 0; static bool g_pickupResult = false; static CThing* g_pickupCreature = 0; static void* g_pickupPos = 0; static void* g_pickupRh = 0; static void* g_pickupAp = 0; static void* g_pickupBar = 0; static bool g_pickupReserve = true; static void* g_pickupThis = 0;
struct CTCTavernBar {
    bool GetPickupSlot(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, long* actionPointIndex, long* barIndex, bool reserve)
    { ++g_pickup; g_pickupCreature = creature; g_pickupPos = pos; g_pickupRh = rhSet; g_pickupAp = actionPointIndex; g_pickupBar = barIndex; g_pickupReserve = reserve; g_pickupThis = this; return g_pickupResult; }
};
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CThing* GetPItem() const { return (CThing*)m_pItem; } };
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0x0c]; };
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub, PBar, PTable, PBarman, PTankard;
    long SeatIndex; C3DVector Pos; CRightHandedSet RHSet; long ActionPointIndex; long NextFramePub; long FrameLeavePub; bool Sitting;
    long BarActionPointIndex; C3DVector BarPos; CRightHandedSet BarRHSet; long BarIndex; bool HandsOffMode; bool UnderScriptControl; long TimeBeforeAskingForBeer;
    bool GetPayPoint();
};
bool CAIStateGroup_GoToPub::GetPayPoint()
{
    CThing* barThing = PBar.GetPItem();
    if (barThing->HasTC(0xb9)) {
        CTCTavernBar* bar = barThing->GetTC<CTCTavernBar>(0xb9);
        CThing* creature = PBrain->PCreature;
        if (bar->GetPickupSlot(creature, &BarPos, &BarRHSet, &BarActionPointIndex, &BarIndex, false))
            return true;
    }
    return false;
}
int main()
{
    CThing barThing; memset(&barThing, 0, sizeof(barThing)); CTCTavernBar bar; CThing creature;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0xb9, (CTCBase*)&bar } };
    barThing.m_interfaces.first = entries; barThing.m_interfaces.last = entries + 2;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_GoToPub g; g.PBrain = &brain; g.PBar.m_pItem = &barThing;
    CHECK(sizeof(CAIStateGroup_GoToPub) == 0xa8);
    // 1. no bar component: false, no slot query
    CHECK(g.GetPayPoint() == false && g_pickup == 0);
    // 2. bar present, slot found
    barThing.m_presentBits[0xb9 >> 5] = 1UL << (0xb9 & 31); g_pickupResult = true;
    CHECK(g.GetPayPoint() == true && g_pickup == 1 && g_pickupThis == &bar && g_pickupCreature == &creature);
    CHECK(g_pickupPos == &g.BarPos && g_pickupRh == &g.BarRHSet && g_pickupAp == &g.BarActionPointIndex && g_pickupBar == &g.BarIndex && g_pickupReserve == false);
    // 3. bar present, no slot
    g_pickupResult = false; CHECK(g.GetPayPoint() == false && g_pickup == 2);
    if (fails == 0) printf("009492a0_TEST PASS\n"); else printf("009492a0_TEST FAIL\n");
    return fails;
}