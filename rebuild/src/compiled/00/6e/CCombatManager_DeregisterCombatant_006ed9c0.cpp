// CCombatManager::DeregisterCombatant(CThingCreatureBase&) -- if the creature is an attacking combatant, take it off the attackers
// Leaf: ?DeregisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z
// NF122 (2026-11-20, q4266): Ego_r.pdb 0x007173E0 / FableWin.pdb 0x02412E40 spelling (by reference; engine/CCombatManager.h declares
//   the same); respelled in place on the row's local models (was a CThingCreatureBase* parameter). The callee 0x006ED510 is
//   Ego_r ?DeregisterAttackingCombatant@CCombatManager@@AAEXAAVCThingCreatureBase@@@Z (private; NAME_FIXES): its stand-in spelling here
//   is left as it was, since 0x006ED9E0 binds a third spelling and the two must move together.
class CThingCreatureBase { public:
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

class CCombatManager { public:
    private: void DeregisterAttackingCombatant(CThingCreatureBase& attacker); public:   // 0x006ED510 (Ego_r.pdb + FableWin.pdb, private)
    void DeregisterCombatant(CThingCreatureBase& c);
};

void CCombatManager::DeregisterCombatant(CThingCreatureBase& c)
{
    if (c.IsInMeleeAsAttacker())
        this->DeregisterAttackingCombatant(c);
}