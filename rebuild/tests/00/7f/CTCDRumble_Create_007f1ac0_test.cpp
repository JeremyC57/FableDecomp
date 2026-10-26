#include <cstdio>
#include <cmath>
// Behaviour test: CTCDRumble::Create -- creates THING_RUMBLE on the map under the position; with the rumble driver it stores strength/length/global, constructs and activates it (null if activation fails); without the driver the thing is killed and null returned.
#define CHECK(c) if (!(c)) { std::printf("007f1ac0_TEST FAIL\n"); return 1; }
#include <cstring>
class C3DVector { public: float X, Y, Z; };
class CTCDRumble { public: float Strength, Length; bool GlobalRumble; int constructed; };
class CThing { public: bool hasDriver; bool activates; bool killed; CTCDRumble driver; };
static CThing g_thing;
static bool g_createOk = true;
static const char* g_lastName = 0;
static long g_lastMap = -1;
static long GetDefGlobalIndexFromName(const char* name) { g_lastName = name; return 42; }
static long GetMapNumberAt(const C3DVector& p) { return (long)p.Z; }
static CThing* CreateThing(long def, long player, long map) { g_lastMap = map; return (g_createOk && def == 42 && player == 4) ? &g_thing : 0; }
static CThing* Create(const C3DVector& pos, float strength, float length, bool global)
{
    long defIndex = GetDefGlobalIndexFromName("THING_RUMBLE");
    CThing* thing = CreateThing(defIndex, 4, GetMapNumberAt(pos));
    CThing* result = 0;
    if (thing) {
        if (thing->hasDriver) {
            thing->driver.Strength = strength; thing->driver.Length = length; thing->driver.GlobalRumble = global;
            ++thing->driver.constructed;
            if (thing->activates) result = thing;
        } else
            thing->killed = true;
    }
    return result;
}
int main()
{
    C3DVector p = { 0.0f, 0.0f, 2.0f };
    g_thing.hasDriver = true; g_thing.activates = true; g_thing.killed = false; g_thing.driver.constructed = 0;
    CHECK(Create(p, 0.5f, 2.0f, true) == &g_thing && g_thing.driver.Strength == 0.5f && g_thing.driver.GlobalRumble && g_lastMap == 2)
    CHECK(std::strcmp(g_lastName, "THING_RUMBLE") == 0)
    g_thing.activates = false;
    CHECK(Create(p, 1.0f, 1.0f, false) == 0 && g_thing.driver.constructed == 2 && !g_thing.killed)
    g_thing.hasDriver = false;
    CHECK(Create(p, 1.0f, 1.0f, false) == 0 && g_thing.killed)
    g_createOk = false;
    CHECK(Create(p, 1.0f, 1.0f, false) == 0)
    std::printf("007f1ac0_TEST PASS\n");
    return 0;
}