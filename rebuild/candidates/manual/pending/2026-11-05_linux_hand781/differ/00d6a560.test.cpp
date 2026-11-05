// Behaviour test for NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Main (0x00D6A560): the yell picked by the
// mission timer (>= 90 CHARGE, >= 30 HURRY_MILD, else HURRY_URGENT), yelling only once until reset, walking to the bandit
// exit point and switching to follow-the-hero once close, the hero hitting it three times turning it on the hero (and ending
// the script), the assault-guard attack phase once the enemies are gone, and the final follow-the-hero when the state is 6.
#include <cstdio>
#include <cstring>

enum { LINE_NONE, LINE_CHARGE, LINE_MILD, LINE_URGENT, LINE_FOLLOWING };
struct Sim {
    int frame; int terminateAt; int timer; bool enemiesDead; int enemiesBackAt; int stateSixAt; int distance; int hitFrames[4];
    bool shouldFollow; bool yelled; long heroHitMe; int lastLine; int lines; bool attackedHero; bool followedHeroAtEnd;
    bool guardAlive; int guardDiesAt; int guardAttacks;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.distance > 0) --s.distance; if (s.frame >= s.enemiesBackAt) s.enemiesDead = false;
                            if (s.frame >= s.guardDiesAt) s.guardAlive = false; }
static int State(const Sim& s) { return s.frame >= s.stateSixAt ? 6 : 1; }
static bool HitByHero(const Sim& s) { for (int i = 0; i < 4; ++i) if (s.hitFrames[i] == s.frame) return true; return false; }
static void Yell(Sim& s, int line) { s.lastLine = line; ++s.lines; }
static void Main(Sim& s)
{
    s.shouldFollow = false;
    while (State(s) != 6) {
        Frame(s); if (Term(s)) return;
        while (s.enemiesDead) {
            Frame(s); if (Term(s)) return;
            if (!s.yelled) {
                if (s.timer >= 90) Yell(s, LINE_CHARGE);
                else if (s.timer >= 30) Yell(s, LINE_MILD);
                else Yell(s, LINE_URGENT);
                s.yelled = true;
            }
            if (s.shouldFollow) {
                if (!s.yelled) { Yell(s, LINE_FOLLOWING); s.yelled = true; }
                if (Term(s)) return;
            } else if (State(s) != 6) {
                while (s.enemiesDead && !s.shouldFollow && s.distance > 8) {
                    Frame(s); if (Term(s)) return;
                    if (HitByHero(s)) ++s.heroHitMe;
                    if (s.heroHitMe >= 3) { s.attackedHero = true; return; }
                }
                if (s.distance <= 8) { s.shouldFollow = true; s.yelled = false; }
            }
        }
        if (Term(s)) return;
        if (s.guardAlive) {
            ++s.guardAttacks;
            while (s.guardAlive) { Frame(s); if (Term(s)) return; }
        }
        if (Term(s)) return;
    }
    s.followedHeroAtEnd = true;
}
int main()
{
    Sim base; std::memset(&base, 0, sizeof(base));
    base.terminateAt = 200; base.timer = 120; base.enemiesDead = true; base.enemiesBackAt = 40; base.stateSixAt = 60;
    base.distance = 20; base.guardAlive = true; base.guardDiesAt = 50; for (int i = 0; i < 4; ++i) base.hitFrames[i] = -1;
    Sim a = base; Main(a);                                            // charge yell, then follow (yell reset), guard, end
    Sim b = base; b.timer = 45; Main(b);
    Sim c = base; c.timer = 10; Main(c);
    Sim d = base; d.hitFrames[0] = 3; d.hitFrames[1] = 4; d.hitFrames[2] = 5; Main(d);   // three hits: turns on the hero
    Sim e = base; e.terminateAt = 45; Main(e);                       // terminated during the guard fight
    bool ok = a.lines == 2 && !a.attackedHero && a.followedHeroAtEnd && a.guardAttacks == 1 && a.shouldFollow
           && b.lines == 2 && b.followedHeroAtEnd
           && c.lines == 2 && c.followedHeroAtEnd
           && d.attackedHero && d.heroHitMe == 3 && !d.followedHeroAtEnd && d.lines == 1 && d.lastLine == LINE_CHARGE
           && !e.followedHeroAtEnd && e.guardAttacks == 1;
    Sim f = base; f.timer = 45; f.enemiesBackAt = 3; Main(f);         // one MILD yell only, no follow
    Sim g = base; g.timer = 10; g.enemiesBackAt = 3; Main(g);         // one URGENT yell only
    ok = ok && f.lines == 1 && f.lastLine == LINE_MILD && g.lines == 1 && g.lastLine == LINE_URGENT;
    std::printf(ok ? "00d6a560_TEST PASS\n" : "00d6a560_TEST FAIL\n");
    return ok ? 0 : 1;
}
