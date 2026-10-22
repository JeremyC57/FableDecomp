// Behaviour test for CAttackingAssassin::Main (0x00E383A0): scripting retried until granted, hero made the enemy and followed, the first run waits for the shout distance then plays a taunt chosen by rand() % count and stops following; then frames run until the thread terminates.
#include <cstdio>
#include <cstring>
struct CSim {
    int frame; int grantAt; int terminateAt; int closeAt; int following; int taunt; int enemy; int stops;
    int rnd;
};
struct CAssassin {
    CSim* S; bool HasTaunted; const int* Taunts; int TauntCount;
    bool Terminating() const { return S->frame >= S->terminateAt; }
    void NewFrame() { ++S->frame; }
    void Main()
    {
        NewFrame();
        if (Terminating()) return;
        while (S->frame < S->grantAt) { NewFrame(); if (Terminating()) return; }
        if (Terminating()) return;
        S->enemy = 1;
        S->following = 1;
        if (!HasTaunted) {
            if (Terminating()) return;
            while (S->frame < S->closeAt) { NewFrame(); if (Terminating()) return; }
            if (Terminating()) return;
            unsigned long count = (unsigned long)TauntCount;
            unsigned long index = (unsigned long)S->rnd % count;
            S->taunt = Taunts[index];
            HasTaunted = true;
            S->following = 0;
            ++S->stops;
        }
        do { NewFrame(); } while (!Terminating());
    }
};
int main()
{
    static const int taunts[3] = { 11, 22, 33 };
    CSim s; std::memset(&s, 0, sizeof(s)); s.grantAt = 3; s.terminateAt = 20; s.closeAt = 6; s.rnd = 7;
    CAssassin a; a.S = &s; a.HasTaunted = false; a.Taunts = taunts; a.TauntCount = 3;
    a.Main();
    bool ok = s.enemy == 1 && s.taunt == 22 && a.HasTaunted && s.following == 0 && s.stops == 1 && s.frame == 20;
    CSim s2; std::memset(&s2, 0, sizeof(s2)); s2.grantAt = 1; s2.terminateAt = 5; s2.closeAt = 0;
    a.S = &s2;
    a.Main();
    ok = ok && s2.taunt == 0 && s2.following == 1 && s2.stops == 0 && s2.frame == 5;
    if (!ok) { std::printf("00e383a0_TEST FAIL\n"); return 1; }
    std::printf("00e383a0_TEST PASS\n");
    return 0;
}