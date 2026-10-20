// Behaviour test for 00933470: body copied from src/00933470.cpp (CAIStateGroup_MinionGuardReactToBribe::IsStillValid), the callees stubbed.
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData; unsigned char Terminated; unsigned char Priority; unsigned char Enabled; unsigned char _pad_0x13;
};
struct CTCBribeState { unsigned char _pad[0x10]; long m_value; long GetValue() const { return m_value; } };
struct CAIStateGroup_MinionGuardReactToBribe : CAIStateGroupBase {
    CBaseIntelligentPointer PHero;
    long FrameLastReactedToReceiveing;
    bool HasRecievedBribe;
    virtual bool IsStillValid();
};
bool CAIStateGroup_MinionGuardReactToBribe::IsStillValid()
{
    if (IS_THING_ALIVE(PHero)) {
        CThing* creature = PBrain->PCreature;
        CTCBribeState* pState = 0;
        if (creature->HasTC(0xb4))
            pState = creature->FindTC<CTCBribeState>(0xb4);
        if (!HasRecievedBribe)
            return true;
        if (pState->GetValue() >= 0)
            return true;
    }
    return false;
}
int main()
{
    CThing hero; memset(&hero, 0, sizeof(hero)); CThing creature; memset(&creature, 0, sizeof(creature)); CTCBribeState state; state.m_value = 5;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0xb4, (CTCBase*)&state } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    CAIBrain brain; brain.PCreature = &creature; CAIStateGroup_MinionGuardReactToBribe g; g.PBrain = &brain; g.PHero.m_pItem = 0; g.HasRecievedBribe = false;
    // 1. no hero / dead hero: false
    CHECK(g.IsStillValid() == false);
    g.PHero.m_pItem = &hero; hero.m_flags91 = 1; CHECK(g.IsStillValid() == false); hero.m_flags91 = 0;
    // 2. hero alive, no bribe received yet: true (even without the bribe component)
    CHECK(g.IsStillValid() == true);
    // 3. bribe received: the component's value decides
    g.HasRecievedBribe = true; creature.m_presentBits[0xb4 >> 5] = 1UL << (0xb4 & 31);
    CHECK(g.IsStillValid() == true);
    state.m_value = -1; CHECK(g.IsStillValid() == false);
    state.m_value = 0; CHECK(g.IsStillValid() == true);
    if (fails == 0) printf("00933470_TEST PASS\n"); else printf("00933470_TEST FAIL\n");
    return fails;
}