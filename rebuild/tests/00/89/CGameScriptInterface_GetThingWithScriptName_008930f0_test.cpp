// Behaviour model of CGameScriptInterface::GetThingWithScriptName (0x008930F0): look up the map number of mapName, find the first
// scan-visible game object whose script name is scriptName on that map; return its script thing when found and alive, else the null
// script thing.
#include <cstdio>
#include <cstring>
#include <vector>
struct Thing { const char* script; long map; bool gameObject, dead, scanned; int id; };
struct ScriptThing { int id; };
template <class T> inline bool IS_THING_ALIVE(const T& thing) { if (thing) { if (!thing->dead) return true; } return false; }
static long MapNumber(const char* mapName) { return std::strcmp(mapName, "Oakvale") == 0 ? 3 : (std::strcmp(mapName, "Bowerstone") == 0 ? 5 : 0); }
static bool GetPFirstThing(const std::vector<Thing>& world, const char* script, long map, const Thing** out)
{
    for (unsigned int i = 0; i < world.size(); ++i) {
        const Thing& t = world[i];
        if (t.gameObject && t.scanned && std::strcmp(t.script, script) == 0 && t.map == map) { *out = &t; return true; }
    }
    return false;
}
static ScriptThing GetThingWithScriptName(const std::vector<Thing>& world, const char* mapName, const char* scriptName)
{
    const Thing* thing = 0;
    long mapNo = MapNumber(mapName);
    bool notFound = !GetPFirstThing(world, scriptName, mapNo, &thing);
    ScriptThing null = { 0 };
    if (notFound) return null;
    if (!IS_THING_ALIVE(thing)) return null;
    ScriptThing r = { thing->id };
    return r;
}
int main()
{
    std::vector<Thing> w;
    Thing a = { "Guard", 3, true, false, true, 11 }; w.push_back(a);
    Thing b = { "Guard", 5, true, true, true, 12 }; w.push_back(b);
    Thing c = { "Trader", 5, false, false, true, 13 }; w.push_back(c);
    Thing d = { "Trader", 5, true, false, false, 14 }; w.push_back(d);
    Thing e = { "Trader", 5, true, false, true, 15 }; w.push_back(e);
    if (GetThingWithScriptName(w, "Oakvale", "Guard").id != 11) { std::printf("008930f0_TEST FAIL 1\n"); return 1; }
    if (GetThingWithScriptName(w, "Bowerstone", "Guard").id != 0) { std::printf("008930f0_TEST FAIL 2\n"); return 1; }
    if (GetThingWithScriptName(w, "Bowerstone", "Trader").id != 15) { std::printf("008930f0_TEST FAIL 3\n"); return 1; }
    if (GetThingWithScriptName(w, "Nowhere", "Guard").id != 0) { std::printf("008930f0_TEST FAIL 4\n"); return 1; }
    std::printf("008930f0_TEST PASS\n");
    return 0;
}