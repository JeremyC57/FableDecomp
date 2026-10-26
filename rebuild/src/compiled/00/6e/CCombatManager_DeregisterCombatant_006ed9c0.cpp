class CThingCreatureBase { public:
    bool IsCombatant_00662410();
};

class CCombatManager { public:
    void RemoveCombatant_006ed510(CThingCreatureBase* c);
    void DeregisterCombatant(CThingCreatureBase* c);
};

void CCombatManager::DeregisterCombatant(CThingCreatureBase* c)
{
    if (c->IsCombatant_00662410())
        this->RemoveCombatant_006ed510(c);
}