// Behaviour test for CV_GhostGrannyNecklaceScript::CreateBandit (0x00E8CB40).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public:
    bool alive;
    bool IsAlive() const { return alive; }
    const char* GetPosition() const { return "pos"; }
};
class CGameScriptInterface { public:
    bool* knows; int knowAfter; int haveAfter; int levelFrames; int frame; bool banditAlive; bool spawned;
    void NewScriptFrame() { Log("frame"); ++frame; if (frame == knowAfter) *knows = true; if (spawned && levelFrames > 0) --levelFrames; }
    bool IsLevelLoaded(const char* l) { return std::strcmp(l, "Greatwood_3") == 0 && levelFrames > 0; }
    CScriptThing GetHero() { CScriptThing t; t.alive = true; return t; }
    bool IsObjectInThingsPossession(const char* o, const CScriptThing&) { return std::strcmp(o, "OBJECT_GHOST_GRANNY_NECKLACE") == 0 && frame >= haveAfter; }
    CScriptThing GetThingWithScriptName(const char*) { CScriptThing t; t.alive = banditAlive; return t; }
    CScriptThing CreateCreature(const char* def, const char* pos, const char* name, bool b) { Log("create"); Log(def); Log(name); if (b || std::strcmp(pos, "pos")) Log("badargs"); CScriptThing t; t.alive = true; return t; }
    void EntityAttachToScript(const CScriptThing&, const char* q) { Log("attach"); Log(q); }
    void SetCreatureGeneratorsEnabled(const char* g, bool on) { Log("gens"); Log(g); Log(on ? "1" : "0"); }
    const char* GetActiveQuestName() { return "Q"; }
};
struct CV_GhostGrannyNecklaceScript {
    CGameScriptInterface* Game;
    bool HeroKnowsAboutNecklace;
    bool BanditKilled;
    int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SpawnFunction(const char* name, const CScriptThing&) { Log("spawn"); Log(name); Game->spawned = true; }
    void CreateBandit()
    {
        while (!HeroKnowsAboutNecklace) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->SetCreatureGeneratorsEnabled("GreatwoodLake", false);
        while (!IsActiveThreadTerminating()) {
            if (Game->IsObjectInThingsPossession("OBJECT_GHOST_GRANNY_NECKLACE", Game->GetHero())) {
                if (IsActiveThreadTerminating()) return;
                if (IsActiveThreadTerminating()) return;
                Game->SetCreatureGeneratorsEnabled("GreatwoodLake", true);
                return;
            }
            if (Game->IsLevelLoaded("Greatwood_3") && BanditKilled != true && !Game->GetThingWithScriptName("GhostGrannyBandit").IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                CScriptThing bandit = Game->CreateCreature("CREATURE_BANDIT_LIEUTENANT", Game->GetThingWithScriptName("GhostGrannyBanditSpawn").GetPosition(), "GhostGrannyBandit", false);
                Game->EntityAttachToScript(bandit, Game->GetActiveQuestName());
                SpawnFunction("CheckBanditDead", bandit);
                while (!!Game->IsLevelLoaded("Greatwood_3")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
            }
            Game->NewScriptFrame();
        }
    }
};
static bool Run(int knowAfter, int haveAfter, int levelFrames, bool killed, int termAfter, const char* expect)
{
    g_log[0] = 0;
    CV_GhostGrannyNecklaceScript s; s.HeroKnowsAboutNecklace = false; s.BanditKilled = killed; s.termAfter = termAfter;
    CGameScriptInterface game; game.knows = &s.HeroKnowsAboutNecklace; game.knowAfter = knowAfter; game.haveAfter = haveAfter;
    game.levelFrames = levelFrames; game.frame = 0; game.banditAlive = false; game.spawned = false;
    s.Game = &game;
    s.CreateBandit();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got  %s\nwant %s\n", g_log, expect); return false; }
    return true;
}
int main()
{
    // knows after 1 frame; necklace held from frame 3; level not loaded: plain frames
    bool ok = Run(1, 3, 0, false, 100, "frame;gens;GreatwoodLake;0;frame;frame;gens;GreatwoodLake;1;")
        // level loaded for 2 frames once the bandit spawns; necklace held from frame 4
        && Run(1, 4, 2, false, 100, "frame;gens;GreatwoodLake;0;create;CREATURE_BANDIT_LIEUTENANT;GhostGrannyBandit;attach;Q;spawn;CheckBanditDead;frame;frame;frame;gens;GreatwoodLake;1;")
        // bandit already killed: no spawn
        && Run(1, 3, 5, true, 100, "frame;gens;GreatwoodLake;0;frame;frame;gens;GreatwoodLake;1;")
        // terminated during the first wait
        && Run(5, 9, 0, false, 1, "frame;frame;");
    if (!ok) { std::printf("00e8cb40_TEST FAIL\n"); return 1; }
    std::printf("00e8cb40_TEST PASS\n");
    return 0;
}