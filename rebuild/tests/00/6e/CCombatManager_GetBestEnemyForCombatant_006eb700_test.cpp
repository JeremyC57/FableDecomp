// Execute the selected combatant search body against deterministic engine doubles.
#include <assert.h>
#include <stdio.h>
#include <map>

struct C3DVector { float X, Y, Z; };
struct CTCTargeted {
    bool IsTargetable() const { return Targetable; }
    bool Targetable;
};
class CThing {
public:
    CThing() : HasComponent(true), Attackable(true) { Target.Targetable = true; }
    bool PeekTC(const CTCTargeted** out) const {
        if (!HasComponent)
            return false;
        *out = &Target;
        return true;
    }
    const C3DVector& GetPos() const { return Position; }
    C3DVector Position;
    CTCTargeted Target;
    bool HasComponent;
    bool Attackable;
};
struct CCombatTypeDef { float FrontFarLimit; };
class CThingCreatureBase : public CThing {
public:
    bool IsInMeleeAsAttacked() const { return UnderAttack; }
    const CCombatTypeDef& GetCombatTypeDef() const { return Def; }
    bool UnderAttack;
    CCombatTypeDef Def;
};
struct CDefendingCombatantInfo {
    std::map<unsigned __int64,CThingCreatureBase*> Attackers;
};
struct CCombatantInfo { CDefendingCombatantInfo DefenderInfo; };

class CCombatManager {
public:
    CCombatManager() : Override(false), Selected(0), Lookups(0), OverrideChecks(0) {}
    CThingCreatureBase* GetBestEnemyForCombatant(const CThingCreatureBase&, const C3DVector&) const;
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase&) const {
        ++Lookups;
        return Info;
    }
    bool IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase&, const CThing& target,
                                                       const C3DVector&) const {
        Selected = &target;
        ++OverrideChecks;
        return Override;
    }
    CCombatantInfo Info;
    bool Override;
    mutable const CThing* Selected;
    mutable int Lookups;
    mutable int OverrideChecks;
};

namespace NCreatureMode {
bool IsAbleToBeAttacked(const CThing& thing) { return thing.Attackable; }
}
namespace NBBBMaths {
bool IsPosInFOVWithWeight(const C3DVector&, const C3DVector& direction, float fov,
                         const C3DVector& attacker_pos, float& weight) {
    assert(fov == 1.0f && direction.X == 1.0f);
    weight = attacker_pos.X;
    return attacker_pos.Y > 0.0f;
}
bool IsInRange(const C3DVector&, const C3DVector& attacker_pos, float range) {
    assert(range == 15.0f);
    return attacker_pos.Z < range;
}
}

CThingCreatureBase* CCombatManager::GetBestEnemyForCombatant(const CThingCreatureBase& combatant, const C3DVector& attempted_facing_direction) const
{
    if (!combatant.IsInMeleeAsAttacked())
        return 0;

    float test_fov = 1.0f;
    const CCombatantInfo& combatant_info = GetCombatantInfo(combatant);
    const CCombatTypeDef& combat_def = combatant.GetCombatTypeDef();
    const CThingCreatureBase* best_enemy = 0;
    const std::map<unsigned __int64,CThingCreatureBase*>& attackers = combatant_info.DefenderInfo.Attackers;
    float best_weight = 0.0f;
    float closest_distance = 1.0e33f;
    for (std::map<unsigned __int64,CThingCreatureBase*>::const_iterator attackerit = attackers.begin(); attackerit != attackers.end(); ++attackerit) {
        const CThingCreatureBase& attacker = *attackerit->second;
        const CTCTargeted* ptc_targeted;
        if (attacker.PeekTC(&ptc_targeted) && ptc_targeted->IsTargetable() && NCreatureMode::IsAbleToBeAttacked(attacker)) {
            const C3DVector& attacker_pos = attacker.GetPos();
            float fov_weight;
            if (NBBBMaths::IsPosInFOVWithWeight(combatant.GetPos(), attempted_facing_direction, test_fov, attacker_pos, fov_weight) &&
                NBBBMaths::IsInRange(combatant.GetPos(), attacker.GetPos(), combat_def.FrontFarLimit) &&
                fov_weight > best_weight) {
                best_weight = fov_weight;
                best_enemy = &attacker;
            }
        }
    }
    if (best_enemy && !IsTargetOverriddenByAttemptedFacingDirection(combatant, *best_enemy, attempted_facing_direction)) {
        CThingCreatureBase* cast_of_greatness = const_cast<CThingCreatureBase*>(best_enemy);
        return cast_of_greatness;
    }
    return 0;
}

int main()
{
    CThingCreatureBase combatant;
    combatant.Position.X = combatant.Position.Y = combatant.Position.Z = 0.0f;
    combatant.Def.FrontFarLimit = 15.0f;
    combatant.UnderAttack = false;
    C3DVector direction = {1.0f, 0.0f, 0.0f};
    CCombatManager manager;
    assert(manager.GetBestEnemyForCombatant(combatant, direction) == 0);
    assert(manager.Lookups == 0 && manager.OverrideChecks == 0);

    combatant.UnderAttack = true;
    assert(manager.GetBestEnemyForCombatant(combatant, direction) == 0);
    assert(manager.Lookups == 1 && manager.OverrideChecks == 0);

    CThingCreatureBase attackers[9];
    for (int i = 0; i != 9; ++i) {
        attackers[i].Position.X = 10.0f;
        attackers[i].Position.Y = 1.0f;
        attackers[i].Position.Z = 2.0f;
        manager.Info.DefenderInfo.Attackers[i] = &attackers[i];
    }
    attackers[0].HasComponent = false;
    attackers[1].Target.Targetable = false;
    attackers[2].Attackable = false;
    attackers[3].Position.Y = 0.0f;
    attackers[4].Position.Z = 20.0f;
    attackers[5].Position.X = 0.0f;
    attackers[6].Position.X = 3.0f;
    attackers[7].Position.X = 4.0f;
    attackers[8].Position.X = 4.0f;
    assert(manager.GetBestEnemyForCombatant(combatant, direction) == &attackers[7]);
    assert(manager.Lookups == 2 && manager.OverrideChecks == 1 && manager.Selected == &attackers[7]);

    manager.Override = true;
    assert(manager.GetBestEnemyForCombatant(combatant, direction) == 0);
    assert(manager.Lookups == 3 && manager.OverrideChecks == 2 && manager.Selected == &attackers[7]);
    puts("006eb700_TEST PASS");
    return 0;
}