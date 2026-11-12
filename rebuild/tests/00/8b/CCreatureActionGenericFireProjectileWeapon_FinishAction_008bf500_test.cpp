// Behaviour test for 0x008BF500 CCreatureAction_GenericFireProjectileWeapon::FinishAction(): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("008bf500_TEST FAIL line %d\n", line); exit(1); } }
class CTCCarrying;
class CTCWeapon { public:
    bool Projectile; int Readied;
    bool IsProjectileWeapon() const { return Projectile; }
    void ReadyProjectile() { ++Readied; trace += 'R'; } };
class CThing { public:
    bool Dead; int Killed; CTCWeapon* Weapon; CTCCarrying* Carrying;
    CThing() : Dead(false), Killed(0), Weapon(0), Carrying(0) { }
    void Kill(bool now) { check(now, __LINE__); ++Killed; trace += 'K'; }
    bool IsDead() const { return Dead; }
    bool GetTC(CTCWeapon** w) { if (!Weapon) return false; *w = Weapon; return true; }
    bool GetTC(CTCCarrying** c) { if (!Carrying) return false; *c = Carrying; return true; }
};
class CTCCarrying { public: CThing* Primary; CThing* GetThingInPrimarySlot() const { return Primary; } };
#define IS_THING_ALIVE(p) ((p) != 0 && !(p)->IsDead())
class CThingCreatureBase : public CThing { };
class CAttackingCombatantInfo { public: CThingCreatureBase* Attacked; };
class CDefendingCombatantInfo { public: std::map<unsigned long, CThingCreatureBase*> Attackers; };
struct CCombatantInfo { CAttackingCombatantInfo AttackerInfo; CDefendingCombatantInfo DefenderInfo; };
class CCombatManager { public:
    std::map<const CThingCreatureBase*, CCombatantInfo> Infos; bool Attacking;
    bool IsCreatureAttacking(const CThingCreatureBase&) const { return Attacking; }
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& c) { return Infos[&c]; } };
static CCombatManager g_manager;
class CThingRef { public: CThing* Item; CThing* operator->() const { return Item; } bool operator!=(int) const { return Item != 0; } };
class CCreatureAction_GenericFireProjectileWeapon { public:
    CThingCreatureBase* PCreature; bool Performed; std::vector<CThingRef> Dependents;
    bool HasPerformedAction() const { return Performed; }
    void PerformAction() { Performed = true; trace += 'P'; }
    const std::vector<CThingRef>& GetDependentThings() const { return Dependents; }
    void BaseFinishAction() { trace += 'F'; }
    void FinishAction();
};
static CCombatManager& GetCombatManager(CThingCreatureBase&) { return g_manager; }
// the row's logic (src/008bf500.cpp)
void CCreatureAction_GenericFireProjectileWeapon::FinishAction()
{
    if (!HasPerformedAction())
        PerformAction();
    for (unsigned long i = 0; i < GetDependentThings().size(); ++i) {
        if (GetDependentThings()[i] != 0)
            GetDependentThings()[i]->Kill(true);
    }
    CCombatManager& combat_manager = GetCombatManager(*PCreature);
    if (combat_manager.IsCreatureAttacking(*PCreature)) {
        CThingCreatureBase* target = combat_manager.GetCombatantInfo(*PCreature).AttackerInfo.Attacked;
        const CCombatantInfo& target_info = combat_manager.GetCombatantInfo(*target);
        for (std::map<unsigned long, CThingCreatureBase*>::const_iterator it = target_info.DefenderInfo.Attackers.begin();
             it != target_info.DefenderInfo.Attackers.end(); ++it) {
            CTCCarrying* carrying;
            if (it->second->GetTC(&carrying)) {
                CThing* item = carrying->GetThingInPrimarySlot();
                CTCWeapon* weapon;
                if (IS_THING_ALIVE(item) && item->GetTC(&weapon) && weapon->IsProjectileWeapon())
                    weapon->ReadyProjectile();
            }
        }
    }
    BaseFinishAction();
}
int main()
{
    CThingCreatureBase self, target, a1, a2, a3, a4;
    CThing dep, bow_item, sword_item, dead_item;
    CTCWeapon bow, sword, dead_bow;
    bow.Projectile = true; bow.Readied = 0; sword.Projectile = false; sword.Readied = 0; dead_bow.Projectile = true; dead_bow.Readied = 0;
    bow_item.Weapon = &bow; sword_item.Weapon = &sword; dead_item.Weapon = &dead_bow; dead_item.Dead = true;
    CTCCarrying c1, c2, c3; c1.Primary = &bow_item; c2.Primary = &sword_item; c3.Primary = &dead_item;
    a1.Carrying = &c1; a2.Carrying = &c2; a3.Carrying = &c3;   // a4 carries nothing
    g_manager.Infos[&self].AttackerInfo.Attacked = &target;
    std::map<unsigned long, CThingCreatureBase*>& at = g_manager.Infos[&target].DefenderInfo.Attackers;
    at[1] = &a1; at[2] = &a2; at[3] = &a3; at[4] = &a4;
    CCreatureAction_GenericFireProjectileWeapon act;
    act.PCreature = &self; act.Performed = false;
    CThingRef r0; r0.Item = 0; CThingRef r1; r1.Item = &dep;
    act.Dependents.push_back(r0); act.Dependents.push_back(r1);
    g_manager.Attacking = true;
    act.FinishAction();
    check(trace == "PKRF", __LINE__);
    check(dep.Killed == 1 && bow.Readied == 1 && sword.Readied == 0 && dead_bow.Readied == 0, __LINE__);
    // already performed, not attacking: no fire, no ready
    trace = ""; g_manager.Attacking = false;
    act.FinishAction();
    check(trace == "KF", __LINE__);
    check(bow.Readied == 1, __LINE__);
    printf("008bf500_TEST PASS\n");
    return 0;
}