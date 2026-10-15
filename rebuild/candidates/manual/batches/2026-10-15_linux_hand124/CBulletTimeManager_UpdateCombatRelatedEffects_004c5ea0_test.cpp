#include <cstdio>
struct CStrikeQuery { bool m_striking; virtual ~CStrikeQuery() { } virtual bool IsStriking() { return m_striking; } };
struct CThingPlayerCreature { CStrikeQuery* m_pStrikeQuery; bool m_busy; bool IsPerformingAnyAction() { return m_busy; } };
struct CPlayer { bool m_init; CThingPlayerCreature* m_creature; bool IsInitialised() { return m_init; } CThingPlayerCreature* GetPlayerCreature() { return m_creature; } };
struct CPlayerManager { CPlayer* m_main; int m_calls; CPlayer* GetMainPlayer() { ++m_calls; return m_main; } };
long g_BulletTimeStrikeSeconds = 3, g_BulletTimeFramesPerSecond = 10;
struct CBulletTimeManager {
    bool Override; bool UseDefaultMultiplier; long LastStrikeActionTick; CPlayerManager* PlayerManager;
    void UpdateCombatRelatedEffects()
    {
        if (!PlayerManager->GetMainPlayer()->IsInitialised()) return;
        CThingPlayerCreature* creature = PlayerManager->GetMainPlayer()->GetPlayerCreature();
        if (creature == 0 || !creature->IsPerformingAnyAction()) return;
        if (creature->m_pStrikeQuery->IsStriking()) { UseDefaultMultiplier = true; LastStrikeActionTick = g_BulletTimeStrikeSeconds * g_BulletTimeFramesPerSecond; }
        else if (LastStrikeActionTick > 0) { LastStrikeActionTick = LastStrikeActionTick - 1; }
    }
};
int main()
{
    CStrikeQuery q; q.m_striking = true;
    CThingPlayerCreature cr; cr.m_pStrikeQuery = &q; cr.m_busy = true;
    CPlayer p; p.m_init = true; p.m_creature = &cr;
    CPlayerManager pm; pm.m_main = &p; pm.m_calls = 0;
    CBulletTimeManager m; m.Override = false; m.UseDefaultMultiplier = false; m.LastStrikeActionTick = 0; m.PlayerManager = &pm;
    m.UpdateCombatRelatedEffects();
    if (!m.UseDefaultMultiplier || m.LastStrikeActionTick != 30 || pm.m_calls != 2) { std::printf("004c5ea0_TEST FAIL\n"); return 1; }
    q.m_striking = false; m.UpdateCombatRelatedEffects();
    if (m.LastStrikeActionTick != 29) { std::printf("004c5ea0_TEST FAIL\n"); return 1; }
    m.LastStrikeActionTick = 0; m.UpdateCombatRelatedEffects();
    if (m.LastStrikeActionTick != 0) { std::printf("004c5ea0_TEST FAIL\n"); return 1; }
    cr.m_busy = false; m.LastStrikeActionTick = 5; m.UpdateCombatRelatedEffects();
    if (m.LastStrikeActionTick != 5) { std::printf("004c5ea0_TEST FAIL\n"); return 1; }
    p.m_init = false; pm.m_calls = 0; m.UpdateCombatRelatedEffects();
    if (pm.m_calls != 1) { std::printf("004c5ea0_TEST FAIL\n"); return 1; }
    p.m_init = true; p.m_creature = 0; m.UpdateCombatRelatedEffects();
    if (m.LastStrikeActionTick != 5) { std::printf("004c5ea0_TEST FAIL\n"); return 1; }
    std::printf("004c5ea0_TEST PASS\n");
    return 0;
}
