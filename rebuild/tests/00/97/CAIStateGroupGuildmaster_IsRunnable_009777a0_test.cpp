// Behaviour test for 009777a0: the model below is a copy of src/009777a0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_registers = 0;
class CThingCreatureBase {};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; m_pItem = 0; }
    CThingCreatureBase* GetPItem() const { ++g_gets; return (CThingCreatureBase*)m_pItem; }
    void SetPItem(void* item) { ++g_sets; m_pItem = item; }
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() {}
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
static CThingCreatureBase g_hero, g_creature;
class CPlayer { public:
    CThingCreatureBase* GetPCharacter() { return &g_hero; }
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() { static CPlayer p; return &p; }
};
static CThingCreatureBase* g_regCreature = 0; static CThingCreatureBase* g_regEnemy = 0; static bool g_regFight = false;
class CCombatManager { public:
    void RegisterCombatant(CThingCreatureBase* creature, CThingCreatureBase* enemy, bool fight) { ++g_registers; g_regCreature = creature; g_regEnemy = enemy; g_regFight = fight; }
};
class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;
};
static CWorld g_world; static CCombatManager g_combat; static CPlayerManager g_players;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CPlayerManager* GetPPlayerManager() { return &g_players; }
    CWorld* GetPWorld() { return &g_world; }
protected:
    virtual bool IsRunnable() { return false; }
};
struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    long AttackNum;
    CIntelligentPointer<CThingCreatureBase> PEnemy;
    bool Run() { return IsRunnable(); }
protected:
    virtual bool IsRunnable();
};

bool CAIStateGroup_Guildmaster::IsRunnable()
{
    PEnemy = CIntelligentPointer<CThingCreatureBase>(GetPPlayerManager()->GetMainPlayer()->GetPCharacter());
    CThingCreatureBase* creature = PBrain->PCreature;
    CCombatManager* combat = GetPWorld()->m_pCombatManager;
    combat->RegisterCombatant(creature, PEnemy.GetPItem(), true);
    return true;
}

int main()
{
    bool ok = true;
    CAIBrain brain; std::memset(&brain, 0, sizeof(brain)); brain.PCreature = &g_creature;
    g_world.m_pCombatManager = &g_combat;
    {
        CAIStateGroup_Guildmaster g; g.PBrain = &brain;
        g_ctors = g_dtors = g_sets = g_gets = g_registers = 0;
        ok = ok && g.Run();
        ok = ok && g.PEnemy.m_pItem == &g_hero;                       // the enemy is the main player's character
        ok = ok && g_ctors == 1 && g_dtors == 1;                      // one temporary pointer built and destroyed
        ok = ok && g_sets == 2 && g_gets == 2;                        // temp set, member set from temp, member read for the register
        ok = ok && g_registers == 1 && g_regCreature == &g_creature && g_regEnemy == &g_hero && g_regFight;
    }
    if (!ok) { std::printf("009777a0_TEST FAIL\n"); return 1; }
    std::printf("009777a0_TEST PASS\n");
    return 0;
}