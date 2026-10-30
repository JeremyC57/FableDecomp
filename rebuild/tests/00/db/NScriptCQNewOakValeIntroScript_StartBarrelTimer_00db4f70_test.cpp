// Behaviour test for NScript::CQ_NewOakValeIntroScript::StartBarrelTimer (0x00DB4F70): nothing happens until the watch timer
// is running; then the HUD clock bar appears, turns green while the hero is within 2 units of the guard point and red
// otherwise, shows the remaining watch time each frame, and is removed once the barrel man has spoken to the returning hero;
// termination stops the thread early and leaves the bar.
#include <cstdio>
#include <cstring>

struct Thing { int id; float x; };
struct Colour { unsigned char B, G, R, A; Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };

static char gLog[1024];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Game;
struct Script;
static Script* gScript;

struct Game {
    Thing hero, guardPoint;
    long timer;
    int frame;
    void NewScriptFrame();
    Thing& GetHero() { return hero; }
    Thing GetThingWithScriptName(const char* name) { Thing t = { 0, 0.0f }; if (std::strcmp(name, "M_WHouse_GuardPoint") == 0) t = guardPoint; return t; }
    long AddQuestInfoBar(float a, float b, Colour c1, Colour c2, const char* icon, const char* text, float scale)
    {
        if (a == 45.0f && b == 0.0f && c1.G == 255 && c1.R == 0 && c2.G == 255 && std::strcmp(icon, "HUD_CLOCK_ICON") == 0 && text[0] == 0 && scale == 1.0f)
            Log("Bar");
        return 7;
    }
    void ChangeQuestInfoBarColour(long bar, Colour c1, Colour c2)
    {
        if (bar != 7) return;
        if (c1.G == 255 && c1.R == 0 && c2.G == 255 && c2.R == 0 && c1.A == 255) Log("Green");
        else if (c1.R == 255 && c1.G == 0 && c2.R == 255 && c2.G == 0 && c1.A == 255) Log("Red");
    }
    void UpdateQuestInfoBar(long bar, float value, float a, float b) { if (bar == 7 && a == -1.0f && b == -1.0f && value == (float)timer) Log("Update"); }
    void RemoveQuestInfoElement(long bar) { if (bar == 7) Log("Remove"); }
};

static bool IsDistanceBetweenThingsUnder(const Thing& a, const Thing& b, float d)
{
    float dx = a.x - b.x; if (dx < 0) dx = -dx;
    return dx < d;
}

struct Script {
    Game* Game_;
    int termBudget;
    long GUIBarrelCounter;
    bool BarrelManSpokenToHeroOnReturn;
    long WatchTimer() { return Game_->timer; }
    bool IsActiveThreadTerminating() { if (termBudget <= 0) return true; --termBudget; return false; }
    void StartBarrelTimer()
    {
        while (WatchTimer() <= 0) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        GUIBarrelCounter = Game_->AddQuestInfoBar(45.0f, 0.0f, Colour(0, 255, 0, 255), Colour(0, 255, 0, 255), "HUD_CLOCK_ICON", "", 1.0f);
        Thing guardPoint = Game_->GetThingWithScriptName("M_WHouse_GuardPoint");
        while (!BarrelManSpokenToHeroOnReturn) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsUnder(Game_->GetHero(), guardPoint, 2.0f))
                Game_->ChangeQuestInfoBarColour(GUIBarrelCounter, Colour(0, 255, 0, 255), Colour(0, 255, 0, 255));
            else
                Game_->ChangeQuestInfoBarColour(GUIBarrelCounter, Colour(255, 0, 0, 255), Colour(255, 0, 0, 255));
            Game_->UpdateQuestInfoBar(GUIBarrelCounter, (float)WatchTimer(), -1.0f, -1.0f);
        }
        if (IsActiveThreadTerminating())
            return;
        Game_->RemoveQuestInfoElement(GUIBarrelCounter);
    }
};

// Frame script: the timer starts on frame 2, the hero walks away from the guard point on frame 4 and the barrel man speaks
// on frame 5.
void Game::NewScriptFrame()
{
    ++frame;
    if (frame == 2) timer = 30;
    if (timer > 0 && frame > 2) --timer;
    if (frame == 4) hero.x = 10.0f;
    if (frame == 5) gScript->BarrelManSpokenToHeroOnReturn = true;
}

int main()
{
    Game g; g.hero.id = 1; g.hero.x = 0.5f; g.guardPoint.id = 2; g.guardPoint.x = 0.0f; g.timer = 0; g.frame = 0;
    Script s; s.Game_ = &g; s.termBudget = 100; s.GUIBarrelCounter = 0; s.BarrelManSpokenToHeroOnReturn = false;
    gScript = &s; gLog[0] = 0;
    s.StartBarrelTimer();
    if (std::strcmp(gLog, "Bar;Green;Update;Red;Update;Red;Update;Remove;") != 0 || s.GUIBarrelCounter != 7) {
        std::printf("00db4f70_TEST FAIL\n"); return 1;
    }
    g.hero.x = 0.5f; g.timer = 0; g.frame = 0; s.termBudget = 3; s.BarrelManSpokenToHeroOnReturn = false; gLog[0] = 0;
    s.StartBarrelTimer();
    if (std::strstr(gLog, "Remove") != 0) { std::printf("00db4f70_TEST FAIL\n"); return 1; }
    std::printf("00db4f70_TEST PASS\n");
    return 0;
}