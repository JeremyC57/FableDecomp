// Behaviour test for 0x00D91E00 NScript::CQ_HobbeCaveScript::CHobbeSpawn::Main(): a self-contained host model of the row's logic
// (spawn count from the def or the marker's data string, capped by HobbesToSpawn; per frame while hobbes are left: alarm, spawn
// timer, wait for the roaming hobbe population to drop below the region maximum, spawn off camera and away from the hero; take one
// from HobbesToSpawn at the end; terminate checks) over a scripted game mock.
#include <cstdio>
#include <cstdlib>

struct Def { long SpawnCountPostReleaseGSon, ReduceCountPostSideRoom, HobbeSpawnDelay, HobbeMaxNumberInRegion; };
struct Quest { bool AlarmTriggered, KilledSideRoomHobbes, ReleaseBrat, InCutscene; long HobbesToSpawn; };

struct Game {
    int frame;
    int terminateAtFrame;      // IsActiveThreadTerminating true once frame >= this (-1 never)
    long timer;                // spawn timer (seconds, counts down one per frame)
    int roaming;               // live "HobbeRoaming" hobbes
    int deadOnFrame;           // first frame at which a roaming hobbe dies (scan sees a dead one)
    bool onScreen;
    float heroDistance;
    int spawned;
    int timerSets;
    void NewScriptFrame() { ++frame; if (timer > 0) --timer; if (deadOnFrame >= 0 && frame >= deadOnFrame && roaming > 0) { --roaming; deadOnFrame = frame + 3; } }
    bool Terminating() const { return terminateAtFrame >= 0 && frame >= terminateAtFrame; }
};

struct Spawner {
    Game* game; Quest* quest; const Def* def; const char* data;
    long NumberToGenerate;
    void Main() {
        long timer = 0; game->timer = 0;
        if (quest->ReleaseBrat) {
            if (game->Terminating()) return;
            NumberToGenerate = def->SpawnCountPostReleaseGSon;
        } else if (quest->KilledSideRoomHobbes) {
            if (game->Terminating()) return;
            NumberToGenerate = std::atol(data) - def->ReduceCountPostSideRoom;
        } else {
            if (game->Terminating()) return;
            NumberToGenerate = std::atol(data);
        }
        if (NumberToGenerate > quest->HobbesToSpawn) {
            if (game->Terminating()) return;
            NumberToGenerate = quest->HobbesToSpawn;
        }
        (void)timer;
        while (NumberToGenerate > 0) {
            game->NewScriptFrame();
            if (game->Terminating()) return;
            if (quest->AlarmTriggered) {
                if (game->Terminating()) return;
                if (game->timer == 0) {
                    if (game->Terminating()) return;
                    int hobbes = game->roaming;
                    while (hobbes > def->HobbeMaxNumberInRegion) {
                        game->NewScriptFrame();
                        if (game->Terminating()) return;
                        bool oneDied = false;
                        while (!oneDied) {
                            int before = game->roaming;
                            game->NewScriptFrame();
                            if (game->Terminating()) return;
                            for (int i = 0; i < hobbes; ++i) {
                                if (game->Terminating()) return;
                                if (i >= game->roaming || game->roaming < before) {
                                    if (game->Terminating()) return;
                                    oneDied = true;
                                }
                            }
                            if (game->Terminating()) return;
                        }
                        if (game->Terminating()) return;
                        hobbes = game->roaming;
                    }
                    if (game->Terminating()) return;
                    if (!quest->InCutscene && !game->onScreen && game->heroDistance > 8.0f) {
                        if (game->Terminating()) return;
                        ++game->spawned; ++game->roaming;
                        --NumberToGenerate;
                    }
                    game->timer = def->HobbeSpawnDelay; ++game->timerSets;
                }
            }
        }
        if (game->Terminating()) return;
        if (quest->HobbesToSpawn > 1) {
            if (game->Terminating()) return;
            --quest->HobbesToSpawn;
        }
    }
};

static int g_fail;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); g_fail = 1; } }

static void Reset(Game& g, Quest& q) {
    g.frame = 0; g.terminateAtFrame = -1; g.timer = 0; g.roaming = 0; g.deadOnFrame = -1; g.onScreen = false; g.heroDistance = 20.0f;
    g.spawned = 0; g.timerSets = 0;
    q.AlarmTriggered = true; q.KilledSideRoomHobbes = false; q.ReleaseBrat = false; q.InCutscene = false; q.HobbesToSpawn = 10;
}

int main()
{
    const Def def = { 6, 2, 3, 4 };
    Game g; Quest q; Spawner s;
    s.game = &g; s.quest = &q; s.def = &def; s.data = "5";
    // Count from the data string; one spawn per delay period; HobbesToSpawn loses one at the end.
    Reset(g, q); s.Main();
    Check(g.spawned == 5 && s.NumberToGenerate == 0 && q.HobbesToSpawn == 9, "data-string count");
    Check(g.timerSets == 5, "timer reset after each spawn attempt");
    // After the brat's release the def count wins; capped by HobbesToSpawn.
    Reset(g, q); q.ReleaseBrat = true; q.HobbesToSpawn = 4; s.Main();
    Check(g.spawned == 4 && q.HobbesToSpawn == 3, "release count capped");
    // Side room cleared: data string less the reduction.
    Reset(g, q); q.KilledSideRoomHobbes = true; s.Main();
    Check(g.spawned == 3, "side-room reduction");
    // HobbesToSpawn of 1 is not decremented.
    Reset(g, q); q.HobbesToSpawn = 1; s.Main();
    Check(g.spawned == 1 && q.HobbesToSpawn == 1, "last hobbe kept");
    // Too many roaming hobbes: wait until one dies before each spawn.
    Reset(g, q); g.roaming = 6; g.deadOnFrame = 2; s.data = "2"; s.Main();
    Check(g.spawned == 2 && g.roaming <= 6, "population cap");
    s.data = "5";
    // On screen or near the hero: nothing spawns before termination.
    Reset(g, q); g.onScreen = true; g.terminateAtFrame = 50; s.Main();
    Check(g.spawned == 0 && q.HobbesToSpawn == 10, "no spawn on screen");
    Reset(g, q); g.heroDistance = 5.0f; g.terminateAtFrame = 50; s.Main();
    Check(g.spawned == 0, "no spawn near hero");
    // No alarm: no spawn; terminate at once: nothing at all.
    Reset(g, q); q.AlarmTriggered = false; g.terminateAtFrame = 20; s.Main();
    Check(g.spawned == 0 && q.HobbesToSpawn == 10, "no alarm");
    Reset(g, q); g.terminateAtFrame = 0; s.Main();
    Check(g.spawned == 0 && g.frame == 0 && q.HobbesToSpawn == 10, "terminating");
    if (g_fail) { std::printf("00d91e00_TEST FAIL\n"); return 1; }
    std::printf("00d91e00_TEST PASS\n");
    return 0;
}