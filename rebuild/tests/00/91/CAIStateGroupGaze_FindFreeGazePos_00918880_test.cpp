// Behaviour test for 0x00918880: FindFreeGazePos tries the primary then (if different) the secondary marker def.
#include <cstdio>
struct CMarker { int id; };
struct CGaze {
    bool valid; long now, sleepStart, sleepEnd; long PrimaryMarkerDef, SecondaryMarkerDef;
    CMarker* primary; CMarker* secondary; int lookups; bool lastFlag;
    bool IsStateGroupStillValid() { return valid; }
    void GetSleepTimes(long* s, long* e) { *s = sleepStart; *e = sleepEnd; }
    bool IsTimeOfDayInRange(long s, long e) { return now >= s && now < e; }
    CMarker* FindFreeParticularGazePos(long def, bool flag) { ++lookups; lastFlag = flag; return def == PrimaryMarkerDef ? primary : secondary; }
    CMarker* FindFreeGazePos()
    {
        bool inSleepTime = false;
        if (IsStateGroupStillValid()) { long s, e; GetSleepTimes(&s, &e); inSleepTime = IsTimeOfDayInRange(s, e); }
        CMarker* m = FindFreeParticularGazePos(PrimaryMarkerDef, inSleepTime);
        if (m == 0 && PrimaryMarkerDef != SecondaryMarkerDef) m = FindFreeParticularGazePos(SecondaryMarkerDef, inSleepTime);
        return m;
    }
};
int main()
{
    CMarker a = { 1 }, b = { 2 };
    CGaze g = { true, 5, 1, 9, 10, 20, &a, &b, 0, false };
    int bad = 0;
    if (g.FindFreeGazePos() != &a || g.lookups != 1 || !g.lastFlag) bad = 1;
    g.primary = 0; g.lookups = 0; if (g.FindFreeGazePos() != &b || g.lookups != 2) bad = 2;
    g.SecondaryMarkerDef = 10; g.lookups = 0; if (g.FindFreeGazePos() != 0 || g.lookups != 1) bad = 3;
    g.valid = false; if (g.FindFreeGazePos() != 0 || g.lastFlag) bad = 4;
    if (bad) { std::printf("00918880_TEST FAIL\n"); return 1; }
    std::printf("00918880_TEST PASS\n");
    return 0;
}