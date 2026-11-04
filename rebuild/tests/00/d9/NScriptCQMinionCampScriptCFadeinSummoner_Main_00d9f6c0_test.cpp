// Behaviour test for CFadeinSummoner::Main (0x00D9F6C0): stage gating, special-idle timers, effect re-creation and ally-on-hit.
#include <cstdio>

struct Sim {
    int frame, stage2At, stage4At, hitAt, effectDiesAt;
    int idles, breathes, effects, allies, removed;
    long timers[3];
};
static Sim s;
struct Timer {
    int id;
    void SetTime(long t) { s.timers[id] = t; }
    long GetTime() const { return s.timers[id]; }
};
struct Summoner {
    long Stage() const { return s.frame >= s.stage4At ? 4 : (s.frame >= s.stage2At ? 2 : 1); }
    void NewScriptFrame() { ++s.frame; for (int i = 0; i < 3; ++i) if (s.timers[i] > 0) --s.timers[i]; }
    bool IsActiveThreadTerminating() const { return s.frame > 200; }
    void Main();
};
void Summoner::Main()
{
    bool effectAlive = true;
    while (Stage() != 2) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    ++s.breathes;
    Timer idleTimer = { 0 }; idleTimer.SetTime(25);
    Timer effectTimer = { 1 }; effectTimer.SetTime(12);
    Timer specialTimer = { 2 }; specialTimer.SetTime(0);
    bool playingSpecial = false;
    while (Stage() == 2 || Stage() == 3) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (s.frame == s.effectDiesAt) effectAlive = false;
        if (s.frame == s.hitAt) ++s.allies;
        if (effectTimer.GetTime() == 0 && !effectAlive) {
            ++s.effects; effectAlive = true;
            effectTimer.SetTime(8);
        }
        if (idleTimer.GetTime() == 0) {
            ++s.idles;
            idleTimer.SetTime(20);
            playingSpecial = true;
            specialTimer.SetTime(4);
        }
        if (playingSpecial && specialTimer.GetTime() == 0) {
            ++s.breathes;
            playingSpecial = false;
        }
    }
    ++s.removed;
}
int main()
{
    Sim z = { 0, 5, 80, 10, 20, 0, 0, 0, 0, 0, { 0, 0, 0 } };
    s = z;
    Summoner m; m.Main();
    // stage 2 from frame 5 to 79: idles at frames 30, 50, 70 -> 3 idles, 1 + 3 breathes; effect re-created once; one ally.
    if (s.idles != 3 || s.breathes != 4 || s.effects != 1 || s.allies != 1 || s.removed != 1) {
        std::printf("00d9f6c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00d9f6c0_TEST PASS\n");
    return 0;
}