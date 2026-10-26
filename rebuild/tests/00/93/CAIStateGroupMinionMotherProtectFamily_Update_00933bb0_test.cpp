#include <cstdio>
class CThingAICreature { public:};
class CThing { public: unsigned char _pad[0x91]; unsigned char m_flags91; };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
static int g_live = 0; static int g_actions = 0; static int g_terminated = 0; static CThingAICreature* g_who = 0;
class CCreatureActionBase { public: virtual void v00() {} unsigned char _body[0xa4]; CCreatureActionBase() { ++g_live; } ~CCreatureActionBase() { --g_live; } };
namespace N_00933cc0 { struct CCreatureAction_AlertToPlayer : CCreatureActionBase { CCreatureAction_AlertToPlayer(CThingAICreature* c) { g_who = c; } }; } using N_00933cc0::CCreatureAction_AlertToPlayer;
class CAIBrain { public: unsigned char _pad[0x18]; CThingAICreature* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc];
    long AIAction_PerformActionAndWait(CCreatureActionBase* a, bool w) { if (w || !a || g_live != 1) return -1; ++g_actions; return 2; } void Terminate() { ++g_terminated; } };
struct CAIStateGroup_MinionMotherProtectFamily : CAIStateGroupBase { CBaseIntelligentPointer PFamilyMember; virtual void Update(); };
void CAIStateGroup_MinionMotherProtectFamily::Update()
{
    if (PFamilyMember.GetPItem() != 0 && !(PFamilyMember.GetPItem()->m_flags91 & 1)) { CThingAICreature* creature = PBrain->Creature; CCreatureAction_AlertToPlayer action(creature); long result = AIAction_PerformActionAndWait(&action, false); if (result == 2) { Terminate(); return; } }
    Terminate();
}
int main()
{
    CThingAICreature mother; CThing child; child.m_flags91 = 0; CAIBrain b; b.Creature = &mother; CAIStateGroup_MinionMotherProtectFamily g; g.PBrain = &b; g.PFamilyMember.m_pItem = 0;
    g.Update(); if (g_actions != 0 || g_terminated != 1) { std::printf("00933bb0_TEST FAIL\n"); return 1; }
    g.PFamilyMember.m_pItem = &child; g.Update(); if (g_actions != 1 || g_who != &mother || g_terminated != 2 || g_live != 0) { std::printf("00933bb0_TEST FAIL\n"); return 1; }
    child.m_flags91 = 1; g.Update(); if (g_actions != 1 || g_terminated != 3) { std::printf("00933bb0_TEST FAIL\n"); return 1; }
    std::printf("00933bb0_TEST PASS\n"); return 0;
}