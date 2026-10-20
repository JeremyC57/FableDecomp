// Behaviour test for CQ_NewOakValeIntroScript::AttackStuff (0x00DBE3C0): the six interface calls happen in
// order with the right quest/theme names, time and objective arguments, and every string is released.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
};
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGameScriptInterface {
    float hour, themeTime; unsigned long delay;
    virtual void ActivateQuest(const CCharString& q) { Log("act"); Log(q.PStringData); }
    virtual void DeactivateQuest(const CCharString& q, unsigned long d) { Log("deact"); Log(q.PStringData); delay = d; }
    virtual void SetQuestCardObjective(const CCharString& q, const CCharString& t, const CCharString& r, const CCharString& e)
    { Log("obj"); Log(q.PStringData); Log(t.PStringData); Log(r.PStringData); Log(e.PStringData); }
    virtual void SetTimeOfDay(float h) { Log("time"); hour = h; }
    virtual CCharString GetActiveQuestName() { Log("name"); return CCharString("Q_Active"); }
    virtual void TransitionToTheme(const CCharString& t, float tm) { Log("theme"); Log(t.PStringData); themeTime = tm; }
};
struct CQ_NewOakValeIntroScript {
    CGameScriptInterface* m_pInterface;
    void AttackStuff();
};
void CQ_NewOakValeIntroScript::AttackStuff()
{
    m_pInterface->ActivateQuest("Q__OakValeIntro_PostAttack");
    m_pInterface->DeactivateQuest("Q_NewOakValeIntro_PreAttack", 0);
    m_pInterface->SetTimeOfDay(23.0f);
    m_pInterface->TransitionToTheme("ENVIRONMENT_OV_POSTATTACK", 0.0f);
    {
        CCharString extra("");
        CCharString region("");
        CCharString text("TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06");
        m_pInterface->SetQuestCardObjective(m_pInterface->GetActiveQuestName(), text, region, extra);
    }
}
int main()
{
    CGameScriptInterface gi; gi.hour = 0; gi.themeTime = 5; gi.delay = 7;
    CQ_NewOakValeIntroScript s; s.m_pInterface = &gi;
    g_log[0] = 0;
    s.AttackStuff();
    const char* want = "act;Q__OakValeIntro_PostAttack;deact;Q_NewOakValeIntro_PreAttack;time;theme;ENVIRONMENT_OV_POSTATTACK;"
                       "name;obj;Q_Active;TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06;;;";
    float h = 23.0f;
    if (std::strcmp(g_log, want) != 0 || gi.hour != h || gi.themeTime != 0.0f || gi.delay != 0 || g_live != 0) {
        std::printf("00dbe3c0_TEST FAIL\n"); return 1;
    }
    std::printf("00dbe3c0_TEST PASS\n");
    return 0;
}