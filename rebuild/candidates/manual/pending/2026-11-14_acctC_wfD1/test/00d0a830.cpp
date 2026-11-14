// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main (0x00D0A830): while the fight is not over, record the
// king's health and taunt once below three quarters and once below half of the starting health (each only when the 8-second taunt
// timer has run out); then lie in the defeated pose begging every ten seconds (lines 10, 20, 30, 40, 50, then 40 / 50 alternating);
// when the hero hits him (a heal spell does not count) play the "over" line in a paused movie (only while he has health), flag
// TwinBladeAttacked with AngryBanditNeeded 4, make him a killable enemy and track his health on a bar until he dies. A thread
// terminating inside the speech wait leaves at once: pause off, movie ended, BegTimer, seh_me and TauntTimer released in that order
// (the row's `goto Terminate` path), and nothing after the speech runs. A state model of the row's statements.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[1024]; int frame, termFrame, hitFrame, healFrame, speechFrames; long health, kingHealth, startHealth;
    bool allOver, attacked, killable; long angry; int bars, overFrame;
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    bool Term() const { return frame >= termFrame; }
    void Frame() { ++frame; if (frame == overFrame) allOver = true; if (frame > 2 && health > 30) health -= 10; }
    void Cleanup(bool movie) { if (movie) Ev("UNPAUSE"), Ev("ENDMOVIE"); Ev("~BEG"); Ev("~SEH"); Ev("~TAUNT"); }
    void Main()
    {
        Frame(); if (Term()) return;
        startHealth = health; bool first = false, second = false; long tauntTimer = 0;
        while (!allOver) {
            Frame(); if (tauntTimer > 0) --tauntTimer; if (Term()) { Ev("~TAUNT"); return; }
            kingHealth = health;
            if (kingHealth < startHealth * 3 / 4 && !first && tauntTimer <= 0) { Ev("TAUNT_10"); first = true; tauntTimer = 8; }
            if (kingHealth < startHealth / 2 && !second && tauntTimer <= 0) { Ev("TAUNT_20"); second = true; tauntTimer = 8; }
        }
        static const char* const beg[5] = { "BEG_10", "BEG_20", "BEG_30", "BEG_40", "BEG_50" };
        static const long next[5] = { 1, 2, 3, 4, 3 };
        long begCount = 0, begTimer = 0;
        for (;; Frame(), (begTimer > 0 ? --begTimer : 0)) {
            if (Term()) { Cleanup(false); return; }
            if (begTimer <= 0) { Ev(beg[begCount]); begCount = next[begCount]; begTimer = 2; }
            if (frame == hitFrame && frame != healFrame) {
                Ev("MOVIE"); Ev("PAUSE");
                if (health > 0) {
                    Ev("OVER");
                    for (int i = 0; i < speechFrames; ++i) { Frame(); if (Term()) { Cleanup(true); return; } }
                }
                attacked = true; angry = 4; killable = true; Ev("UNPAUSE"); Ev("ENDMOVIE");
                while (health > 0) { Frame(); if (Term()) { Cleanup(false); return; } health -= 10; ++bars; }
            }
        }
    }
};
static void Make(Sim& s) { std::memset(&s, 0, sizeof s); s.termFrame = 1000; s.hitFrame = 30; s.healFrame = -1; s.speechFrames = 2; s.health = 100; s.overFrame = 20; }
int main()
{
    Sim a; Make(a); a.termFrame = 60; a.Main();
    bool ok = std::strstr(a.log, "TAUNT_10;TAUNT_20;BEG_10;BEG_20;BEG_30;BEG_40;BEG_50;BEG_40;MOVIE;PAUSE;OVER;UNPAUSE;ENDMOVIE;") == a.log
        && std::strstr(a.log, "OVER;UNPAUSE;ENDMOVIE;") != 0 && a.attacked && a.angry == 4 && a.killable && a.bars == 3 && a.kingHealth == 30;
    Sim b; Make(b); b.termFrame = 31; b.Main();   // terminates inside the speech wait
    const char* t = std::strstr(b.log, "MOVIE;PAUSE;OVER;UNPAUSE;ENDMOVIE;~BEG;~SEH;~TAUNT;");
    ok = ok && t != 0 && std::strlen(t) == std::strlen("MOVIE;PAUSE;OVER;UNPAUSE;ENDMOVIE;~BEG;~SEH;~TAUNT;") && !b.attacked && !b.killable;
    Sim c; Make(c); c.healFrame = 30; c.termFrame = 40; c.Main();   // a heal spell is not a hit
    ok = ok && std::strstr(c.log, "OVER") == 0 && !c.attacked;
    std::printf(ok ? "00d0a830_TEST PASS\n" : "00d0a830_TEST FAIL\n");
    return ok ? 0 : 1;
}
