// Behaviour test for 0x00865AC0: IsKnockdownToStun = combat def allows stun AND killable AND not player-controlled AND
// health below the stunned threshold.
#include <cstdio>

class CCombatTypeDef { public: bool KnockdownCausesStun; };

class CThingCreatureBase
{
public:
    float          Health;
    unsigned char  Damageable : 1;
    unsigned char  Killable : 1;
    CCombatTypeDef m_def;
    float          m_stunThreshold;
    bool           m_player;
    const CCombatTypeDef* GetCombatTypeDef() const { return &m_def; }
    float GetStunnedHealthThreshold() const { return m_stunThreshold; }
    virtual ~CThingCreatureBase() { }
    virtual bool IsPlayerControlled() const { return m_player; }
};

bool IsKnockdownToStun(const CThingCreatureBase& creature)
{
    if (creature.GetCombatTypeDef()->KnockdownCausesStun && creature.Killable && !creature.IsPlayerControlled())
    {
        float health = creature.Health;
        if (health < creature.GetStunnedHealthThreshold())
            return true;
    }
    return false;
}

int main()
{
    CThingCreatureBase c;
    c.Health = 10.0f; c.Damageable = 1; c.Killable = 1; c.m_def.KnockdownCausesStun = true;
    c.m_stunThreshold = 20.0f; c.m_player = false;
    int fails = 0;
    if (!IsKnockdownToStun(c)) ++fails;
    c.Health = 20.0f;                       if (IsKnockdownToStun(c)) ++fails;   // not below
    c.Health = 10.0f; c.m_player = true;    if (IsKnockdownToStun(c)) ++fails;
    c.m_player = false; c.Killable = 0;     if (IsKnockdownToStun(c)) ++fails;
    c.Killable = 1; c.m_def.KnockdownCausesStun = false; if (IsKnockdownToStun(c)) ++fails;
    if (fails) { std::printf("00865ac0_TEST FAIL\n"); return 1; }
    std::printf("00865ac0_TEST PASS\n");
    return 0;
}