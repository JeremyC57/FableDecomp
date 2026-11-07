// Behaviour test for NScript::CV_ChickenKickingScript::CChickenMaster::Main (0x00E64FB0): a model of the talk rules -- the first talk
// plays the initial walk when the ghost fisherman is gone (else the initial-meeting line); in the game loop a yes with 50+ gold starts
// kicking (START_KICKING once, then START_KICKING_RETURN), takes 50 gold and creates the spectators once the ghost is known gone;
// a yes without the gold is NO_MONEY, a no is DONT_KICK.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[256]; bool HeroHasPlayed, KnowGhostHasGone, SpectatorsCreated, KickingChickens; long gold;
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void FirstTalk(bool ghostAlive)
    {
        if (!ghostAlive) { Ev("CS_CHICKING_INITIALWALK1"); KnowGhostHasGone = true; }
        else Ev("INITIAL_MEETING");
    }
    void Answer(long answer)
    {
        if (answer == 1) {
            if (gold >= 50) {
                if (HeroHasPlayed) Ev("START_KICKING_RETURN");
                else { Ev("START_KICKING"); HeroHasPlayed = true; }
                Ev("FADE");
                if (KnowGhostHasGone && !SpectatorsCreated) { Ev("SPECTATORS"); SpectatorsCreated = true; }
                gold -= 50; KickingChickens = true;
            }
            else { Ev("NO_MONEY"); KickingChickens = false; }
        }
        else { Ev("DONT_KICK"); KickingChickens = false; }
    }
};
int main()
{
    Sim s; s.log[0] = 0; s.HeroHasPlayed = s.KnowGhostHasGone = s.SpectatorsCreated = s.KickingChickens = false; s.gold = 120;
    s.FirstTalk(false); s.Answer(1); s.Answer(1); s.Answer(0); s.Answer(1);
    bool ok = !std::strcmp(s.log, "CS_CHICKING_INITIALWALK1;START_KICKING;FADE;SPECTATORS;START_KICKING_RETURN;FADE;DONT_KICK;NO_MONEY;")
        && s.gold == 20 && !s.KickingChickens && s.SpectatorsCreated;
    Sim t; t.log[0] = 0; t.HeroHasPlayed = t.KnowGhostHasGone = t.SpectatorsCreated = t.KickingChickens = false; t.gold = 50;
    t.FirstTalk(true); t.Answer(1);
    ok = ok && !std::strcmp(t.log, "INITIAL_MEETING;START_KICKING;FADE;") && t.gold == 0 && t.KickingChickens && !t.SpectatorsCreated;
    std::printf(ok ? "00e64fb0_TEST PASS\n" : "00e64fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
