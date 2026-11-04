#include <cstdio>
#include <cstring>
// behaviour test: CRockTroll::Main attaches to the quest, waits for the hero within 20 units, then adds a green health bar
class CCharString { public: const char* t; CCharString(const char* s, long) : t(s) {} };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CScriptThing { public: int id; };
static int g_frames = 0, g_nearAt = 3, g_doneAt = 100, g_attached = 0, g_filters = 0, g_handles = 0;
static bool g_shown = false; static CRGBColour g_colour(1, 1, 1, 1); static const char* g_icon = 0; static float g_scale = 0;
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return d == 20.0f && g_frames >= g_nearAt; }
class CGameScriptInterface { public:
    CScriptThing hero;
    void NewScriptFrame() const { ++g_frames; }
    const CScriptThing& GetHero() const { return hero; }
    void DisplayQuestInfo(bool d) const { g_shown = d; }
    long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour& c, const CCharString& icon, float s) const
    { g_colour = c; g_icon = icon.t; g_scale = s; return 42; }
    void EntityAttachToScript(const CScriptThing&, const CCharString& script) const { if (std::strcmp(script.t, "Q") == 0) ++g_attached; }
    CCharString GetActiveQuestName() const { return CCharString("Q", -1); }
};
struct CIsThingAlive { CIsThingAlive(const CScriptThing&) {} };
class CScriptGameResourceObjectScriptedThingBase { public: CScriptGameResourceObjectScriptedThingBase() { ++g_handles; } };
static const char g_rockTrollIcon[] = "HUD_QUEST_ICON_ROCK_TROLL";
class CRockTroll {
public:
    CGameScriptInterface* Game; CScriptThing Me; long HealthBarID;
    void RegisterFilter(CIsThingAlive&) { ++g_filters; }
    bool IsDone() { return g_frames >= g_doneAt; }
    void Main();
};
void CRockTroll::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->DisplayQuestInfo(true);
    {
        CCharString icon(g_rockTrollIcon, -1);
        HealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), icon, 1.0f);
    }
}

int main()
{
    CGameScriptInterface game; CRockTroll t; t.Game = &game; t.HealthBarID = 0;
    t.Main();
    bool ok = g_filters == 1 && g_attached == 1 && g_handles == 1 && g_frames == 3 && g_shown && t.HealthBarID == 42
           && g_colour.R == 0 && g_colour.G == 255 && g_colour.B == 0 && g_colour.A == 255 && g_scale == 1.0f
           && std::strcmp(g_icon, "HUD_QUEST_ICON_ROCK_TROLL") == 0;
    g_frames = 0; g_doneAt = 2; g_nearAt = 50; g_shown = false; t.HealthBarID = 0;
    t.Main();                                           // done before the hero arrives: no bar
    ok = ok && !g_shown && t.HealthBarID == 0;
    if (!ok) { std::printf("00da5f40_TEST FAIL\n"); return 1; }
    std::printf("00da5f40_TEST PASS\n");
    return 0;
}