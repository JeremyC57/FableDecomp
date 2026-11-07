// Behaviour test for NScript::CQ_HeroSoulsBriarScript::CBriarRose::Main (0x00D82640): the soul-2 intro plays once (line 10B
// when Thunder was killed), talks give ONTALK_10/_20 the first time and the sequential line afterwards, hits away from the
// shrine make her an ally, a hit within 13 m ends the loop (ONHITTWICE_10, quest start, boss set-up), then the boss taunts:
// TAUNT_10 in boss phase 1, the numbered taunts every 35 frames from 20 upwards (back to 20 when the text entry is missing).
#include <cstdio>
#include <cstring>
struct Sim {
    char log[2048]; int frame, termFrame; bool PlayedBriarCS, KilledThunder, FirstSpeakAndWaitDone;
    int talk1, talk2, hitFar, hitNear, phaseFrame, lastTaunt; float health; int deliveredSoul;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void EvN(const char* e, long n) { char b[64]; std::sprintf(b, "%s%ld", e, n); Ev(b); }
    bool TextExists(long n) const { return n <= lastTaunt; }
    void Main()
    {
        if (Term()) return;
        if (!PlayedBriarCS) { Ev(KilledThunder ? "CS10B" : "CS10"); PlayedBriarCS = true; ++deliveredSoul; }
        if (Term()) return;
        for (;;) {
            if (frame == talk1 || frame == talk2) {
                if (!FirstSpeakAndWaitDone) { Ev("T10"); Ev("T20"); FirstSpeakAndWaitDone = true; }
                else Ev("TSEQ");
            }
            if (frame == hitNear) break;
            if (frame == hitFar) Ev("ALLY");
            ++frame; if (Term()) return;
        }
        Ev("ALLY"); Ev("HIT2"); Ev("QUEST");
        long timer = 15, number = 20; bool taunted = false;
        while (health > 5.0f) {
            ++frame; if (timer > 0) --timer; health -= 1.0f;
            if (Term()) return;
            if (!taunted && frame >= phaseFrame) { Ev("TAUNT10"); timer = 30; taunted = true; }
            if (timer == 0) { if (!TextExists(number)) number = 20; EvN("TAUNT", number); timer = 35; number += 10; }
        }
        while (!Term()) ++frame;
    }
};
static void Make(Sim& s)
{
    std::memset(&s, 0, sizeof s); s.termFrame = 400; s.talk1 = 2; s.talk2 = 4; s.hitFar = 3; s.hitNear = 6;
    s.phaseFrame = 1000; s.lastTaunt = 30; s.health = 100.0f;
}
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = std::strncmp(a.log, "CS10;T10;T20;ALLY;TSEQ;ALLY;HIT2;QUEST;TAUNT20;TAUNT30;TAUNT20;", 61) == 0 && a.deliveredSoul == 1 && a.frame == 400;
    Sim b; Make(b); b.KilledThunder = true; b.PlayedBriarCS = false; b.phaseFrame = 10; b.health = 20.0f; b.Main();
    ok = ok && std::strcmp(b.log, "CS10B;T10;T20;ALLY;TSEQ;ALLY;HIT2;QUEST;TAUNT10;") == 0;
    Sim c; Make(c); c.PlayedBriarCS = true; c.termFrame = 5; c.Main();
    ok = ok && std::strcmp(c.log, "T10;T20;ALLY;TSEQ;") == 0 && c.deliveredSoul == 0;
    std::printf(ok ? "00d82640_TEST PASS\n" : "00d82640_TEST FAIL\n");
    return ok ? 0 : 1;
}
