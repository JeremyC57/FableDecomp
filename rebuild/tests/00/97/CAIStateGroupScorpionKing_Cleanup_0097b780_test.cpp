// Behaviour test for 0097b780: the model below is a copy of src/0097b780.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_deregisters = 0, g_resets = 0, g_lowerBounds = 0, g_worldCalls = 0, g_combatantCalls = 0, g_isCombatant = 0;
class CTCBase;
class CThing;
class CWorld;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { ++g_lowerBounds; SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    virtual bool IsStillValid() { return true; }
    virtual void Cleanup() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct CBaseIntelligentPointerCtor {
    CThing* m_pItem;
    CBaseIntelligentPointerCtor() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
    ~CBaseIntelligentPointerCtor() {}
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
static CThing* g_deregistered = 0;
class CCombatManager { public:
    void DeregisterCombatant(CThing* creature) { ++g_deregisters; g_deregistered = creature; }
};
class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;
};
struct CThingCombatant : CThing {
    unsigned char _padx[0xb4 - sizeof(CThing)];
    float Health;
    bool IsCombatant() { ++g_combatantCalls; return g_isCombatant != 0; }
};
struct CTCTrap {
    void Reset() { ++g_resets; }
};
struct CIntelligentPointerVector {
    CBaseIntelligentPointerCtor* First;
    CBaseIntelligentPointerCtor* Last;
    CBaseIntelligentPointerCtor* End;
    unsigned long _alloc;
    unsigned long size() const { return Last - First; }
    CBaseIntelligentPointerCtor* begin() { return First; }
    CBaseIntelligentPointerCtor& operator[](unsigned long i) { return *(begin() + i); }
};
static CWorld g_world; static CCombatManager g_combat;
struct CAIStateGroup_ScorpionKing : CAIStateGroupBase {
    CBaseIntelligentPointerCtor PArenaCentre;
    CIntelligentPointerVector TrapThings;
    CWorld* GetPWorld() { ++g_worldCalls; return &g_world; }
    virtual void Cleanup();
};

void CAIStateGroup_ScorpionKing::Cleanup()
{
    if (((CThingCombatant*)PBrain->PCreature)->IsCombatant()) {
        CThing* combatant = PBrain->PCreature;
        CWorld* world = GetPWorld();
        world->PCombatManager->DeregisterCombatant(combatant);
    }
    CThing* creature = PBrain->PCreature;
    if (creature != 0 && !(creature->m_flags91 & 1) && !(((CThingCombatant*)creature)->Health < 1.0f))
        return;
    for (unsigned long i = 0; i < TrapThings.size(); ++i) {
        if (IS_THING_ALIVE(TrapThings[i])) {
            CThing* trap = TrapThings[i].GetPItem();
            if (trap->HasTC(0x7d))
                ((CTCTrap*)trap->FindEntry(0x7d)->component)->Reset();
        }
    }
}

static CThingCombatant g_king; static CThing g_traps[3]; static CTCTrap g_trapTC; static CInterfaceMap::SEntry g_entries[3][2];
static bool Run(bool combatant, bool kingDead, float health, int expectDeregs, int expectResets)
{
    std::memset(&g_king, 0, sizeof(g_king)); g_king.Health = health; g_king.m_flags91 = kingDead ? 1 : 0; g_isCombatant = combatant;
    g_world.PCombatManager = &g_combat;
    CBaseIntelligentPointerCtor ptrs[3];
    for (int i = 0; i < 3; ++i) {
        std::memset(&g_traps[i], 0, sizeof(CThing));
        g_entries[i][0].type = 5; g_entries[i][0].component = 0; g_entries[i][1].type = 0x7d; g_entries[i][1].component = (CTCBase*)&g_trapTC;
        g_traps[i].m_interfaces.first = g_entries[i]; g_traps[i].m_interfaces.last = g_entries[i] + 2;
        ptrs[i].SetPItem(&g_traps[i]);
    }
    g_traps[0].m_presentBits[0x7d >> 5] |= 1UL << (0x7d & 31);    // trap 0 has the component
    g_traps[1].m_flags91 = 1;                                     // trap 1 is dead
    g_traps[2].m_presentBits[0x7d >> 5] |= 1UL << (0x7d & 31);    // trap 2 has it too
    CAIBrain brain; std::memset(&brain, 0, sizeof(brain)); brain.PCreature = &g_king;
    CAIStateGroup_ScorpionKing s; s.PBrain = &brain; s.TrapThings.First = ptrs; s.TrapThings.Last = ptrs + 3; s.TrapThings.End = ptrs + 3; s.TrapThings._alloc = 0;
    g_deregisters = g_resets = g_lowerBounds = g_worldCalls = g_combatantCalls = 0; g_deregistered = 0;
    s.Cleanup();
    if (g_combatantCalls != 1 || g_deregisters != expectDeregs) return false;
    if (expectDeregs && (g_deregistered != &g_king || g_worldCalls != 1)) return false;
    if (g_resets != expectResets || g_lowerBounds != expectResets) return false;
    return true;
}
int main()
{
    bool ok = true;
    ok = ok && Run(true, false, 50.0f, 1, 0);     // alive combatant with health: deregistered, traps untouched
    ok = ok && Run(false, false, 50.0f, 0, 0);    // not a combatant, still alive: nothing
    ok = ok && Run(true, true, 50.0f, 1, 2);      // dead: traps 0 and 2 reset (trap 1 is dead)
    ok = ok && Run(false, false, 0.5f, 0, 2);     // health below 1: traps reset
    if (!ok) { std::printf("0097b780_TEST FAIL\n"); return 1; }
    std::printf("0097b780_TEST PASS\n");
    return 0;
}