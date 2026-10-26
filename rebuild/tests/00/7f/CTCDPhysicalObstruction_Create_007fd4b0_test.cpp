#include <cstdio>
// Behaviour test: CTCDPhysicalObstruction::Create -- creates THING_PHYSICAL_OBSTRUCTION on the map under the start; with the obstruction driver it stores the line and radius, constructs at the start and activates (null if activation fails); without the driver the thing is killed.
#define CHECK(c) if (!(c)) { std::printf("007fd4b0_TEST FAIL\n"); return 1; }
#include <cstring>
class C3DVector { public: float X, Y, Z; };
class CTCDPhysicalObstruction { public: C3DVector StartPos, EndPos; long LifeTimer; float Radius; const C3DVector* constructedAt;
    void SetParams(const C3DVector& s, const C3DVector& e, float r) { StartPos = s; EndPos = e; Radius = r; } };
class CThing { public: bool hasDriver; bool activates; bool killed; CTCDPhysicalObstruction driver; };
static CThing g_thing;
static bool g_createOk = true;
static const char* g_lastName = 0;
static long g_lastMap = -1;
static long GetDefGlobalIndexFromName(const char* name) { g_lastName = name; return 77; }
static long GetMapNumberAt(const C3DVector& p) { return (long)p.Z; }
static CThing* CreateThing(long def, long player, long map) { g_lastMap = map; return (g_createOk && def == 77 && player == 4) ? &g_thing : 0; }
static CThing* Create(const C3DVector& startPos, const C3DVector& endPos, float radius)
{
    long defIndex = GetDefGlobalIndexFromName("THING_PHYSICAL_OBSTRUCTION");
    CThing* thing = CreateThing(defIndex, 4, GetMapNumberAt(startPos));
    CThing* result = 0;
    if (thing) {
        if (thing->hasDriver) {
            thing->driver.SetParams(startPos, endPos, radius);
            thing->driver.constructedAt = &startPos;
            if (thing->activates) result = thing;
        } else
            thing->killed = true;
    }
    return result;
}
int main()
{
    C3DVector s = { 1.0f, 2.0f, 3.0f }, e = { 4.0f, 5.0f, 6.0f };
    g_thing.hasDriver = true; g_thing.activates = true; g_thing.killed = false; g_thing.driver.constructedAt = 0;
    CHECK(Create(s, e, 1.5f) == &g_thing && g_lastMap == 3 && std::strcmp(g_lastName, "THING_PHYSICAL_OBSTRUCTION") == 0)
    CHECK(g_thing.driver.StartPos.Y == 2.0f && g_thing.driver.EndPos.Z == 6.0f && g_thing.driver.Radius == 1.5f && g_thing.driver.constructedAt == &s)
    g_thing.activates = false; CHECK(Create(s, e, 1.0f) == 0 && !g_thing.killed)
    g_thing.hasDriver = false; CHECK(Create(s, e, 1.0f) == 0 && g_thing.killed)
    g_createOk = false; CHECK(Create(s, e, 1.0f) == 0)
    std::printf("007fd4b0_TEST PASS\n");
    return 0;
}