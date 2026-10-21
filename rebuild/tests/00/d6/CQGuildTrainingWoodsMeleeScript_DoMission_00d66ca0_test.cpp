// Behaviour test for 00d66ca0 (CQ_GuildTrainingWoodsMeleeScript::DoMission): objective first, wait for the level,
// spawn the two threads in order, take a frame, then EndMission unless the mission failed.
#include <cstdio>
#include <cstring>

struct Game {
    int frames, levelAt; int objectives; char spawned[2][32]; int nspawned; int ended;
    void NewScriptFrame() { ++frames; }
    bool IsLevelLoaded(const char* n) const { return std::strcmp(n, "GuildWoods") == 0 && frames >= levelAt; }
    void GiveHeroNewQuestObjective(const char* t, unsigned long id) { if (std::strcmp(t, "first objective") == 0 && id == 1) ++objectives; }
};
struct Script {
    Game* game; bool failed; int termAt;
    bool IsActiveThreadTerminating() { return game->frames >= termAt; }
    void Spawn(const char* n) { std::strcpy(game->spawned[game->nspawned++], n); }
    void EndMission() { ++game->ended; }
    void DoMission()
    {
        game->GiveHeroNewQuestObjective("first objective", 1);
        while (!game->IsLevelLoaded("GuildWoods")) { game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Spawn("WatchForLeaving");
        Spawn("TeleportOutHero");
        game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!failed) EndMission();
    }
};
int main()
{
    int ok = 1;
    Game g; std::memset(&g, 0, sizeof g); g.levelAt = 3;
    Script s = { &g, false, 100 };
    s.DoMission();
    if (g.objectives != 1 || g.frames != 4 || g.nspawned != 2 || std::strcmp(g.spawned[0], "WatchForLeaving") != 0 ||
        std::strcmp(g.spawned[1], "TeleportOutHero") != 0 || g.ended != 1) ok = 0;
    Game h; std::memset(&h, 0, sizeof h);
    Script t = { &h, true, 100 };
    t.DoMission();
    if (h.ended != 0 || h.nspawned != 2) ok = 0;
    Game k; std::memset(&k, 0, sizeof k); k.levelAt = 10;
    Script u = { &k, false, 2 };
    u.DoMission();
    if (k.nspawned != 0 || k.ended != 0 || k.objectives != 1) ok = 0;
    if (!ok) { std::printf("00d66ca0_TEST FAIL\n"); return 1; }
    std::printf("00d66ca0_TEST PASS\n");
    return 0;
}