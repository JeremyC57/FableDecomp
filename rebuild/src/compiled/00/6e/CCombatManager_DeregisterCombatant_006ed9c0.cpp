class CThingCreatureBase { public:
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

class CCombatManager { public:
    void RemoveCombatant_006ed510(CThingCreatureBase* c);
    void DeregisterCombatant(CThingCreatureBase* c);
};

void CCombatManager::DeregisterCombatant(CThingCreatureBase* c)
{
    if (c->IsInMeleeAsAttacker())
        this->RemoveCombatant_006ed510(c);
}