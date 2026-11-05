// Behaviour test for NScript::CQ_TraderConflictGoodScript::AttackPeople (0x00DFD600): while the hero has not entered the new
// region, enemies remain and fewer traders reached the teleporter than follow the hero, each pass finds the first enemy that is
// within 15 of the hero or of its nearest following trader, or aware of the hero, or aware of the trader (bandit guards excepted);
// that enemy targets the trader when near or aware of a living trader, else the hero; the attack line is said only when the
// conversation timer is 0 (then set to 4); the enemy gets a red mini-map orb and leaves the list. Five script frames per pass;
// on leaving, EnteredNewRegion is cleared. A log model.
#include <cstdio>
#include <cstring>
#include <vector>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Enemy { const char* Name; float HeroDist, TraderDist; bool AwareHero, AwareTrader; };
struct Script {
    bool EnteredNewRegion, TraderFollowing, TraderAlive;
    long TradersFollowing, TradersReachedTeleporter, Timer, Frames, EnterAt;
    std::vector<Enemy> AllCreatures;
    void Frame() { ++Frames; if (Timer > 0) --Timer; if (Frames == EnterAt) EnteredNewRegion = true; }
    void AttackPeople()
    {
        std::vector<Enemy> enemies(AllCreatures);
        while (!EnteredNewRegion && enemies.size() != 0 && TradersReachedTeleporter < TradersFollowing) {
            Frame();
            for (unsigned int i = 0; i < enemies.size(); ++i) {
                const bool trader = TraderFollowing;
                const Enemy& e = enemies[i];
                const bool nearTrader = trader && e.TraderDist < 15.0f, awareTrader = trader && e.AwareTrader;
                if (e.HeroDist < 15.0f || nearTrader || e.AwareHero || (awareTrader && std::strcmp(e.Name, "TC_BanditGuard") != 0)) {
                    const bool atTrader = nearTrader || (awareTrader && TraderAlive);
                    Log(e.Name); Log(atTrader ? "->trader" : "->hero");
                    if (Timer == 0) { Log(atTrader ? "TEXT_QST_B11_BANDIT_ATTACK_TRADER" : "TEXT_QST_B11_BANDIT_ATTACK_HERO"); Timer = 4; }
                    Log("HUD_ORB_RED_SMALL");
                    enemies.erase(enemies.begin() + i);
                    break;
                }
            }
            for (int f = 0; f < 5; ++f) Frame();
        }
        if (EnteredNewRegion) EnteredNewRegion = false;
    }
};
int main()
{
    Script s;
    s.EnteredNewRegion = false; s.TraderFollowing = true; s.TraderAlive = true;
    s.TradersFollowing = 2; s.TradersReachedTeleporter = 0; s.Timer = 0; s.Frames = 0; s.EnterAt = 100;
    Enemy a = { "A", 50, 50, false, false }, b = { "B", 10, 50, false, false }, c = { "TC_BanditGuard", 50, 50, false, true },
          d = { "D", 50, 5, false, false };
    s.AllCreatures.push_back(a); s.AllCreatures.push_back(b); s.AllCreatures.push_back(c); s.AllCreatures.push_back(d);
    s.EnterAt = 13;  // two passes (6 frames each), then the region is entered at frame 13
    g_log[0] = 0;
    s.AttackPeople();
    bool ok = std::strcmp(g_log, "B;->hero;TEXT_QST_B11_BANDIT_ATTACK_HERO;HUD_ORB_RED_SMALL;D;->trader;TEXT_QST_B11_BANDIT_ATTACK_TRADER;"
                                 "HUD_ORB_RED_SMALL;") == 0 && !s.EnteredNewRegion && s.Frames == 18;
    // bandit guard aware only of the trader is skipped; timer still running so no line
    Script t = s; t.EnteredNewRegion = false; t.Frames = 0; t.EnterAt = 7; t.Timer = 3; t.AllCreatures.clear();
    t.AllCreatures.push_back(c); Enemy e2 = { "E", 50, 50, true, false }; t.AllCreatures.push_back(e2);
    g_log[0] = 0;
    t.AttackPeople();
    ok = ok && std::strcmp(g_log, "E;->hero;HUD_ORB_RED_SMALL;") == 0;
    if (!ok) { std::printf("%s\n00dfd600_TEST FAIL\n", g_log); return 1; }
    std::printf("00dfd600_TEST PASS\n");
    return 0;
}
