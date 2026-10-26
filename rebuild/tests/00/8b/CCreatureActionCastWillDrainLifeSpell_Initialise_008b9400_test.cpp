#include <cstdio>
#include <cstring>
static const char* g_anim; static int g_baseInit; static int g_order;
class CCharString { public: const char* m_text; CCharString(const char* t, long) : m_text(t) { } ~CCharString() { } };
class CCombatActionBase { public: virtual void v00() { } virtual void Initialise() { ++g_baseInit; g_order = g_order * 10 + 2; }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_text; g_order = g_order * 10 + 1; }
};

class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase { public: virtual void Initialise(); };
void CCreatureAction_CastWillDrainLifeSpell::Initialise() { { CCharString name("ST_CAST_DRAIN_LIFE", -1); SetAnimationName(name); } CCombatActionBase::Initialise(); }
int main()
{
    CCreatureAction_CastWillDrainLifeSpell a; a.Initialise();
    if (g_baseInit != 1 || g_order != 12 || std::strcmp(g_anim, "ST_CAST_DRAIN_LIFE") != 0) { std::printf("008b9400_TEST FAIL\n"); return 1; }
    std::printf("008b9400_TEST PASS\n");
    return 0;
}