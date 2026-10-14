#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CThingCreatureBase;
struct CCombatManager { void RegisterCombatant(CThingCreatureBase* c, CThingCreatureBase* t, bool h) { Log(h ? "reg-hostile" : "reg"); Log(c == t ? "same" : "pair"); } };
struct CThingCreatureBase { unsigned char _pad[0x91]; unsigned char IsDead : 1; CCombatManager m_cm; CCombatManager* GetCombatManager() { return &m_cm; } };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)m_pItem; } };
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase { CIntelligentPointer PTargetThing; };
struct CAIStateGroup_CreatureRangedCombat2 : CAIStateGroup_EnemyCreatureBase {
    bool AddedStrafe;
    virtual void OnActivate() { if (PTargetThing.GetPItem() != 0 && !PTargetThing.GetPItem()->IsDead) { CThingCreatureBase* creature = PBrain->Creature; creature->GetCombatManager()->RegisterCombatant(creature, PTargetThing.GetPItem(), false); } }
};
int main()
{
    CThingCreatureBase me, target; me.IsDead = 0; target.IsDead = 1; CAIBrain b; b.Creature = &me;
    CAIStateGroup_CreatureRangedCombat2 g; g.PBrain = &b;
    g.PTargetThing.m_pItem = 0; g.OnActivate();
    g.PTargetThing.m_pItem = &target; g.OnActivate();
    if (g_n != 0) return Check("0091ea20", "x");
    target.IsDead = 0; g.OnActivate();
    return Check("0091ea20", "reg pair ");
}