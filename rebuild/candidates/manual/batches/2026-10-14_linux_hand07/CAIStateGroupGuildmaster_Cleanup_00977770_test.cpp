#include <cstdio>
// Behaviour model of CAIStateGroup_Guildmaster::Cleanup @ 0x00977770.
static bool g_registered = true; static void* g_deregistered = 0;
struct CThingAICreature { bool IsRegisteredCombatant() { return g_registered; } };
struct CCombatManager { void DeregisterCombatant(CThingAICreature* c) { g_deregistered = c; } };
struct CWorld { unsigned char _pad[0x4c]; CCombatManager* m_pCombatManager; };
struct CAIBrain { unsigned char _pad[0x18]; CThingAICreature* PCreature; };
static CWorld g_world; static CCombatManager g_combat;
struct CAIStateGroupBase { virtual void v00() {} CAIBrain* PBrain; CWorld* GetPWorld() { return &g_world; } };
struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    void Cleanup() { if (PBrain->PCreature->IsRegisteredCombatant()) { CThingAICreature* c = PBrain->PCreature; CWorld* w = GetPWorld(); w->m_pCombatManager->DeregisterCombatant(c); } } };
int main()
{
    g_world.m_pCombatManager = &g_combat; CThingAICreature creature; CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_Guildmaster g; g.PBrain = &brain;
    g.Cleanup(); bool ok = g_deregistered == &creature;
    g_deregistered = 0; g_registered = false; g.Cleanup(); ok = ok && g_deregistered == 0;
    if (!ok) { std::printf("00977770_TEST FAIL\n"); return 1; }
    std::printf("00977770_TEST PASS\n");
    return 0;
}
