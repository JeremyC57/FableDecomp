// behaviour test for CAIStateGroup_MinionHasInformation::Wander (0x0090F320): logic copy over stubs
#include <cstdio>
struct V { float x, y, z; };
struct Info { V spot; float radius; long state; };
struct Group {
    Info* info; V pos; long wanderResult, idleResults[2], moveResult; int idles, moves, terminated;
    long Idle() { return idleResults[idles++]; }
    bool Wander()
    {
        if (wanderResult != 2) return false;
        if (info->state != 4) return true;
        if (Idle() != 2) { terminated++; return false; }
        V spot = info->spot; float radius = info->radius;
        float dx = pos.x - spot.x, dy = pos.y - spot.y;
        float d2 = dx * dx + dy * dy, r2 = radius * radius;
        if (!(d2 < r2)) {
            moves++;
            if (moveResult != 2) { terminated++; return false; }
            if (Idle() != 2) { terminated++; return false; }
        }
        return true;
    }
};
int main()
{
    Info info = { { 0, 0, 0 }, 5.0f, 4 };
    Group g = { &info, { 1, 1, 0 }, 2, { 2, 2 }, 2, 0, 0, 0 };
    bool ok = g.Wander() && g.moves == 0 && g.idles == 1;                      // already near: done
    Group h = { &info, { 10, 0, 0 }, 2, { 2, 2 }, 2, 0, 0, 0 };
    if (!h.Wander() || h.moves != 1 || h.idles != 2) ok = false;               // far: walk up and idle
    Group k = { &info, { 10, 0, 0 }, 2, { 2, 2 }, 1, 0, 0, 0 };
    if (k.Wander() || k.terminated != 1) ok = false;                           // walk failed
    Group m = { &info, { 10, 0, 0 }, 1, { 2, 2 }, 2, 0, 0, 0 };
    if (m.Wander() || m.idles != 0) ok = false;                                // wander failed
    info.state = 3;
    Group n = { &info, { 10, 0, 0 }, 2, { 2, 2 }, 2, 0, 0, 0 };
    if (!n.Wander() || n.idles != 0) ok = false;
    if (!ok) { std::printf("0090f320_TEST FAIL\n"); return 1; }
    std::printf("0090f320_TEST PASS\n");
    return 0;
}
