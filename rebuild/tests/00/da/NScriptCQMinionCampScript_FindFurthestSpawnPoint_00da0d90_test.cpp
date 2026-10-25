// Behaviour test for CQ_MinionCampScript::FindFurthestSpawnPoint (0x00DA0D90): walk the spawn points, keep the one
// furthest from the hero (a -1 distance means "none yet"), and report false when nothing was found or the thread is
// terminating.
#include <cstdio>

class CScriptThing {
public:
    float x;
};
static float AbsF(float v) { return v < 0.0f ? -v : v; }
float GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b) { return AbsF(a.x - b.x); }
struct CQ_MinionCampScript {
    bool terminating;
    CScriptThing hero;
    bool IsActiveThreadTerminating() { return terminating; }
    bool FindFurthestSpawnPoint(const CScriptThing* first, const CScriptThing* last, const CScriptThing*& furthest, float& furthestDistance)
    {
        bool found = false;
        for (const CScriptThing* it = first; it != last; ++it) {
            if (IsActiveThreadTerminating())
                return false;
            const CScriptThing& spawnPoint = *it;
            float distance = GetDistanceBetweenThings(spawnPoint, hero);
            if (distance > furthestDistance || furthestDistance == -1.0f) {
                if (IsActiveThreadTerminating())
                    return false;
                furthestDistance = distance;
                furthest = &spawnPoint;
                found = true;
            }
        }
        if (IsActiveThreadTerminating())
            return false;
        return found;
    }
};

int main()
{
    CQ_MinionCampScript s;
    s.terminating = false;
    s.hero.x = 0.0f;
    CScriptThing points[4];
    points[0].x = 3.0f; points[1].x = -9.0f; points[2].x = 4.0f; points[3].x = 8.5f;
    const CScriptThing* furthest = 0;
    float dist = -1.0f;
    int bad = 0;
    if (!s.FindFurthestSpawnPoint(points, points + 4, furthest, dist) || furthest != &points[1] || AbsF(dist - 9.0f) > 0.001f) ++bad;
    const CScriptThing* f2 = 0;
    float d2 = 100.0f;                                                  // nothing beats an existing record
    if (s.FindFurthestSpawnPoint(points, points + 4, f2, d2) || f2 != 0 || AbsF(d2 - 100.0f) > 0.001f) ++bad;
    float d3 = -1.0f;
    if (s.FindFurthestSpawnPoint(points, points, f2, d3)) ++bad;        // empty list
    s.terminating = true;
    float d4 = -1.0f;
    if (s.FindFurthestSpawnPoint(points, points + 4, f2, d4) || f2 != 0) ++bad;
    if (bad) {
        std::printf("00da0d90_TEST FAIL\n");
        return 1;
    }
    std::printf("00da0d90_TEST PASS\n");
    return 0;
}