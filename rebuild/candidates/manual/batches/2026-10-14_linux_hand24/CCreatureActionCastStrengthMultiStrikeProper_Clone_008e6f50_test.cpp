#include <cstdio>
#include <cstring>
struct CCreatureActionBase;
static int g_baseCopies;
struct CCombatActionBase { virtual void v00() { } unsigned char _body[0x114 - 4];
    CCombatActionBase() { std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { ++g_baseCopies; std::memcpy(_body, o._body, sizeof(_body)); } };
struct CCreatureAction_CastStrengthMultiStrikeProper : CCombatActionBase {
    long SpellLevel;
    virtual void v00() { }
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_CastStrengthMultiStrikeProper::Clone() const { return (CCreatureActionBase*)new CCreatureAction_CastStrengthMultiStrikeProper(*this); }
int main()
{
    CCreatureAction_CastStrengthMultiStrikeProper a; a._body[3] = 42; a.SpellLevel = 7;
    CCreatureAction_CastStrengthMultiStrikeProper* b = (CCreatureAction_CastStrengthMultiStrikeProper*)a.Clone();
    if (b == 0 || b == &a || g_baseCopies != 1 || b->_body[3] != 42 || sizeof(a) != 0x118 || !(b->SpellLevel == 7)) { std::printf("008e6f50_TEST FAIL\n"); return 1; }
    delete b;
    std::printf("008e6f50_TEST PASS\n");
    return 0;
}
