// Behaviour test for CHSB_MagicBarrier::Main (0x00d81c50): raise two forcefield effects; while Briar lives, re-raise both
// once both have gone; when Briar dies remove them, play the two "off" effects and remove Me.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int briarDiesAt; int effectsDieAt;
    bool e1, e2; int raised; int offs; int facings; bool meRemoved;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.frame == s.effectsDieAt) { s.e1 = false; s.e2 = false; } }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    s.e1 = true; s.e2 = true; s.raised += 2;
    if (s.e1 && s.e2) { s.facings += 2; }
    while (!(s.frame >= s.briarDiesAt)) {
        Frame(s);
        if (Term(s)) return;
        if (!s.e1 && !s.e2) { s.e1 = s.e2 = true; s.raised += 2; s.facings += 2; }
    }
    if (Term(s)) return;
    s.e1 = s.e2 = false;
    s.offs += 2; s.facings += 2;
    s.meRemoved = true;
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 1000; a.briarDiesAt = 50; a.effectsDieAt = 20;
    Sim b = a; b.effectsDieAt = 0;          // effects never vanish
    Sim c = a; c.terminateAt = 30;          // terminated before Briar dies
    Main(a); Main(b); Main(c);
    bool ok = a.raised == 4 && a.offs == 2 && a.facings == 6 && a.meRemoved
           && b.raised == 2 && b.facings == 4 && b.meRemoved
           && c.raised == 4 && c.offs == 0 && !c.meRemoved;
    std::printf(ok ? "00d81c50_TEST PASS\n" : "00d81c50_TEST FAIL\n");
    return ok ? 0 : 1;
}
