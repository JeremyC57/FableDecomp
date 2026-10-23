// Behaviour test for CGuardToLeadTheWay::Main (0x00da7d30): wait for a minion attacker, fight it (heal 100 per hit) until it
// dies, say the hint once (unless already pointed), follow the hero, show the health bar, spawn GuardChatter, then wait for
// the mission to fail or succeed and remove Me.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int attackerAt; int attackerDiesAt; int hitEvery; int missionEndsAt;
    bool pointed; int hints; float health; bool following; long healthBar; int spawned; bool removed;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static bool AttackerAlive(const Sim& s) { return s.frame >= s.attackerAt && s.frame < s.attackerDiesAt; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (!AttackerAlive(s)) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    while (AttackerAlive(s)) {
        Frame(s);
        if (Term(s)) return;
        if (s.hitEvery && s.frame % s.hitEvery == 0) s.health += 100.0f;
    }
    if (Term(s)) return;
    if (!s.pointed) { ++s.hints; s.pointed = true; }
    s.following = true;
    s.healthBar = 42;
    ++s.spawned;
    while (!(s.frame >= s.missionEndsAt)) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    s.removed = true;
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 1000; a.attackerAt = 5; a.attackerDiesAt = 25; a.hitEvery = 5; a.missionEndsAt = 60; a.health = 50.0f;
    Sim b = a; b.pointed = true;            // hint already given
    Sim c = a; c.terminateAt = 15;          // terminates mid-fight
    Main(a); Main(b); Main(c);
    bool ok = a.hints == 1 && a.pointed && a.following && a.healthBar == 42 && a.spawned == 1 && a.removed && a.health == 450.0f
           && b.hints == 0 && b.removed
           && c.hints == 0 && !c.following && !c.removed && c.spawned == 0;
    std::printf(ok ? "00da7d30_TEST PASS\n" : "00da7d30_TEST FAIL\n");
    return ok ? 0 : 1;
}
