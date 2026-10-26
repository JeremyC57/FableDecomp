#include <cstdio>
class CThingCreatureBase { public:};
class CThing { public: unsigned char _pad[0x6c]; unsigned char m_flags6c; };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
class CCombatManager { public: int n; CThingCreatureBase* c; CThing* t; bool h; void RegisterCombatant(CThingCreatureBase* a, CThing* b, bool x) { ++n; c = a; t = b; h = x; } };
static long g_frame = 500; long GFGetCurrentFrame() { return g_frame; }
static int g_worldCalls = 0;
class CWorld { public: unsigned char _pad[0x4c]; CCombatManager* m_pCombatManager; static long GetFrameCount() { return GFGetCurrentFrame(); } };
class CAIBrain { public: unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; CWorld* w; CWorld* GetPWorld() { ++g_worldCalls; return w; } };
struct CAIStateGroup_MazeTutorialBattle : CAIStateGroupBase { CBaseIntelligentPointer PEnemy; unsigned char _pad1c[4]; long FrameOfLastShield; virtual void OnActivate(); };
void CAIStateGroup_MazeTutorialBattle::OnActivate()
{
    if (PEnemy.GetPItem()->m_flags6c & 8) { CThing* enemy = PEnemy.GetPItem(); CThingCreatureBase* creature = PBrain->Creature; CWorld* world = GetPWorld(); world->m_pCombatManager->RegisterCombatant(creature, enemy, true); }
    FrameOfLastShield = GetPWorld()->GetFrameCount();
}
int main()
{
    CThing enemy; enemy.m_flags6c = 0; CThingCreatureBase c; CCombatManager m = { 0, 0, 0, false }; CWorld w; w.m_pCombatManager = &m; CAIBrain b; b.Creature = &c;
    CAIStateGroup_MazeTutorialBattle g; g.PBrain = &b; g.w = &w; g.PEnemy.m_pItem = &enemy; g.FrameOfLastShield = 0;
    g.OnActivate(); if (m.n != 0 || g.FrameOfLastShield != 500 || g_worldCalls != 1) { std::printf("00954190_TEST FAIL\n"); return 1; }
    enemy.m_flags6c = 0x0a; g_frame = 501; g.OnActivate();
    if (m.n != 1 || m.c != &c || m.t != &enemy || !m.h || g.FrameOfLastShield != 501 || g_worldCalls != 3) { std::printf("00954190_TEST FAIL\n"); return 1; }
    std::printf("00954190_TEST PASS\n"); return 0;
}