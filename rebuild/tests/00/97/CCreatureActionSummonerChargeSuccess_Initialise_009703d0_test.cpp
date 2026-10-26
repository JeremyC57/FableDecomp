#include <cstdio>
#include <cstring>
// copy of the model: the scoped CCharString name handed to SetCombatAnimation, then the base Initialise
static int g_order[8];
static int g_n = 0;
static char g_anim[32];
class CCharString {
public:
    const char* m_text;
    CCharString(const char* text, long length) : m_text(text) { g_order[g_n++] = 1; }
    ~CCharString() { g_order[g_n++] = 3; }
};
class CCombatActionBase { public:
    virtual void v00() { }
    virtual void Initialise() { g_order[g_n++] = 4; }
    unsigned char _body[0x114 - 4];
    void SetCombatAnimation(const CCharString& name) { std::strcpy(g_anim, name.m_text); g_order[g_n++] = 2; }
};
struct CCreatureAction_SummonerChargeSuccess : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_SummonerChargeSuccess::Initialise()
{
    {
        CCharString name("CHARGE_SUCCESS", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}

int main()
{
    CCreatureAction_SummonerChargeSuccess a;
    a.Initialise();
    if (g_n != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4 ||
        std::strcmp(g_anim, "CHARGE_SUCCESS") != 0) {
        std::printf("009703d0_TEST FAIL\n");
        return 1;
    }
    std::printf("009703d0_TEST PASS\n");
    return 0;
}