// Behaviour test for CAIStateGroup_WhisperRetreat::UseWill (0x0096AE50), g++ model of the retail logic.
#include <cstdio>
struct Thing { bool dead; unsigned typeFlags; bool physics; };
struct Group { Thing* target; int casts; Thing* castTarget; Thing* followTarget; int posReads; bool castWait; };
static void UseWill(Group& g)
{
    Thing* t = g.target;
    if (t && !t->dead && (t->typeFlags & 8)) {
        if (t->physics) {
            g.posReads += 2;
            g.followTarget = t;
            ++g.casts; g.castTarget = t; g.castWait = false;
        }
    }
}
int main()
{
    Thing live = { false, 8, true }, nophys = { false, 8, false }, dead = { true, 8, true }, other = { false, 4, true };
    Group a = { &live, 0, 0, 0, 0, true };
    UseWill(a);
    int ok = a.casts == 1 && a.castTarget == &live && a.followTarget == &live && a.posReads == 2 && !a.castWait;
    Group b = { &nophys, 0, 0, 0, 0, true }; UseWill(b); ok &= b.casts == 0 && b.posReads == 0;
    Group c = { &dead, 0, 0, 0, 0, true }; UseWill(c); ok &= c.casts == 0;
    Group d = { &other, 0, 0, 0, 0, true }; UseWill(d); ok &= d.casts == 0;
    Group e = { 0, 0, 0, 0, 0, true }; UseWill(e); ok &= e.casts == 0;
    std::printf(ok ? "0096ae50_TEST PASS\n" : "0096ae50_TEST FAIL\n");
    return ok ? 0 : 1;
}