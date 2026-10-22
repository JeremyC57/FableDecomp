// Behaviour test for CMC_Minion::Main (0x00D9F270): target Briar Rose when she is alive and rand() % 3 != 0, else the hero;
// then each frame retarget the hero when the hero hits Me (directly, or with a special ability other than 14) and the
// current target is not already the hero.
#include <cstdio>
#include <cstring>

enum { NOBODY = 0, HERO = 1, BRIAR = 2 };
struct Sim {
    int frame; int terminateAt; bool briarAlive; int rnd;
    bool hitDirect[16]; bool hitAnySpecial[16]; bool hitSpecial14[16];
    int gives; int lastGiven;
};

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static void Give(Sim& s, int who) { ++s.gives; s.lastGiven = who; }
static void Main(Sim& s)
{
    ++s.frame;
    if (Terminating(s)) return;
    int target = NOBODY;
    if (s.briarAlive && s.rnd % 3 != 0) {
        if (Terminating(s)) return;
        target = BRIAR;
    } else {
        if (Terminating(s)) return;
        target = HERO;
    }
    Give(s, target);
    for (;;) {
        if (Terminating(s)) return;
        int f = s.frame;
        bool hit = s.hitDirect[f] || (s.hitAnySpecial[f] && !s.hitSpecial14[f]);
        if (hit && target != HERO) {
            if (Terminating(s)) return;
            target = HERO;
            Give(s, HERO);
        }
        ++s.frame;
    }
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 10; a.briarAlive = true; a.rnd = 4;
    a.hitAnySpecial[3] = true; a.hitSpecial14[3] = true;   // ability 14 only: ignored
    a.hitDirect[5] = true;                                 // retarget the hero
    a.hitDirect[7] = true;                                 // already the hero: no give
    Main(a);
    bool ok = a.gives == 2 && a.lastGiven == HERO && a.frame == 10;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 4; b.briarAlive = true; b.rnd = 3;     // rand() % 3 == 0: hero straight away
    b.hitDirect[2] = true;
    Main(b);
    ok = ok && b.gives == 1 && b.lastGiven == HERO;

    Sim c; std::memset(&c, 0, sizeof(c));
    c.terminateAt = 1;                                     // terminating after the first frame
    Main(c);
    ok = ok && c.gives == 0;

    std::printf(ok ? "00d9f270_TEST PASS\n" : "00d9f270_TEST FAIL\n");
    return ok ? 0 : 1;
}
