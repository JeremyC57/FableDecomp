// Behaviour test for NScript::CV_ChickenKickingScript::CChickenMaster::Main (0x00E64FB0): a model of the talk rules. The first talk
// with the ghost fisherman gone: if the organiser is still walking and more than 6 m from his marker he stops, says QUICK_WALK, the
// screen fades out, he is teleported and the spectators are created (once); otherwise fade, spectators, teleport. BOTH arms then play
// CS_CHICKING_INTRO and fade back in (the row writes the two statements in each arm; retail merges them at 0x00E6743E). In the game
// loop a yes with 50+ gold starts kicking (START_KICKING once, then START_KICKING_RETURN), takes 50 gold and creates the spectators
// once the ghost is known gone; a yes without the gold is NO_MONEY, a no is DONT_KICK.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[512]; bool HeroHasPlayed, KnowGhostHasGone, SpectatorsCreated, KickingChickens; long gold;
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void Spectators() { if (KnowGhostHasGone && !SpectatorsCreated) { Ev("SPECTATORS"); SpectatorsCreated = true; } }
    void FirstTalk(bool walking, float distance)
    {
        if (walking && distance > 6.0f) {
            Ev("CLEAR"); Ev("QUICK_WALK"); Ev("FADE_OUT"); Ev("TELEPORT");
            Spectators();
            Ev("CS_CHICKING_INTRO"); Ev("FADE_IN");
        }
        else {
            Ev("FADE_OUT");
            Spectators();
            Ev("TELEPORT");
            Ev("CS_CHICKING_INTRO"); Ev("FADE_IN");
        }
    }
    void Answer(long answer)
    {
        if (answer == 1) {
            if (gold >= 50) {
                if (HeroHasPlayed) Ev("START_KICKING_RETURN");
                else { Ev("START_KICKING"); HeroHasPlayed = true; }
                Ev("FADE_OUT");
                Spectators();
                gold -= 50; KickingChickens = true;
            }
            else { Ev("NO_MONEY"); KickingChickens = false; }
        }
        else { Ev("DONT_KICK"); KickingChickens = false; }
    }
    void Reset(long g, bool ghostGone)
    {
        log[0] = 0; HeroHasPlayed = SpectatorsCreated = KickingChickens = false; KnowGhostHasGone = ghostGone; gold = g;
    }
};
int main()
{
    Sim s; s.Reset(120, true);
    s.FirstTalk(true, 9.0f); s.Answer(1); s.Answer(1); s.Answer(0); s.Answer(1);
    bool ok = !std::strcmp(s.log, "CLEAR;QUICK_WALK;FADE_OUT;TELEPORT;SPECTATORS;CS_CHICKING_INTRO;FADE_IN;"
                                  "START_KICKING;FADE_OUT;START_KICKING_RETURN;FADE_OUT;DONT_KICK;NO_MONEY;")
        && s.gold == 20 && !s.KickingChickens && s.SpectatorsCreated;
    Sim t; t.Reset(50, true);
    t.FirstTalk(true, 3.0f); t.Answer(1);
    ok = ok && !std::strcmp(t.log, "FADE_OUT;SPECTATORS;TELEPORT;CS_CHICKING_INTRO;FADE_IN;START_KICKING;FADE_OUT;")
        && t.gold == 0 && t.KickingChickens && t.SpectatorsCreated;
    Sim u; u.Reset(10, false);
    u.FirstTalk(false, 20.0f); u.Answer(1);
    ok = ok && !std::strcmp(u.log, "FADE_OUT;TELEPORT;CS_CHICKING_INTRO;FADE_IN;NO_MONEY;") && u.gold == 10 && !u.SpectatorsCreated;
    std::printf(ok ? "00e64fb0_TEST PASS\n" : "00e64fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}