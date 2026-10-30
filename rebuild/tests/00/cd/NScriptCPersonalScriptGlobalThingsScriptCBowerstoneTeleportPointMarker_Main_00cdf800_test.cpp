// behaviour test for 0x00CDF800 NScript::CPersonalScript_GlobalThingsScript::CBowerstoneTeleportPointMarker::Main (copy of the logic over a scripted stub game)
#include <cstdio>
#include <cstring>
class CCharString { public:
    char m_text[64];
    CCharString(const char* text, long length = -1) { std::strncpy(m_text, text, 63); m_text[63] = 0; (void)length; }
    ~CCharString() {}
};
class CScriptThing { public: float m_x; };
enum ETutorialCategory { TUTORIAL_CATEGORY_TELEPORTING = 31 };
static int g_frames = 0, g_terminateAfter = 1000, g_tutorialCategory = -1, g_clickAfter = 0, g_heroCloseAfter = 0;
static bool g_tutorialShown = true;
static char g_deactivated[64];
class CGameScriptInterface { public:
    CScriptThing m_hero;
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing& GetHero() { m_hero.m_x = (g_frames >= g_heroCloseAfter) ? 1.0f : 100.0f; return m_hero; }
    bool DisplayTutorial(ETutorialCategory c) const { g_tutorialCategory = c; return g_tutorialShown; }
    bool MsgIsTutorialClickedPast() const { return g_frames >= g_clickAfter; }
    void DeactivateQuestLater(const CCharString& name, unsigned long) const { std::strcpy(g_deactivated, name.m_text); }
};
class CScriptDef { public: long TeleporterActivationDist; };
static CScriptDef g_def = { 5 };
static CScriptDef* pGlobalScriptDef = &g_def;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{
    float d = a.m_x - b.m_x; if (d < 0) d = -d; return d < distance;
}
class CEntityScriptBase { public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frames >= g_terminateAfter; }
};
class CBowerstoneTeleportPointMarker : public CEntityScriptBase { public: void Main(); };
void CBowerstoneTeleportPointMarker::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->TeleporterActivationDist)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->DisplayTutorial(TUTORIAL_CATEGORY_TELEPORTING)) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgIsTutorialClickedPast()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    CCharString questName("Hook_BowerstoneTeleportTutorial");
    Game->DeactivateQuestLater(questName, 0);
}
int main()
{
    CGameScriptInterface game; CBowerstoneTeleportPointMarker m; m.Game = &game; m.Me.m_x = 0.0f;
    g_heroCloseAfter = 3; g_clickAfter = 7; g_deactivated[0] = 0;
    m.Main();                                            // hero arrives after 3 frames, tutorial clicked after 7
    if (g_frames != 7 || g_tutorialCategory != 31 || std::strcmp(g_deactivated, "Hook_BowerstoneTeleportTutorial"))
    { std::printf("00cdf800_TEST FAIL\n"); return 1; }
    g_frames = 0; g_deactivated[0] = 0; g_terminateAfter = 2;  // the thread terminates while waiting for the hero
    m.Main();
    if (g_frames != 2 || g_deactivated[0] != 0) { std::printf("00cdf800_TEST FAIL\n"); return 1; }
    g_frames = 0; g_terminateAfter = 1000; g_heroCloseAfter = 0; g_tutorialShown = false;   // tutorial not shown: deactivate at once
    m.Main();
    if (g_frames != 0 || std::strcmp(g_deactivated, "Hook_BowerstoneTeleportTutorial")) { std::printf("00cdf800_TEST FAIL\n"); return 1; }
    std::printf("00cdf800_TEST PASS\n");
    return 0;
}