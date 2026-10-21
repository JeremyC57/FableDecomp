// Behaviour test for 0x00EF1E10 (CTrappedMan::Main): the call sequence of the script over a logging game interface.
#include <cstdio>
#include <cstring>
// Host model: the game interface logs every call ('F' frame, 'A' first message, 'D' death message, 'h' health query),
// Me reports a hit by the hero on chosen frames, IsDone turns true after a configurable number of checks.
struct Game {
    char log[128]; int n; float health; int frames;
    Game(float h) : n(0), health(h), frames(0) { log[0] = 0; }
    void Add(char c) { if (n < 126) { log[n++] = c; log[n] = 0; } }
    void NewScriptFrame() { Add('F'); ++frames; }
    void AddScreenMessage(const char* text, long sel)
    {
        if (sel != 0) Add('?');
        else if (std::strcmp(text, "You hit the man on the hill!") == 0) Add('A');
        else if (std::strcmp(text, "You hit him to the point of death!") == 0) Add('D');
        else Add('?');
    }
    float GetHealth() { Add('h'); return health; }
};
struct Thing {
    Game* game; int hitFrame;
    bool MsgIsHitBy(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && game->frames >= hitFrame; }
};
struct TrappedMan {
    Game* Game_; Thing Me; int filters; int doneAfter; int checks;
    TrappedMan(Game* g, int after, int hit) : Game_(g), filters(0), doneAfter(after), checks(0) { Me.game = g; Me.hitFrame = hit; }
    void RegisterFilter() { ++filters; }
    bool IsDone() { return ++checks > doneAfter; }
    void Main()
    {
        RegisterFilter();
        Game_->NewScriptFrame();
        if (IsDone()) return;
        while (!IsDone()) {
            while (!Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                Game_->NewScriptFrame();
                if (IsDone()) return;
            }
            if (IsDone()) return;
            Game_->AddScreenMessage("You hit the man on the hill!", 0);
            if (Game_->GetHealth() < 1e-4f) {
                if (IsDone()) return;
                Game_->AddScreenMessage("You hit him to the point of death!", 0);
            }
            Game_->NewScriptFrame();
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
int main()
{
    { Game g(5.0f); TrappedMan t(&g, 0, 0); t.Main(); CHECK(std::strcmp(g.log, "F") == 0 && t.filters == 1); }
    { Game g(5.0f); TrappedMan t(&g, 1, 0); t.Main(); CHECK(std::strcmp(g.log, "F") == 0); }
    // hit from frame 1 (immediately), healthy: message + health query + frame each loop
    { Game g(5.0f); TrappedMan t(&g, 5, 1); t.Main(); CHECK(std::strcmp(g.log, "FAhFAhF") == 0); }
    // dying: death message follows the first
    { Game g(0.0f); TrappedMan t(&g, 7, 1); t.Main(); CHECK(std::strcmp(g.log, "FAhDFAhDF") == 0); }
    // no hit until frame 3: waits two frames first
    { Game g(5.0f); TrappedMan t(&g, 6, 3); t.Main(); CHECK(std::strcmp(g.log, "FFFAhF") == 0); }
    // done while waiting for the hit
    { Game g(5.0f); TrappedMan t(&g, 3, 9); t.Main(); CHECK(std::strcmp(g.log, "FFF") == 0); }
    if (g_fail) { std::printf("00ef1e10_TEST FAIL\n"); return 1; }
    std::printf("00ef1e10_TEST PASS\n");
    return 0;
}
