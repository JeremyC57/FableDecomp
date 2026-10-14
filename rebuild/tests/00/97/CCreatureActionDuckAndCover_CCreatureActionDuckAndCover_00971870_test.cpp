#include <cstdio>

static int g_baseCopies = 0;
struct CCombatActionBase {
    virtual void v00() { }
    unsigned char _body[0x110];
    CCombatActionBase() { }
    CCombatActionBase(const CCombatActionBase& other) { ++g_baseCopies; }
};
struct CCreatureAction_DuckAndCover : CCombatActionBase {
    long m_coverState, m_coverTimer, m_coverTarget;
    virtual void v00() { }
    CCreatureAction_DuckAndCover() : m_coverState(1), m_coverTimer(2), m_coverTarget(3) { }
    CCreatureAction_DuckAndCover(const CCreatureAction_DuckAndCover& o) : CCombatActionBase(o), m_coverState(o.m_coverState), m_coverTimer(o.m_coverTimer), m_coverTarget(o.m_coverTarget) { }
};
int main()
{
    CCreatureAction_DuckAndCover a; CCreatureAction_DuckAndCover b(a);
    if (g_baseCopies != 1 || b.m_coverState != 1 || b.m_coverTimer != 2 || b.m_coverTarget != 3) { std::printf("00971870_TEST FAIL\n"); return 1; }
    std::printf("00971870_TEST PASS\n"); return 0;
}