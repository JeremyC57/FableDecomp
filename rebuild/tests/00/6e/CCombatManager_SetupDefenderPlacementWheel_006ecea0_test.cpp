// Behaviour test for CCombatManager::SetupDefenderPlacementWheel (0x006ECEA0): the row body verbatim over models of its callees.
// Kills: the frame or position not stamped (or stamped when not forced), the wheel rebuilt when not forced, the UsingCombatWheel
// filter dropped or inverted, the wrong map key (the attacker's id instead of the creature) or value (not that creature's
// AttackerInfo), the lookup done on the defender instead of each attacker, the wrong world map or defender passed to Init, and
// the wheel not updated on either path.
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <utility>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
#ifndef _MSC_VER
#define __int64 long long
#endif
typedef unsigned __int64 UID;
struct C3DVector { float X, Y, Z; };
static long world_frame;
struct CWorldMap { int Tag; };
class CWorld {
public:
    CWorldMap Map;
    static long GetFrame() { return world_frame; }
    CWorldMap& GetWorldMap() { return Map; }
};
struct CThingCreatureBase {
    UID UniqueID; C3DVector Pos;
    UID GetUniqueID() const { return UniqueID; }
    const C3DVector& GetPos() const { return Pos; }
};
class CCombatManager;
class CAttackingCombatantInfo { public: CThingCreatureBase* Attacked; bool UsingCombatWheel; };
class CDefendingCombatantInfo {
public:
    class CCombatWheel {
    public:
        int InitCalls, UpdateCalls;
        const CCombatManager* InitManager; const CWorldMap* InitMap; const CThingCreatureBase* InitDefender;
        std::map<CThingCreatureBase*, CAttackingCombatantInfo*> InitAttackers;
        void Init(CCombatManager& m, CWorldMap& wm, const std::map<CThingCreatureBase*, CAttackingCombatantInfo*>& a, const CThingCreatureBase& d)
        { ++InitCalls; InitManager = &m; InitMap = &wm; InitAttackers = a; InitDefender = &d; }
        void Update() { ++UpdateCalls; }
    };
    std::map<UID, CThingCreatureBase*> Attackers; CCombatWheel CombatWheel;
    C3DVector LastVictimWheelUpdatePos; long LastWheelUpdateWorldFrame;
};
struct CCombatantInfo { CThingCreatureBase* Parent; CAttackingCombatantInfo AttackerInfo; CDefendingCombatantInfo DefenderInfo; };
class CCombatManager {
public:
    std::map<UID, CCombatantInfo> Combatants; CWorld& World;
    CCombatManager(CWorld& w) : World(w) {}
    CWorld& GetWorld() const { return World; }
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) { return Combatants.find(creature.GetUniqueID())->second; }
    void SetupDefenderPlacementWheel(CCombatantInfo& combat_info, bool force_wheel_update);
};
// ---- row body (verbatim) ----
void CCombatManager::SetupDefenderPlacementWheel(CCombatantInfo& combat_info, bool force_wheel_update)
{
    CDefendingCombatantInfo& defender_info = combat_info.DefenderInfo;
    if (force_wheel_update) {
        defender_info.LastWheelUpdateWorldFrame = GetWorld().GetFrame();
        defender_info.LastVictimWheelUpdatePos = combat_info.Parent->GetPos();
        const std::map<unsigned __int64, CThingCreatureBase*>& attackers = defender_info.Attackers;
        std::map<CThingCreatureBase*, CAttackingCombatantInfo*> attackers_map;
        for (std::map<unsigned __int64, CThingCreatureBase*>::const_iterator it = attackers.begin(); it != attackers.end(); ++it) {
            CThingCreatureBase* creature = (*it).second;
            CCombatantInfo& att_comb_inf = GetCombatantInfo(*creature);
            if (att_comb_inf.AttackerInfo.UsingCombatWheel)
                attackers_map.insert(std::make_pair(creature, &att_comb_inf.AttackerInfo));
        }
        defender_info.CombatWheel.Init(*this, GetWorld().GetWorldMap(), attackers_map, *combat_info.Parent);
    }
    defender_info.CombatWheel.Update();
}
// ---- end row ----

