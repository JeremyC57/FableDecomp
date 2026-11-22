// Behaviour test for 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable (self-contained model over stubs).
// Cases: no best enemy; dead best enemy; alive but not still runnable; still runnable but not navigable; navigable. Checks that PEnemy
// is set whenever the enemy is alive, and that the navigation query gets both positions and the manager's proximity (a reference getter).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CThingCreatureBase { bool Dead; C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
static bool IS_THING_ALIVE(const CThingCreatureBase* p) { return p != 0 && !p->Dead; }
struct CCombatManager {
    float ProximityForNavigation;
    const float& GetProximityForNavigation() const { return ProximityForNavigation; }
};
struct CTCTargetingBase { CThingCreatureBase* Best; CThingCreatureBase* GetPBestEnemyCreatureForAttack() const { return Best; } };
static int g_navCalls; static C3DVector g_from, g_to; static float g_prox; static bool g_navigable;
struct CTCCreatureNavigation {
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float prox)
    { ++g_navCalls; g_from = from; g_to = to; g_prox = prox; return g_navigable; }
};
struct CThingAICreature {
    CTCTargetingBase Targeting; CTCCreatureNavigation Nav; CCombatManager Combat; C3DVector Pos;
    void PeekTC(const CTCTargetingBase** out) const { *out = &Targeting; }
    void GetTC(CTCCreatureNavigation** out) { *out = &Nav; }
    const C3DVector& GetPos() const { return Pos; }
    CCombatManager& GetCombatManager() { return Combat; }
};
struct CAIStateGroup_MeleeCombatNearbyBase {
    CThingAICreature* Creature; CThingCreatureBase* PEnemy; bool StillRunnable;
    CThingAICreature& GetCreature() { return *Creature; }
    bool IsStillRunnable(bool) { return StillRunnable; }
    bool IsRunnable();
};
bool CAIStateGroup_MeleeCombatNearbyBase::IsRunnable()
{
    const CTCTargetingBase* ptc_targeting;
    GetCreature().PeekTC(&ptc_targeting);
    CThingCreatureBase* pcreature_base = ptc_targeting->GetPBestEnemyCreatureForAttack();
    if (IS_THING_ALIVE(pcreature_base))
    {
        PEnemy = pcreature_base;
        if (IsStillRunnable(false))
        {
            CTCCreatureNavigation* ptc_nav;
            GetCreature().GetTC(&ptc_nav);
            if (ptc_nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(), PEnemy->GetPos(), GetCreature().GetCombatManager().GetProximityForNavigation()))
                return true;
        }
    }
    return false;
}
static int fails;
static void check(bool c, const char* what) { if (!c) { ++fails; std::printf("FAIL %s\n", what); } }
int main()
{
    CThingAICreature me; me.Pos.X = 1; me.Pos.Y = 2; me.Pos.Z = 3; me.Combat.ProximityForNavigation = 4.5f;
    CThingCreatureBase enemy; enemy.Dead = false; enemy.Pos.X = 7; enemy.Pos.Y = 8; enemy.Pos.Z = 9;
    CAIStateGroup_MeleeCombatNearbyBase g; g.Creature = &me; g.PEnemy = 0; g.StillRunnable = true;
    me.Targeting.Best = 0; g_navCalls = 0;
    check(!g.IsRunnable() && g.PEnemy == 0 && g_navCalls == 0, "no enemy");
    enemy.Dead = true; me.Targeting.Best = &enemy;
    check(!g.IsRunnable() && g.PEnemy == 0 && g_navCalls == 0, "dead enemy");
    enemy.Dead = false; g.StillRunnable = false;
    check(!g.IsRunnable() && g.PEnemy == &enemy && g_navCalls == 0, "alive, not still runnable");
    g.PEnemy = 0; g.StillRunnable = true; g_navigable = false;
    check(!g.IsRunnable() && g.PEnemy == &enemy && g_navCalls == 1, "not navigable");
    check(g_from.X == 1 && g_from.Z == 3 && g_to.Y == 8 && g_prox == 4.5f, "navigation arguments");
    g_navigable = true;
    check(g.IsRunnable() && g_navCalls == 2, "navigable");
    if (fails == 0) std::printf("0093d270_TEST PASS\n");
    return fails;
}