// Behaviour test for CHeroBed::Main (0x00d446d0): Me and every pallet, apprentice bed and slum bed become unusable and
// persistent; a terminating thread stops the sweep where it is.
#include <cstdio>
#include <cstring>
#include <vector>

struct Thing { bool usable; bool persistent; };
struct Sim { int calls; int terminateAfter; };
static bool Term(Sim& s) { return ++s.calls > s.terminateAfter; }
static void Main(Sim& s, Thing& me, std::vector<Thing>& a, std::vector<Thing>& b, std::vector<Thing>& c)
{
    me.usable = false; me.persistent = true;
    std::vector<Thing>* lists[3] = { &a, &b, &c };
    for (int l = 0; l < 3; ++l) {
        std::vector<Thing>& v = *lists[l];
        long count = (long)v.size();
        for (long i = 0; i < count; ++i) {
            if (Term(s)) return;
            v[i].usable = false;
            v[i].persistent = true;
        }
        if (Term(s)) return;
    }
}
static int Done(const std::vector<Thing>& v) { int n = 0; for (unsigned i = 0; i < v.size(); ++i) if (!v[i].usable && v[i].persistent) ++n; return n; }
int main()
{
    Thing t; t.usable = true; t.persistent = false;
    std::vector<Thing> a(3, t), b(2, t), c(4, t);
    Thing me = t;
    Sim s; std::memset(&s, 0, sizeof(s)); s.terminateAfter = 1000;
    Main(s, me, a, b, c);
    std::vector<Thing> a2(3, t), b2(2, t), c2(4, t);
    Thing me2 = t;
    Sim s2; std::memset(&s2, 0, sizeof(s2)); s2.terminateAfter = 5;   // stops after 5 checks: pallets done, 1 apprentice bed
    Main(s2, me2, a2, b2, c2);
    bool ok = !me.usable && me.persistent && Done(a) == 3 && Done(b) == 2 && Done(c) == 4
           && !me2.usable && Done(a2) == 3 && Done(b2) == 1 && Done(c2) == 0;
    std::printf(ok ? "00d446d0_TEST PASS\n" : "00d446d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
