// Behaviour test for 0x006DD8F0 CCreatureAction_UnsheatheToShowToSheatheTrophy::FinishAction: base finish first, then the
// creature receives a show-to-sheathe action (code 1001) carrying the creature, the trophy and the animation name; the
// temporary is destroyed afterwards.
#include <cstdio>
static int g_baseFinish = 0, g_set = 0, g_dtor = 0, g_code = 0;
static void* g_creatureSeen = 0; static void* g_trophySeen = 0; static const char* g_animSeen = 0;
struct CThing;
struct CAction {
    CThing* m_creature; CThing* m_trophy; const char* m_anim; int m_code;
    CAction(CThing* c, CThing* t, const char* a, int code) : m_creature(c), m_trophy(t), m_anim(a), m_code(code) {}
    ~CAction() { ++g_dtor; }
};
struct CShowToSheathe : CAction { CShowToSheathe(CThing* c, CThing* t, const char* a) : CAction(c, t, a, 1001) {} };
struct CThing {
    bool SetCurrentAction(const CAction& a)
    {
        ++g_set; g_code = a.m_code; g_creatureSeen = a.m_creature; g_trophySeen = a.m_trophy; g_animSeen = a.m_anim;
        return g_dtor == 0;   // the temporary is still alive during the call
    }
};
struct CUnsheatheToShow {
    CThing* m_creature; CThing* m_trophy; const char* ShowAnimName;
    CThing* GetCreature() { return m_creature; }
    void BaseFinish() { ++g_baseFinish; }
    void FinishAction()
    {
        BaseFinish();
        CThing* creature = GetCreature();
        GetCreature()->SetCurrentAction(CShowToSheathe(creature, m_trophy, ShowAnimName));
    }
};
int main()
{
    CThing creature, trophy;
    CUnsheatheToShow a; a.m_creature = &creature; a.m_trophy = &trophy; a.ShowAnimName = "ShowTrophy";
    a.FinishAction();
    if (g_baseFinish != 1 || g_set != 1 || g_code != 1001 || g_creatureSeen != &creature || g_trophySeen != &trophy
        || g_animSeen != a.ShowAnimName || g_dtor != 1) { std::printf("006dd8f0_TEST FAIL\n"); return 1; }
    std::printf("006dd8f0_TEST PASS\n");
    return 0;
}