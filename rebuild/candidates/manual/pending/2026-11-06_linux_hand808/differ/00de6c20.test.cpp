// Behaviour test for NScript::CQ_RansomVictimScript::CVictim::Main (0x00DE6C20): until rescued each talk gives the help line; after
// the rescue (thanks cut-scene when the ransommer died) the victim follows the hero and per frame: leaving the cave sets GotOutOfCave,
// talks give the cave thanks / waiting-for-way-cleared (bandits or creatures) / a nagging line, a hero hit (timer idle) gives the
// on-hero-hit line and re-allies, near the hero the on-hit lines run 10, 20, .. and wrap to 10 when the text entry does not exist,
// a near enemy starts waiting and a dead or distant one resumes following; the parent's thanks cut-scene stops him for good.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Victim {
    int Frames, TerminateAt, RescueAt, Timer, OnHitLine, MaxLine, TalkMask, HeroHitMask, EnemyNear, EnemyGone, ThanksAt;
    bool RansommerDied, GotOutOfCave, Told, Waiting, InCave, Following;
    bool T() const { return Frames >= TerminateAt; }
    void NewScriptFrame() { ++Frames; if (Timer > 0) --Timer; }
    bool Talked() const { return Frames < 31 && ((TalkMask >> Frames) & 1); }
    bool HeroHit() const { return Frames < 31 && ((HeroHitMask >> Frames) & 1); }
    void Main()
    {
        NewScriptFrame(); if (T()) return;
        while (Frames < RescueAt) {
            NewScriptFrame(); if (T()) return;
            if (Talked()) Log("HELP_10");
        }
        if (T()) return;
        if (RansommerDied) { Log("CS_RANSOM_THANKS"); GotOutOfCave = true; }
        Following = true;
        while (!T()) {
            if (!GotOutOfCave && !InCave) GotOutOfCave = true;
            if (Talked()) {
                if (InCave && !GotOutOfCave) { Log("THANKS"); GotOutOfCave = true; }
                else if (Waiting) Log(RansommerDied ? "WAITING_BANDITS" : "WAITING_CREATURES");
                else Log("NAGGING");
            }
            if (HeroHit() && Timer == 0) { Log("ON_HERO_HIT"); Timer = 5; Log("ally"); }
            if (!Told && Frames == 3) {
                if (Timer == 0) {
                    char line[16];
                    if (OnHitLine > MaxLine) OnHitLine = 10;
                    std::sprintf(line, "ON_HIT_%d", OnHitLine); Log(line);
                    Timer = 5; OnHitLine += 10;
                }
                Told = true;
            }
            if (!Waiting && Frames == EnemyNear) { Waiting = true; Following = false; Log("wait"); }
            if (Frames == ThanksAt) { Log("stop"); while (!T()) NewScriptFrame(); return; }
            if (Waiting && Frames == EnemyGone) { Waiting = false; Following = true; Log("follow"); }
            NewScriptFrame();
        }
    }
};
int main()
{
    Victim a = { 0, 14, 3, 0, 30, 20, (1 << 2) | (1 << 6) | (1 << 9), (1 << 9), 7, 10, -1, true, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "HELP_10;CS_RANSOM_THANKS;ON_HIT_10;NAGGING;wait;WAITING_BANDITS;ON_HERO_HIT;ally;follow;")
        && a.Following && !a.Waiting && a.OnHitLine == 20;
    g_log[0] = 0;
    Victim b = { 0, 20, 1, 0, 10, 20, (1 << 4), 0, 5, -1, 8, false, false, false, false, true, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "ON_HIT_10;THANKS;wait;stop;") && b.Frames == 20 && b.Waiting;
    std::printf(ok ? "00de6c20_TEST PASS\n" : "00de6c20_TEST FAIL\n");
    return ok ? 0 : 1;
}
