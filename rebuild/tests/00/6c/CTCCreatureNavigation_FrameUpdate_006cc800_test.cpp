// Behaviour test for CTCCreatureNavigation::FrameUpdate (0x006CC800), a portable model of the row's decisions: the navigator update
// every fifth frame (uid + frame), and the rescan when due ((frame >= next and (next == 0 or ((i64)uid + frame) % fps == 0)) or the creature
// sped up), which records the speed, schedules frame + fps, clears the agents and scans a box of half-size max(ceil(fps * speed * 3), 2).
#include <stdio.h>
#include <math.h>
#ifdef _MSC_VER
typedef __int64 i64; typedef unsigned __int64 u64;
#else
typedef long long i64; typedef unsigned long long u64;
#endif
struct Nav { long NextUpdate; float CurrentSpeed; int agents; long lastHalf; int navUpdates; int scans; };
static void FrameUpdate(Nav& n, u64 uid, long frame, long fps, float speed)
{
    if (((i64)uid + frame) % 5 == 0) ++n.navUpdates;
    if ((frame >= n.NextUpdate && (n.NextUpdate == 0 || ((i64)uid + frame) % fps == 0)) || speed > n.CurrentSpeed) {
        long next_update = fps;
        n.CurrentSpeed = speed;
        n.NextUpdate = frame + next_update;
        n.agents = 0;
        long half = (long)ceil(next_update * n.CurrentSpeed * 3.0f);
        if (half < 2) half = 2;
        n.lastHalf = half; ++n.scans;
    }
}
int main()
{
    int bad = 0;
    Nav n = { 0, 0.0f, 5, 0, 0, 0 };
    FrameUpdate(n, 7, 3, 30, 0.5f);                 // first update: NextUpdate 0 -> scan; (7+3)%5==0 -> navigator
    if (n.scans != 1 || n.navUpdates != 1 || n.NextUpdate != 33 || n.lastHalf != 45 || n.agents != 0) ++bad;
    FrameUpdate(n, 7, 4, 30, 0.5f);                 // not due, same speed
    if (n.scans != 1 || n.navUpdates != 1) ++bad;
    FrameUpdate(n, 7, 5, 30, 0.75f);                // sped up -> rescan (ceil(30*0.75*3) = 68)
    if (n.scans != 2 || n.lastHalf != 68 || n.NextUpdate != 35) ++bad;
    FrameUpdate(n, 7, 35, 30, 0.1f);                // due, but (7+35)%30 != 0 and slower -> no scan
    if (n.scans != 2) ++bad;
    FrameUpdate(n, 7, 53, 30, 0.0f);                // due and (7+53)%30 == 0 -> scan, half-size floor 2
    if (n.scans != 3 || n.lastHalf != 2 || n.navUpdates != 2) ++bad;
    printf(bad ? "006cc800_TEST FAIL\n" : "006cc800_TEST PASS\n");
    return bad ? 1 : 0;
}