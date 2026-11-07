// Behaviour test for CDF_DressAsBandit::Main (0x00E71F10): the door guard waits to be talked to; talked to, it plays its speech movie;
// a hero wearing all five bandit-camp clothing items (shirt, trousers, hat, boots, gloves) gets the opening speech (if the guard is
// alive), the fade-out door opening and the region exit re-enabled, ending the script; otherwise the intro speech (if alive).
#include <cstdio>

enum { SHIRT = 1, TROUSERS = 2, HAT = 4, BOOTS = 8, GLOVES = 16, ALL = 31 };
struct Guard {
    long Health, Movies, Intros, Openings, DoorsOpened;
    bool ExitActive, Finished;
    void Talk(long clothing)
    {
        if (Finished) return;
        ++Movies;
        if ((clothing & ALL) == ALL) {
            ++Movies;
            if (Health > 0) ++Openings;
            ++DoorsOpened;
            ExitActive = true;
            Finished = true;
            return;
        }
        if (Health > 0) ++Intros;
    }
};

int main()
{
    Guard g = { 10, 0, 0, 0, 0, false, false };
    bool ok = true;
    g.Talk(SHIRT | HAT);                         ok = ok && g.Intros == 1 && g.Movies == 1 && !g.ExitActive;
    g.Talk(SHIRT | TROUSERS | HAT | BOOTS);      ok = ok && g.Intros == 2 && !g.Finished;
    g.Health = 0; g.Talk(0);                     ok = ok && g.Intros == 2 && g.Movies == 3;
    g.Health = 5; g.Talk(ALL);                   ok = ok && g.Openings == 1 && g.DoorsOpened == 1 && g.ExitActive && g.Finished;
    g.Talk(ALL);                                 ok = ok && g.DoorsOpened == 1 && g.Movies == 5;
    if (!ok) { std::printf("00e71f10_TEST FAIL\n"); return 1; }
    std::printf("00e71f10_TEST PASS\n");
    return 0;
}