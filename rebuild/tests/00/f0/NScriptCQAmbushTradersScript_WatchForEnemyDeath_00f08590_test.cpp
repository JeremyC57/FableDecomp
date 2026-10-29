// Behaviour test for NScript::CQ_AmbushTradersScript::WatchForEnemyDeath 0x00F08590: the enemy's def name is taken once;
// every frame the thread asks the enemy who killed it; once a killer exists the watch ends, and only a hero kill calls
// EntityCountDecrease(defName); a terminating thread returns before (or instead of) the decrease.
#include <cstdio>
#include <cstring>
static int g_frames, g_terminateAt, g_calls;
static char g_decreased[64];
class CScriptThing { public:
    const char* name; const char* defName; int dieAfter; const char* killerName;
    const char* GetName() const { return name ? name : ""; }
    const char* GetDefName() const { return defName ? defName : ""; }
    bool IsNull() const { return name == 0; }
    CScriptThing MsgWhoKilledMe() const
    {
        CScriptThing k; k.name = 0; k.defName = 0; k.dieAfter = 0; k.killerName = 0;
        if (g_frames >= dieAfter) k.name = killerName;
        return k;
    }
};
struct Game { void NewScriptFrame() { ++g_frames; } };
struct CQ_AmbushTradersScript {
    Game* Game_;
    bool IsActiveThreadTerminating() { ++g_calls; return g_calls == g_terminateAt; }
    void EntityCountDecrease(const char* defName) { std::strcpy(g_decreased, defName); }
    void WatchForEnemyDeath(CScriptThing enemy)
    {
        CScriptThing killer; killer.name = 0;
        char defName[64]; std::strcpy(defName, enemy.GetDefName());
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            killer = enemy.MsgWhoKilledMe();
            if (!killer.IsNull()) {
                if (IsActiveThreadTerminating()) return;
                if (IsActiveThreadTerminating()) return;
                if (std::strcmp(killer.GetName(), "SCRIPT_NAME_HERO") == 0) {
                    if (IsActiveThreadTerminating()) return;
                    EntityCountDecrease(defName);
                }
                break;
            }
            Game_->NewScriptFrame();
        }
    }
};
static bool Run(const char* killer, int dieAfter, int terminateAt, const char* expectDecrease, int expectFrames)
{
    g_frames = 0; g_calls = 0; g_terminateAt = terminateAt; g_decreased[0] = 0;
    Game g; CQ_AmbushTradersScript s; s.Game_ = &g;
    CScriptThing e; e.name = "Bandit01"; e.defName = "CREATURE_BANDIT_ARCHER"; e.dieAfter = dieAfter; e.killerName = killer;
    s.WatchForEnemyDeath(e);
    return std::strcmp(g_decreased, expectDecrease) == 0 && g_frames == expectFrames;
}
int main()
{
    if (!Run("SCRIPT_NAME_HERO", 3, 0, "CREATURE_BANDIT_ARCHER", 3) || !Run("TraderGuard", 2, 0, "", 2)
        || !Run("SCRIPT_NAME_HERO", 5, 3, "", 2) || !Run("SCRIPT_NAME_HERO", 0, 4, "", 0) || !Run("SCRIPT_NAME_HERO", 0, 0, "CREATURE_BANDIT_ARCHER", 0)) {
        std::printf("00f08590_TEST FAIL\n");
        return 1;
    }
    std::printf("00f08590_TEST PASS\n");
    return 0;
}