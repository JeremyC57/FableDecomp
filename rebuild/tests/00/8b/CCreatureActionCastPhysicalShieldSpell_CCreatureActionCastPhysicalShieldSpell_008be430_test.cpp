#include <cstdio>
#include <cstring>
static int g_baseCopies;
class CCombatActionBase { public: virtual void v00() { } unsigned char _body[0x114 - 4];
    CCombatActionBase() { std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { ++g_baseCopies; std::memcpy(_body, o._body, sizeof(_body)); } };
class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase { public:
    unsigned char ImmediateCast, OneAnimCast; long LevelToCast, TimeLimit, ShieldHealth;
    virtual void v00() { }
    CCreatureAction_CastPhysicalShieldSpell() { }
    CCreatureAction_CastPhysicalShieldSpell(const CCreatureAction_CastPhysicalShieldSpell& o)
        : CCombatActionBase(o), ImmediateCast(o.ImmediateCast), OneAnimCast(o.OneAnimCast), LevelToCast(o.LevelToCast), TimeLimit(o.TimeLimit), ShieldHealth(o.ShieldHealth) { } };
int main()
{
    CCreatureAction_CastPhysicalShieldSpell a; a.ImmediateCast = 1; a.OneAnimCast = 2; a.LevelToCast = 3; a.TimeLimit = 4; a.ShieldHealth = 5; a._body[5] = 9;
    CCreatureAction_CastPhysicalShieldSpell b(a);
    if (g_baseCopies != 1 || b.ImmediateCast != 1 || b.OneAnimCast != 2 || b.LevelToCast != 3 || b.TimeLimit != 4 || b.ShieldHealth != 5 || b._body[5] != 9
        || sizeof(b) != 0x124 || (char*)&b.ShieldHealth - (char*)&b != 0x120) { std::printf("008be430_TEST FAIL\n"); return 1; }
    std::printf("008be430_TEST PASS\n");
    return 0;
}