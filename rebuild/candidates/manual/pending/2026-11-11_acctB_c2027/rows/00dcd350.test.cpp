// Behaviour test for 0x00DCD350 NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Main(): a self-contained host model of the row's
// logic (wait for the introduction; start the reinforcements timer; the hero's bandit team shows the bandits-left counter; per
// frame until Whisper spawns: the guard team refills to TeamMemberLimit at Me (hero far, Me off camera) or at a re-picked random
// "EitherTeamSpawn" marker; the bandit team updates the counter and spawns waves (archer as second member when the hero leads the
// guards), a guard near the hero says NEXT_WAVE, three waves end the spawner (MissionFailed = 3 for the hero's team); remove the
// counter at the end; terminate checks) over a scripted game mock.
#include <cstdio>
#include <cstring>

struct Team { long MemberCount; long Timer; };
struct Parent {
    bool DoneIntroduction, WhisperSpawned;
    long MissionFailed, HeroTeam, BanditWavesSpawned;
    Team Teams[2];
};

struct Game {
    int frame, terminateAtFrame, whisperAtFrame, introAtFrame;
    Parent* parent;
    bool heroFar, onScreen, guardAlive, guardNearHero, banditsDie;
    int created, archers, atMe, atOther, attached, pauses, repicks, comments, counterUpdates, displayed, removed;
    long lastCounterValue, lastPause;
    void NewScriptFrame() {
        ++frame;
        if (frame >= introAtFrame) parent->DoneIntroduction = true;
        if (whisperAtFrame >= 0 && frame >= whisperAtFrame) parent->WhisperSpawned = true;
        for (int t = 0; t < 2; ++t) if (parent->Teams[t].Timer > 0) --parent->Teams[t].Timer;
        if (banditsDie) parent->Teams[1].MemberCount = 0;   // every bandit wave is wiped out within a frame
        if (frame > 1000) terminateAtFrame = frame;         // safety net for the host run
    }
    bool Terminating() const { return terminateAtFrame >= 0 && frame >= terminateAtFrame; }
};

struct TeamSpawn {
    Game* game; Parent* ParentClass;
    long TeamID, TeamMemberLimit, TeamRespawnTime, BanditsLeftID;
    long reinforcementDelay;
    void Create(Team& team, bool archer, bool atMe) {
        ++game->created; if (archer) ++game->archers; if (atMe) ++game->atMe; else ++game->atOther;
        ++game->attached; ++game->pauses; ++team.MemberCount;
    }
    void Main() {
        while (!ParentClass->DoneIntroduction) {
            game->NewScriptFrame();
            if (game->Terminating()) return;
        }
        if (game->Terminating()) return;
        ParentClass->Teams[TeamID].Timer = TeamRespawnTime;
        if (TeamID == 1 && ParentClass->HeroTeam == 1) {
            if (game->Terminating()) return;
            ++game->displayed;
            BanditsLeftID = 42;
            game->lastCounterValue = TeamMemberLimit * 3; ++game->counterUpdates;
        }
        while (!ParentClass->WhisperSpawned) {
            game->NewScriptFrame();
            if (game->Terminating()) return;
            Team& team = ParentClass->Teams[TeamID];
            if (TeamID == 0) {
                if (game->Terminating()) return;
                if (team.Timer == 0 && team.MemberCount < TeamMemberLimit) {
                    if (game->Terminating()) return;
                    team.Timer = TeamRespawnTime;
                    long members = team.MemberCount;
                    bool atMe = game->heroFar && !game->onScreen;
                    if (game->Terminating()) return;
                    for (long i = 0; i < TeamMemberLimit - members; i++) {
                        if (game->Terminating()) return;
                        Create(team, false, atMe);
                        if (!atMe) ++game->repicks;
                    }
                    if (game->Terminating()) return;
                }
            } else {
                if (game->Terminating()) return;
                if (TeamID == ParentClass->HeroTeam) {
                    if (game->Terminating()) return;
                    game->lastCounterValue = (2 - ParentClass->BanditWavesSpawned) * TeamMemberLimit + team.MemberCount;
                    ++game->counterUpdates;
                }
                if (team.MemberCount == 0) {
                    if (game->Terminating()) return;
                    game->lastPause = reinforcementDelay;
                    ParentClass->BanditWavesSpawned++;
                    if (TeamID != ParentClass->HeroTeam) {
                        if (game->Terminating()) return;
                        if (game->guardAlive && game->guardNearHero) {
                            if (game->Terminating()) return;
                            ++game->comments;
                        }
                    }
                    if (ParentClass->BanditWavesSpawned == 3) {
                        if (game->Terminating()) return;
                        if (TeamID == ParentClass->HeroTeam) {
                            if (game->Terminating()) return;
                            ParentClass->MissionFailed = 3;
                        }
                        return;
                    }
                    if (game->Terminating()) return;
                    long members = team.MemberCount;
                    for (long i = 0; i < TeamMemberLimit - members; i++) {
                        if (game->Terminating()) return;
                        bool archer = (i == 1 && ParentClass->HeroTeam == 0);
                        if (game->Terminating()) return;
                        Create(team, archer, true);
                    }
                    if (game->Terminating()) return;
                }
            }
        }
        if (game->Terminating()) return;
        ++game->removed;
    }
};

