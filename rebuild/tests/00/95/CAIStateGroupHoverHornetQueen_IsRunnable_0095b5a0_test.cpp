// Behaviour test for 0095b5a0: the model below is a copy of src/0095b5a0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_registers = 0, g_arenas = 0, g_lowerBounds = 0;
struct CTCBase;
struct CThing;
struct CWorld;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { ++g_lowerBounds; SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { ++g_gets; return m_pItem; }
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    void SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;
};
struct CCombatManager;
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;
};
static CWorld g_world;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual long GetTimeToInitiate() { return 0; }
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld() { return &g_world; }
};
static CThing* g_combatEnemy = 0;
struct CTCCombatTarget {
    virtual CThing* GetEnemy() { return g_combatEnemy; }
};
static CThing* g_hero = 0;
struct CPlayer {
    CThing* GetPCharacter() { return g_hero; }
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() { static CPlayer p; return &p; }
};
static CThing* g_regCreature = 0; static CThing* g_regTarget = 0; static bool g_regNotify = false;
struct CCombatManager {
    void RegisterCombatant(CThing* creature, CThing* target, bool notify) { ++g_registers; g_regCreature = creature; g_regTarget = target; g_regNotify = notify; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CIntelligentPointerCreature() : CBaseIntelligentPointer() {}
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    CIntelligentPointerCreature PTargetThing;
    CPlayerManager* GetPPlayerManager() { static CPlayerManager m; return &m; }
    void SetupArena() { ++g_arenas; }
    virtual bool IsRunnable();
};

bool CAIStateGroup_HoverHornetQueen::IsRunnable()
{
    CTCCombatTarget* combat;
    PBrain->PCreature->GetInterfaceT(&combat, 8);
    CThing* enemy = combat->GetEnemy();
    if (!enemy)
        enemy = GetPPlayerManager()->GetMainPlayer()->GetPCharacter();
    if (enemy) {
        PTargetThing = CIntelligentPointerCreature(enemy);
        CThing* creature = PBrain->PCreature;
        CCombatManager* manager = GetPWorld()->CombatManager;
        manager->RegisterCombatant(creature, PTargetThing.GetPItem(), true);
        SetupArena();
        return true;
    }
    return false;
}

static bool Run(bool combatEnemy, bool hero, bool expect)
{
    static CThing creature, enemy, heroThing; static CTCCombatTarget target; static CCombatManager combat;
    static CInterfaceMap::SEntry entries[2];
    std::memset(&creature, 0, sizeof(creature));
    entries[0].type = 3; entries[0].component = 0; entries[1].type = 8; entries[1].component = (CTCBase*)&target;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2; creature.m_presentBits[0] |= 1UL << 8;
    g_combatEnemy = combatEnemy ? &enemy : 0; g_hero = hero ? &heroThing : 0; g_world.CombatManager = &combat;
    CAIBrain brain; std::memset(&brain, 0, sizeof(brain)); brain.PCreature = &creature;
    CAIStateGroup_HoverHornetQueen s; s.PBrain = &brain;
    g_ctors = g_dtors = g_sets = g_gets = g_registers = g_arenas = g_lowerBounds = 0;
    bool r = s.IsRunnable();
    if (r != expect || g_lowerBounds != 1) return false;
    if (expect) {
        CThing* want = combatEnemy ? &enemy : &heroThing;
        if (s.PTargetThing.m_pItem != want || g_regTarget != want || g_regCreature != &creature || !g_regNotify) return false;
        if (g_registers != 1 || g_arenas != 1 || g_ctors != 1 || g_dtors != 1 || g_sets != 2 || g_gets != 2) return false;
    } else if (g_registers != 0 || g_arenas != 0 || g_ctors != 0 || s.PTargetThing.m_pItem != 0) return false;
    return true;
}
int main()
{
    bool ok = Run(true, true, true) && Run(false, true, true) && Run(false, false, false);
    if (!ok) { std::printf("0095b5a0_TEST FAIL\n"); return 1; }
    std::printf("0095b5a0_TEST PASS\n");
    return 0;
}