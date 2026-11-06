// Behaviour test for CQ_UndeadRisingScript::Main (0x00E0B160): wait for GraveYard_2; until the hero reaches the centre keep the
// rock sparkles and the portal alive (re-creating whichever died); then status COLLECTING_TOKENS (1) until every activation
// point has a zombie token, FIGHTING_BOSSES (2) until every boss point has a kill, then COMPLETED (3) with MissionSucceeded.
#include <cstdio>

struct Quest {
    bool LevelLoaded, HeroReachedCentre, SparklesAlive, PortalAlive, BossCutscenePlayed, MissionSucceeded;
    long Status, Tokens, Kills, ActivationPoints, BossPoints, SparkleSpawns, PortalSpawns, Phase;
    void Step()
    {
        switch (Phase) {
        case 0: if (LevelLoaded) { Phase = 1; SparkleSpawns = PortalSpawns = 1; SparklesAlive = PortalAlive = true; } break;
        case 1:
            if (HeroReachedCentre) { Status = 1; Phase = 2; break; }
            if (!LevelLoaded) break;
            if (!SparklesAlive) { SparklesAlive = true; ++SparkleSpawns; }
            if (!PortalAlive) { PortalAlive = true; ++PortalSpawns; }
            break;
        case 2: if (!(Tokens < ActivationPoints)) { Status = 2; BossCutscenePlayed = true; Phase = 3; } break;
        case 3: if (!(Kills < BossPoints)) { Status = 3; MissionSucceeded = true; Phase = 4; } break;
        default: break;
        }
    }
};

int main()
{
    Quest q = { false, false, false, false, false, false, 0, 0, 0, 3, 3, 0, 0, 0 };
    bool ok = true;
    q.Step(); ok = ok && q.Phase == 0;
    q.LevelLoaded = true; q.Step(); ok = ok && q.Phase == 1 && q.SparkleSpawns == 1 && q.PortalSpawns == 1;
    q.PortalAlive = false; q.Step(); ok = ok && q.PortalSpawns == 2 && q.SparkleSpawns == 1;
    q.SparklesAlive = false; q.LevelLoaded = false; q.Step(); ok = ok && q.SparkleSpawns == 1;
    q.LevelLoaded = true; q.Step(); ok = ok && q.SparkleSpawns == 2;
    q.HeroReachedCentre = true; q.Step(); ok = ok && q.Status == 1 && q.Phase == 2;
    q.Tokens = 2; q.Step(); ok = ok && q.Status == 1;
    q.Tokens = 3; q.Step(); ok = ok && q.Status == 2 && q.BossCutscenePlayed;
    q.Kills = 2; q.Step(); ok = ok && q.Status == 2 && !q.MissionSucceeded;
    q.Kills = 3; q.Step(); ok = ok && q.Status == 3 && q.MissionSucceeded;
    if (!ok) { std::printf("00e0b160_TEST FAIL\n"); return 1; }
    std::printf("00e0b160_TEST PASS\n");
    return 0;
}