static CCombatantInfo blank()
{
    CCombatantInfo c; c.Parent = 0; c.AttackerInfo.Attacked = 0; c.AttackerInfo.UsingCombatWheel = false;
    c.DefenderInfo.CombatWheel.InitCalls = 0; c.DefenderInfo.CombatWheel.UpdateCalls = 0;
    c.DefenderInfo.CombatWheel.InitManager = 0; c.DefenderInfo.CombatWheel.InitMap = 0; c.DefenderInfo.CombatWheel.InitDefender = 0;
    c.DefenderInfo.LastVictimWheelUpdatePos.X = c.DefenderInfo.LastVictimWheelUpdatePos.Y = c.DefenderInfo.LastVictimWheelUpdatePos.Z = -1.0f;
    c.DefenderInfo.LastWheelUpdateWorldFrame = -1;
    return c;
}

int main()
{
    CWorld world; world.Map.Tag = 5;
    CCombatManager mgr(world);
    CThingCreatureBase defender = { 1, { 3.0f, 4.0f, 5.0f } };
    CThingCreatureBase a = { (UID)0x10000000 * 16 + 2, { 0, 0, 0 } }, b = { 3, { 0, 0, 0 } }, c = { 4, { 0, 0, 0 } };
    mgr.Combatants[defender.UniqueID] = blank();
    mgr.Combatants[a.UniqueID] = blank(); mgr.Combatants[a.UniqueID].Parent = &a;
    mgr.Combatants[b.UniqueID] = blank(); mgr.Combatants[b.UniqueID].Parent = &b;
    mgr.Combatants[c.UniqueID] = blank(); mgr.Combatants[c.UniqueID].Parent = &c;
    mgr.Combatants[a.UniqueID].AttackerInfo.UsingCombatWheel = true;
    mgr.Combatants[b.UniqueID].AttackerInfo.UsingCombatWheel = false;
    mgr.Combatants[c.UniqueID].AttackerInfo.UsingCombatWheel = true;
    mgr.Combatants[defender.UniqueID].AttackerInfo.UsingCombatWheel = false;   // the defender's own flag must not matter
    CCombatantInfo& d = mgr.Combatants[defender.UniqueID];
    d.Parent = &defender;
    d.DefenderInfo.Attackers[a.UniqueID] = &a;
    d.DefenderInfo.Attackers[b.UniqueID] = &b;
    d.DefenderInfo.Attackers[c.UniqueID] = &c;
    long cases = 0;

    // Not forced: only the wheel update.
    world_frame = 77;
    mgr.SetupDefenderPlacementWheel(d, false);
    CHECK(d.DefenderInfo.CombatWheel.UpdateCalls == 1 && d.DefenderInfo.CombatWheel.InitCalls == 0); ++cases;
    CHECK(d.DefenderInfo.LastWheelUpdateWorldFrame == -1 && d.DefenderInfo.LastVictimWheelUpdatePos.X == -1.0f); ++cases;

    // Forced: stamp, collect the wheel-using attackers, rebuild, update.
    mgr.SetupDefenderPlacementWheel(d, true);
    const CDefendingCombatantInfo::CCombatWheel& w = d.DefenderInfo.CombatWheel;
    CHECK(w.UpdateCalls == 2 && w.InitCalls == 1); ++cases;
    CHECK(d.DefenderInfo.LastWheelUpdateWorldFrame == 77); ++cases;
    CHECK(d.DefenderInfo.LastVictimWheelUpdatePos.X == 3.0f && d.DefenderInfo.LastVictimWheelUpdatePos.Y == 4.0f &&
          d.DefenderInfo.LastVictimWheelUpdatePos.Z == 5.0f); ++cases;
    CHECK(w.InitManager == &mgr && w.InitMap == &world.Map && w.InitDefender == &defender); ++cases;
    CHECK(w.InitAttackers.size() == 2); ++cases;
    CHECK(w.InitAttackers.count(&a) == 1 && w.InitAttackers.find(&a)->second == &mgr.Combatants[a.UniqueID].AttackerInfo); ++cases;
    CHECK(w.InitAttackers.count(&c) == 1 && w.InitAttackers.find(&c)->second == &mgr.Combatants[c.UniqueID].AttackerInfo); ++cases;
    CHECK(w.InitAttackers.count(&b) == 0); ++cases;

    // Forced with no attackers: an empty map still rebuilds the wheel.
    d.DefenderInfo.Attackers.clear();
    world_frame = 78;
    mgr.SetupDefenderPlacementWheel(d, true);
    CHECK(w.InitCalls == 2 && w.UpdateCalls == 3 && w.InitAttackers.empty() && d.DefenderInfo.LastWheelUpdateWorldFrame == 78); ++cases;

    CHECK(cases == 11);
    puts("006ecea0_TEST PASS");
    return 0;
}