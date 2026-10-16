#include <cstdio>
// Behaviour model of CTCNymph::OnInitialActivate (0x0080FD90): the borrowed def reference is returned, the state reset
// and the spline given five control points.
struct Def { long ref; };
struct Nymph {
    const Def* sub; bool canAttack, ready, summon, limbo, flying, spline, hit; unsigned long tick; long fade, total; int points;
    void OnInitialActivate(Def* ownerDef)
    {
        Def* d = ownerDef;
        if (d) ++d->ref;
        sub = d;
        if (d && --d->ref == 0) { }
        canAttack = true; ready = false; summon = false; tick = 0; limbo = false; flying = false; spline = false; hit = false; fade = 0; total = 0;
        points = 5;
    }
};
int main()
{
    Def d = { 1 };
    Nymph n = { 0, false, true, true, true, true, true, true, 9, 9, 9, 0 };
    n.OnInitialActivate(&d);
    if (d.ref != 1 || n.sub != &d || !n.canAttack || n.ready || n.summon || n.tick || n.limbo || n.flying || n.spline || n.hit || n.fade || n.total || n.points != 5) { std::printf("0080fd90_TEST FAIL\n"); return 1; }
    std::printf("0080fd90_TEST PASS\n");
    return 0;
}