// Behaviour test for NScript::CQ_HangingTreeEvilScript::WatchForHeroNearExit (0x00D6CA90): the exit points are collected once,
// then each frame the hero is checked against every one within 8 units; the first frame he is near one, the bandit's
// "don't exit" line is queued in a new conversation and the thread ends; termination stops it early with no line.
#include <cstdio>
#include <cstring>
#include <vector>

struct Thing { int id; float x; };

static int gLines;
static int gFrames;

struct Game {
    Thing hero;
    int heroMoveFrame;
    std::vector<Thing> points;
    Thing& GetHero() { return hero; }
    void NewScriptFrame() { ++gFrames; if (gFrames == heroMoveFrame) hero.x = 100.0f; }
    void GetAllThingsWithDefName(const char* name, std::vector<Thing>& out) { if (std::strcmp(name, "REGION_EXIT_POINT") == 0) out = points; }
    long AddNewConversation(const Thing& speaker, bool, bool) { return speaker.id == hero.id ? 42 : -1; }
    void AddLineToConversation(long conv, const char* text, bool, const Thing& a, const Thing& b)
    {
        if (conv == 42 && std::strcmp(text, "TEXT_QST_029_BANDIT_DONT_EXIT") == 0 && a.id == hero.id && b.id == hero.id) ++gLines;
    }
};

static bool IsDistanceBetweenThingsUnder(const Thing& a, const Thing& b, float d)
{
    float dx = a.x - b.x; if (dx < 0) dx = -dx;
    return dx < d;
}

struct Script {
    Game* Game_;
    int termBudget;
    bool IsActiveThreadTerminating() { if (termBudget <= 0) return true; --termBudget; return false; }
    void WatchForHeroNearExit()
    {
        bool warned = false;
        std::vector<Thing> exitPoints;
        Game_->GetAllThingsWithDefName("REGION_EXIT_POINT", exitPoints);
        do {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            for (unsigned long i = 0; i < exitPoints.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (IsDistanceBetweenThingsUnder(Game_->GetHero(), exitPoints[i], 8.0f)) {
                    long conversation = Game_->AddNewConversation(Game_->GetHero(), false, false);
                    Game_->AddLineToConversation(conversation, "TEXT_QST_029_BANDIT_DONT_EXIT", false, Game_->GetHero(), Game_->GetHero());
                    warned = true;
                }
            }
            if (IsActiveThreadTerminating())
                return;
        } while (!warned);
        if (IsActiveThreadTerminating())
            return;
    }
};

int main()
{
    Game g; g.hero.id = 1; g.hero.x = 0.0f; g.heroMoveFrame = 4;
    Thing a = { 2, 50.0f }, b = { 3, 105.0f };
    g.points.push_back(a); g.points.push_back(b);
    Script s; s.Game_ = &g; s.termBudget = 1000;
    gLines = 0; gFrames = 0;
    s.WatchForHeroNearExit();
    if (gLines != 1 || gFrames != 4) { std::printf("00d6ca90_TEST FAIL\n"); return 1; }
    g.hero.x = 0.0f; gLines = 0; gFrames = 0; s.termBudget = 5;
    s.WatchForHeroNearExit();
    if (gLines != 0 || gFrames != 2) { std::printf("00d6ca90_TEST FAIL\n"); return 1; }
    std::printf("00d6ca90_TEST PASS\n");
    return 0;
}