#include <cstdio>
// Behaviour model of CTCKrakenTentacle::RemoveObstruction (0x007F0810): kill a live obstruction, then always clear the
// pointer and the queued strike tick.
struct Thing { unsigned char flags; int killed; };
struct Tentacle { Thing* obstruction; long queuedStrikeTick; };
static void RemoveObstruction(Tentacle& t)
{
    if (t.obstruction != 0 && !(t.obstruction->flags & 1))
        t.obstruction->killed++;
    t.obstruction = 0;
    t.queuedStrikeTick = 0;
}
int main()
{
    Thing live = { 0, 0 }, dead = { 1, 0 };
    Tentacle a = { &live, 7 }, b = { &dead, 9 };
    RemoveObstruction(a); RemoveObstruction(b);
    if (a.obstruction || b.obstruction || a.queuedStrikeTick || b.queuedStrikeTick || live.killed != 1 || dead.killed != 0) { std::printf("007f0810_TEST FAIL\n"); return 1; }
    std::printf("007f0810_TEST PASS\n");
    return 0;
}
