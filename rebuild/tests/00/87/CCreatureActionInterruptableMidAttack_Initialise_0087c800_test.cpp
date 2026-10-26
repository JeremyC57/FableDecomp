#include <cstdio>
#include <cstring>
static const char* g_anim; static int g_baseInit; static int g_order;
class CCharString { public: const char* m_text; CCharString(const char* t, long) : m_text(t) { } ~CCharString() { } };
class CCombatActionBase { public: virtual void v00() { } virtual void Initialise() { ++g_baseInit; g_order = g_order * 10 + 2; }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_text; g_order = g_order * 10 + 1; }
    unsigned char _body[0x114 - 4];
};

class CCreatureAction_InterruptableMidAttack : public CCombatActionBase { public: long InterruptTime; virtual void Initialise(); };
void CCreatureAction_InterruptableMidAttack::Initialise() { { CCharString name("STRIKE_MEDIUM_FRONT", -1); SetAnimationName(name); } CCombatActionBase::Initialise(); InterruptTime = -999; }
int main()
{
    CCreatureAction_InterruptableMidAttack a; a.InterruptTime = 5; a.Initialise();
    if (g_baseInit != 1 || g_order != 12 || std::strcmp(g_anim, "STRIKE_MEDIUM_FRONT") != 0 || a.InterruptTime != -999 || (char*)&a.InterruptTime - (char*)&a != 0x114) { std::printf("0087c800_TEST FAIL\n"); return 1; }
    std::printf("0087c800_TEST PASS\n");
    return 0;
}