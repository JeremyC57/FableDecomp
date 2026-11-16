// Behaviour test for the bandit-toll placement helper (0x00e4e0e0): Me is teleported to the blockway when it is nearer to the
// hero than Me (else to M_TollBandit), the near or far blockade markers are used accordingly, and three BlockingBandit
// creatures (archer, grunt, archer) are spawned at the markers and turned to each marker's heading. A terminating thread
// returns before anything is placed. Empty marker handles fall back to the origin and heading 0.
#include <cstdio>
#include <cstring>

struct Marker { int valid; float x; float heading; };
struct Out { int teleportTarget; int spawned; int kinds[3]; float spawnX[3]; float facing[3]; };
enum { TO_BLOCKWAY = 1, TO_TOLLBANDIT = 2 };
static float PosX(const Marker& m) { return m.valid == 0 ? 0.0f : m.x; }
static float Heading(const Marker& m) { return m.valid == 0 ? 0.0f : m.heading; }
static void Place(float distMe, float distBlock, bool terminating, const Marker* nearM, const Marker* farM, Out& o)
{
    std::memset(&o, 0, sizeof(o));
    const Marker* m;
    if (distMe > distBlock) {
        if (terminating) return;
        o.teleportTarget = TO_BLOCKWAY; m = nearM;
    } else {
        if (terminating) return;
        o.teleportTarget = TO_TOLLBANDIT; m = farM;
    }
    const int kinds[3] = { 1, 2, 1 };              // archer, grunt, archer
    for (int i = 0; i < 3; ++i) { o.kinds[i] = kinds[i]; o.spawnX[i] = PosX(m[i]); ++o.spawned; }
    for (int i = 0; i < 3; ++i) o.facing[i] = Heading(m[i]);
}
int main()
{
    Marker nearM[3] = { { 1, 10.0f, 0.5f }, { 1, 11.0f, 1.0f }, { 0, 12.0f, 1.5f } };
    Marker farM[3] = { { 1, 50.0f, 2.0f }, { 1, 51.0f, 2.5f }, { 1, 52.0f, 3.0f } };
    Out a, b, c;
    Place(100.0f, 20.0f, false, nearM, farM, a);
    Place(20.0f, 100.0f, false, nearM, farM, b);
    Place(100.0f, 20.0f, true, nearM, farM, c);
    bool ok = a.teleportTarget == TO_BLOCKWAY && a.spawned == 3 && a.kinds[1] == 2 && a.spawnX[0] == 10.0f
           && a.spawnX[2] == 0.0f && a.facing[2] == 0.0f && a.facing[1] == 1.0f
           && b.teleportTarget == TO_TOLLBANDIT && b.spawnX[2] == 52.0f && b.facing[0] == 2.0f
           && c.teleportTarget == 0 && c.spawned == 0;
    std::printf(ok ? "00e4e0e0_TEST PASS\n" : "00e4e0e0_TEST FAIL\n");
    return ok ? 0 : 1;
}