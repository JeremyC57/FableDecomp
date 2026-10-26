// Behaviour test for CV_HeroDuelScript::ShowEnemyHealth (0x00E99470): health tracking until Thunder is beaten or the mission ends.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class C2DVector { public: float X, Y; C2DVector(float x, float y) { X = x; Y = y; } };
class CScriptThing { public: const char* name; bool alive; bool IsAlive() const { return alive; } };
struct CV_HeroDuelScript;
class CGameScriptInterface { public:
    CV_HeroDuelScript* script;
    int frame;
    float health;
    unsigned char barR, barG, barB;
    void NewScriptFrame();
    CScriptThing GetThingWithScriptName(const char* name) { CScriptThing t; t.name = name; t.alive = true; return t; }
    float GetHealth(const CScriptThing&) { return health; }
    void DisplayQuestInfo(bool on) { Log(on ? "info-on" : "info-off"); }
    long AddQuestInfoBarHealth(const CScriptThing& t, const CRGBColour& c, const char* icon, float) { Log(t.name); Log(icon); barR = c.R; barG = c.G; barB = c.B; return 1; }
};
struct CV_HeroDuelScript {
    CGameScriptInterface* Game;
    bool MissionSucceeded, MissionFailed, DuelStarted, ThunderDefeated;
    unsigned long MyHealth;
    bool IsActiveThreadTerminating() { return false; }
    void ShowEnemyHealth()
    {
        while (!DuelStarted) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
        static const CRGBColour healthHigh(0, 128, 0, 255);
        static const C2DVector barPosition(370.0f, 50.0f);
        Game->AddQuestInfoBarHealth(thunder, CRGBColour(255, 255, 0, 255), "HUD_QUEST_ICON_THUNDER", 1.0f);
        Game->DisplayQuestInfo(true);
        while (!ThunderDefeated) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (thunder.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                MyHealth = (unsigned long)Game->GetHealth(thunder);
            }
            if (MissionSucceeded || MissionFailed) {
                if (IsActiveThreadTerminating()) return;
                Game->DisplayQuestInfo(false);
                return;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
void CGameScriptInterface::NewScriptFrame()
{
    ++frame;
    health -= 10.0f;
    if (frame == 2) script->DuelStarted = true;
    if (frame == 6) script->ThunderDefeated = true;
}
int main()
{
    CGameScriptInterface game;
    CV_HeroDuelScript s;
    game.script = &s; game.frame = 0; game.health = 100.0f;
    s.Game = &game;
    s.MissionSucceeded = s.MissionFailed = s.DuelStarted = s.ThunderDefeated = false;
    s.MyHealth = 0;
    g_log[0] = 0;
    s.ShowEnemyHealth();
    // frames 1-2 wait for the duel, the loop then runs passes at frames 3-4 and 5-6
    bool ok = std::strcmp(g_log, "Thunder;HUD_QUEST_ICON_THUNDER;info-on;") == 0 && s.MyHealth == 40 && game.frame == 6
        && game.barR == 255 && game.barG == 255 && game.barB == 0;
    game.frame = 10; game.health = 75.0f; s.ThunderDefeated = false; s.MissionFailed = true; g_log[0] = 0;
    s.ShowEnemyHealth();
    ok = ok && std::strcmp(g_log, "Thunder;HUD_QUEST_ICON_THUNDER;info-on;info-off;") == 0 && s.MyHealth == 55;
    if (!ok) { std::printf("00e99470_TEST FAIL\n"); return 1; }
    std::printf("00e99470_TEST PASS\n");
    return 0;
}