// Behaviour test for CFinalTabletSarcophagus::Main (0x00cffb60): nothing when tablet A is already taken; otherwise raise the
// forcefield, wait for 3 tablets, wait for the hero to come near, play the summoner cutscene once, wait for the summoners to
// die, open the sarcophagus (tablet count +1, GotA), speak two lines and idle until termination.
#include <cstdio>
#include <cstring>

struct Quest { long tablets; bool spawned; long summoners; bool gotA; };
struct Sim {
    Quest q; int frame; int terminateAt; int tabletsAt; int heroNearAt; int summonersDeadAt;
    int forcefields; int cutscenes; int lines; bool expression; bool usable;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s)
{
    ++s.frame;
    if (s.frame == s.tabletsAt) s.q.tablets = 3;
    if (s.frame == s.summonersDeadAt) s.q.summoners = 0;
}
static void Main(Sim& s)
{
    s.usable = false;
    if (s.q.gotA) return;
    ++s.forcefields;
    while (s.q.tablets <= 2) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    while (!(s.frame >= s.heroNearAt)) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    if (!s.q.spawned) { s.q.spawned = true; ++s.cutscenes; ++s.forcefields; }
    while (s.q.summoners != 0) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    ++s.cutscenes;
    ++s.q.tablets;
    s.expression = true;
    s.q.gotA = true;
    s.lines += 2;
    do Frame(s); while (!Term(s));
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.usable = true; a.terminateAt = 100; a.tabletsAt = 5; a.heroNearAt = 10; a.summonersDeadAt = 20; a.q.tablets = 2; a.q.summoners = 4;
    Sim b = a; b.q.gotA = true;              // already done
    Sim c = a; c.q.spawned = true;           // summoners cutscene already played
    Sim d = a; d.terminateAt = 15;           // terminates while the summoners live
    Main(a); Main(b); Main(c); Main(d);
    bool ok = a.forcefields == 2 && a.cutscenes == 2 && a.q.tablets == 4 && a.q.gotA && a.lines == 2 && a.frame == 100 && !a.usable
           && b.forcefields == 0 && b.frame == 0
           && c.forcefields == 1 && c.cutscenes == 1 && c.q.gotA
           && d.cutscenes == 1 && !d.q.gotA && d.lines == 0;
    std::printf(ok ? "00cffb60_TEST PASS\n" : "00cffb60_TEST FAIL\n");
    return ok ? 0 : 1;
}
