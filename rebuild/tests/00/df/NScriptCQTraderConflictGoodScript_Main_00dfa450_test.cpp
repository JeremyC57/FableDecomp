// Behaviour test for NScript::CQ_TraderConflictGoodScript::Main (0x00DFA450): seven bindings (BCMTrader / BCGameMaster / BanditExtra
// flag 0), wait for the entrance, open Gate1 / Gate2Outer, intro cut-scene (IntroDone), start screen, fade in, WatchTimeLimit, rebuild
// the enemy list, objective 1, give every enemy a 10 m break-off range, wake it and put it into FACTION_BANDITS, four watcher threads,
// wait for the camp centre while marking FirstRegionDead once the list is empty, open Gate2Inner, wait for the outro, complete the quest
// and once the hero has left the entrance deactivate the extras and (later) the quest. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    unsigned int EnemyCount;
    bool FirstRegionDead, IntroDone, OutroDone;
    int Frames, Configured;
    bool IsRegionLoaded(const char* r)
    {
        if (!std::strcmp(r, "BanditCampEntrance")) return Frames < 1 || (Frames >= 1 && !OutroDone);
        if (!std::strcmp(r, "BanditCampCentre")) return Frames >= 3;
        return false;
    }
    void NewScriptFrame() { ++Frames; if (Frames == 2) EnemyCount = 0; if (Frames == 4) OutroDone = true; }
    bool IsActiveThreadTerminating() { return false; }
    void Main()
    {
        Log("bind7");
        while (!IsRegionLoaded("BanditCampEntrance")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("Gate1"); Log("Gate2Outer"); Log("CS_TRADERCON_GOOD_INTRO");
        IntroDone = true;
        Log("start_screen"); Log("fade_in"); Log("WatchTimeLimit"); Log("update_enemies"); Log("obj1");
        for (unsigned int i = 0; i < EnemyCount; ++i) { if (IsActiveThreadTerminating()) return; ++Configured; }
        if (IsActiveThreadTerminating()) return;
        Log("watchers4");
        while (!IsRegionLoaded("BanditCampCentre")) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (!FirstRegionDead && EnemyCount == 0) { if (IsActiveThreadTerminating()) return; FirstRegionDead = true; Log("first_dead"); }
        }
        if (IsActiveThreadTerminating()) return;
        Log("Gate2Inner");
        while (!OutroDone) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("completed");
        while (!(!IsRegionLoaded("BanditCampEntrance"))) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("-Q_TraderConflictGood_Extras"); Log("deactivate_later");
    }
};
int main()
{
    Quest q = { 3, false, false, false, 1, 0 };
    q.Main();
    bool ok = !std::strcmp(g_log, "bind7;Gate1;Gate2Outer;CS_TRADERCON_GOOD_INTRO;start_screen;fade_in;WatchTimeLimit;update_enemies;obj1;"
                                  "watchers4;first_dead;Gate2Inner;completed;-Q_TraderConflictGood_Extras;deactivate_later;")
        && q.Configured == 3 && q.IntroDone && q.FirstRegionDead && q.Frames == 4;
    std::printf(ok ? "00dfa450_TEST PASS\n" : "00dfa450_TEST FAIL\n");
    return ok ? 0 : 1;
}