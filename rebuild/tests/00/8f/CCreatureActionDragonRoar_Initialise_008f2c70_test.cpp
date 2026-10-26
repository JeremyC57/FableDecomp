#include <cstdio>
#include <cstring>
static const char* g_anim; static int g_baseInit; static int g_order;
class CCharString { public: const char* m_text; CCharString(const char* t, long) : m_text(t) { } ~CCharString() { } };
class CCombatActionBase { public: virtual void v00() { } virtual void Initialise() { ++g_baseInit; g_order = g_order * 10 + 2; }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_text; g_order = g_order * 10 + 1; }
    unsigned char _pad04[0x0c]; long TotalTime; long ActionTime;
};

class CCreatureAction_DragonRoar : public CCombatActionBase { public: virtual void Initialise(); };
void CCreatureAction_DragonRoar::Initialise() { { CCharString name("ROAR", -1); SetAnimationName(name); } CCombatActionBase::Initialise(); ActionTime = (long)(TotalTime * 0.3f); }
int main()
{
    CCreatureAction_DragonRoar a; a.TotalTime = 100; a.ActionTime = -1; a.Initialise();
    if (g_baseInit != 1 || g_order != 12 || std::strcmp(g_anim, "ROAR") != 0 || a.ActionTime != 30 || (char*)&a.ActionTime - (char*)&a != 0x14) { std::printf("008f2c70_TEST FAIL\n"); return 1; }
    std::printf("008f2c70_TEST PASS\n");
    return 0;
}