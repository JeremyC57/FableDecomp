// Behaviour test for CSpectator::Main (0x00E998F0): the cheer-line pick (any of the ten lines, or only the last five
// once the crowd is jeering), the cheer-timer reload range, the alternating facing toggle and the three-hit jeer switch.
#include <cstdio>
#include <cstring>

struct Duel {
    bool cheerDue, jeering;
    long heroHits;
};
struct Spectator {
    Duel* parent;
    bool facingHero;
    int jeerLines, facedHero, facedThunder;
    long lastLine, lastTimer;
    // one cheer: the line index comes from the supplied random value
    void Cheer(int rnd)
    {
        static const char* lines[10] = {
            "TEXT_QST_044_HERO_DUEL_CHEERING_10", "TEXT_QST_044_HERO_DUEL_CHEERING_20",
            "TEXT_QST_044_HERO_DUEL_CHEERING_30", "TEXT_QST_044_HERO_DUEL_CHEERING_40",
            "TEXT_QST_044_HERO_DUEL_CHEERING_50", "TEXT_QST_044_HERO_DUEL_CHEERING_60",
            "TEXT_QST_044_HERO_DUEL_CHEERING_70", "TEXT_QST_044_HERO_DUEL_CHEERING_80",
            "TEXT_QST_044_HERO_DUEL_CHEERING_90", "TEXT_QST_044_HERO_DUEL_CHEERING_100"
        };
        parent->cheerDue = false;
        long line = parent->jeering ? rnd % 5 + 5 : rnd % 10;
        lastLine = line;
        (void)lines[line];
        if (facingHero) { ++facedHero; facingHero = false; }
        else { ++facedThunder; facingHero = true; }
        lastTimer = rnd % 60 + 2;
    }
    void Hit()
    {
        if (parent->jeering) return;
        ++parent->heroHits;
        if (parent->heroHits >= 3) { parent->jeering = true; ++jeerLines; }
    }
};

int main()
{
    int fail = 0;
    Duel d;
    std::memset(&d, 0, sizeof(d));
    Spectator s;
    std::memset(&s, 0, sizeof(s));
    s.parent = &d;
    d.cheerDue = true;
    s.Cheer(37);
    if (s.lastLine != 7 || s.lastTimer != 39 || !s.facingHero || s.facedThunder != 1 || d.cheerDue) fail = 1;
    s.Cheer(59);
    if (s.lastLine != 9 || s.lastTimer != 61 || s.facingHero || s.facedHero != 1) fail = 1;
    s.Hit(); s.Hit();
    if (d.jeering || d.heroHits != 2) fail = 1;
    s.Hit();
    if (!d.jeering || s.jeerLines != 1) fail = 1;
    s.Hit();   // no more counting once jeering
    if (d.heroHits != 3 || s.jeerLines != 1) fail = 1;
    s.Cheer(3);
    if (s.lastLine != 8) fail = 1;   // jeering: lines 5..9 only
    s.Cheer(10);
    if (s.lastLine != 5 || s.lastTimer != 12) fail = 1;

    if (fail) { std::printf("00e998f0_TEST FAIL\n"); return 1; }
    std::printf("00e998f0_TEST PASS\n");
    return 0;
}
