#include <cstdio>
// Behaviour model of CModeDragonStrafeBurninate::EndBeamEffect (0x008D7BB0): a live beam effect is killed and the
// pointer cleared; a dead or missing one is left alone.
struct Thing { unsigned char flags; int killed; };
struct Mode { Thing* beam; };
static void EndBeamEffect(Mode& m)
{
    if (m.beam != 0 && !(m.beam->flags & 1)) {
        m.beam->killed++;
        m.beam = 0;
    }
}
int main()
{
    Thing live = { 0, 0 }, dead = { 1, 0 };
    Mode a = { &live }, b = { &dead }, c = { 0 };
    EndBeamEffect(a); EndBeamEffect(b); EndBeamEffect(c);
    if (a.beam != 0 || live.killed != 1 || b.beam != &dead || dead.killed != 0 || c.beam != 0) { std::printf("008d7bb0_TEST FAIL\n"); return 1; }
    std::printf("008d7bb0_TEST PASS\n");
    return 0;
}
