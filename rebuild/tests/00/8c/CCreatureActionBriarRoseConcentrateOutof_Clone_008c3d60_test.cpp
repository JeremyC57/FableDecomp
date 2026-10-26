#include <cstdio>
#include <cstring>
static const void* g_copied;
class CCombatActionBase { public: virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase() { std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { g_copied = &o; std::memcpy(_body, o._body, sizeof(_body)); } };
class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase { public: long m_level; long m_defIndex; virtual void v00() {} };
class CCreatureAction_BriarRoseConcentrateOutof : public CCreatureAction_CastWillDrainLifeSpell { public: virtual void v00() {} CCombatActionBase* Clone(); };
CCombatActionBase* CCreatureAction_BriarRoseConcentrateOutof::Clone() { return new CCreatureAction_BriarRoseConcentrateOutof(*this); }
int main()
{
    CCreatureAction_BriarRoseConcentrateOutof a; a.m_level = 3; a.m_defIndex = 1234; a._body[2] = 5;
    CCreatureAction_BriarRoseConcentrateOutof* c = (CCreatureAction_BriarRoseConcentrateOutof*)a.Clone();
    if (c == 0 || c == &a || g_copied != &a || c->m_level != 3 || c->m_defIndex != 1234 || c->_body[2] != 5 || sizeof(*c) != 0x11c) { std::printf("008c3d60_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("008c3d60_TEST PASS\n");
    return 0;
}