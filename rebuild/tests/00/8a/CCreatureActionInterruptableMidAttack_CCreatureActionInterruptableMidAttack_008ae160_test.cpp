// behaviour test for 0x008AE160 CCreatureAction_InterruptableMidAttack::CCreatureAction_InterruptableMidAttack(CThingCreatureBase&, CThing&) (model; the base records its arguments)
#include <cstdio>
class CThing { public: int id; };
class CThingCreatureBase : public CThing { };
class CSoundPair { public: CSoundPair() : FirstSound(0), LastSound(0) { } long FirstSound; long LastSound; };
static const void* g_owner; static const void* g_target; static long g_code = -1, g_s0 = -1, g_s1 = -1;
class CCombatActionBase {
public:
    CCombatActionBase(CThingCreatureBase& owner, CThing& target, long actionCode, const CSoundPair& sound)
    { g_owner = &owner; g_target = &target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; }
    virtual ~CCombatActionBase() { }
};
class CCreatureAction_InterruptableMidAttack : public CCombatActionBase { public: CCreatureAction_InterruptableMidAttack(CThingCreatureBase& owner, CThing& target); };
CCreatureAction_InterruptableMidAttack::CCreatureAction_InterruptableMidAttack(CThingCreatureBase& owner, CThing& target) : CCombatActionBase(owner, target, 150, CSoundPair()) { }
int main()
{
    CThingCreatureBase owner; CThing target;
    CCreatureAction_InterruptableMidAttack action(owner, target);
    int ok = g_owner == &owner && g_target == &target && g_code == 150 && g_s0 == 0 && g_s1 == 0;
    if (!ok) { std::printf("008ae160_TEST FAIL\n"); return 1; }
    std::printf("008ae160_TEST PASS\n");
    return 0;
}