static bool fail(const char* what) { std::printf("00dcd350_TEST FAIL (%s)\n", what); return false; }

static void setup(Game& g, Parent& p) {
    std::memset(&g, 0, sizeof g); std::memset(&p, 0, sizeof p);
    g.parent = &p; g.terminateAtFrame = -1; g.whisperAtFrame = -1; g.introAtFrame = 2;
}

int main()
{
    Game g; Parent p;
    // 1. terminated while waiting for the introduction: nothing happens
    setup(g, p); g.terminateAtFrame = 1; g.introAtFrame = 5;
    { TeamSpawn s = { &g, &p, 0, 2, 10, 0, 0 }; s.Main(); }
    if (g.created || g.removed || p.Teams[0].Timer) return fail("terminate"), 1;
    // 2. guard team, hero far and Me off camera: two members at Me, then Whisper ends the loop and the counter is removed
    setup(g, p); g.heroFar = true; g.whisperAtFrame = 6;
    { TeamSpawn s = { &g, &p, 0, 2, 3, 0, 0 }; s.Main(); }
    if (g.created != 2 || g.atMe != 2 || g.repicks || g.attached != 2 || g.pauses != 2 || g.removed != 1) return fail("guard at me"), 1;
    // 3. guard team on camera: members at the other spawn point, one random re-pick per member
    setup(g, p); g.heroFar = true; g.onScreen = true; g.whisperAtFrame = 6;
    { TeamSpawn s = { &g, &p, 0, 3, 3, 0, 0 }; s.Main(); }
    if (g.created != 3 || g.atOther != 3 || g.repicks != 3) return fail("guard elsewhere"), 1;
    // 4. bandit team is the hero's: counter shown; three waves end the spawner with MissionFailed = 3, no counter removal
    setup(g, p); p.HeroTeam = 1; g.guardAlive = true; g.guardNearHero = true; g.banditsDie = true;
    { TeamSpawn s = { &g, &p, 1, 2, 3, 0, 7 }; s.Main();
      if (s.BanditsLeftID != 42 || !g.displayed) return fail("counter"), 1; }
    if (g.created != 4 || g.archers != 0) return fail("bandit waves spawned (hero team)"), 1;
    if (p.MissionFailed != 3 || g.comments != 0 || g.removed != 0) return fail("bandit waves (hero team)"), 1;
    // 5. bandit team against a guard hero: NEXT_WAVE comments, the second member of a wave is an archer
    setup(g, p); p.HeroTeam = 0; g.guardAlive = true; g.guardNearHero = true; g.banditsDie = true;
    { TeamSpawn s = { &g, &p, 1, 3, 3, 0, 7 }; s.Main(); }
    if (p.BanditWavesSpawned != 3 || g.comments != 3 || g.archers != 2 || p.MissionFailed != 0 || g.lastPause != 7) return fail("bandit waves (guard hero)"), 1;
    std::printf("00dcd350_TEST PASS\n");
    return 0;
}
