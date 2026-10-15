#include <cstdio>
#include <cstring>
// Host model of the CGameScriptThing constructors: counted pair (+4/+8), intelligent pointer, counter, UID, two
// strings, two positions, angle and world.
struct Thing { int id; };
struct World { int id; };
struct Str { const char* text; Str() : text("") {} Str& operator=(const Str& o) { text = o.text; return *this; } };
static Str g_defaultName;
struct IntelligentPointer { Thing* item; int sets; IntelligentPointer() : item(0), sets(0) {} IntelligentPointer(Thing* t) : item(0), sets(0) { item = t; ++sets; } };
struct Vec { float X, Y, Z; Vec(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct GameScriptThing {
    void* refObject; void* refCount;
    IntelligentPointer PThing;
    long ScriptCounter;
    unsigned int ThingUID;
    Str Name, DataString;
    Vec Pos, HomePos;
    float AngleXY;
    World* W;
    Thing* initFrom;
    GameScriptThing(World* w)
        : refObject(0), refCount(0), PThing(0), ScriptCounter(0), Pos(0, 0, 0), HomePos(0, 0, 0), AngleXY(0), W(w), initFrom(0)
    {
        Name = g_defaultName;
        ThingUID = 0;
    }
    GameScriptThing(World* w, Thing* t)
        : refObject(0), refCount(0), ScriptCounter(0), Pos(0, 0, 0), HomePos(0, 0, 0), AngleXY(0), W(w), initFrom(0)
    {
        InitFromThing(t);
    }
    void InitFromThing(Thing* t) { initFrom = t; PThing.item = t; ThingUID = 5; }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static bool Zeroed(const GameScriptThing& g)
{
    return g.refObject == 0 && g.refCount == 0 && g.ScriptCounter == 0 && g.Pos.X == 0 && g.Pos.Y == 0 && g.Pos.Z == 0
        && g.HomePos.X == 0 && g.HomePos.Y == 0 && g.HomePos.Z == 0 && g.AngleXY == 0;
}
int main()
{
    g_defaultName.text = "ScriptThing";
    World w; GameScriptThing g(&w);
    CHECK(Zeroed(g));
    CHECK(g.PThing.item == 0 && g.PThing.sets == 1);
    CHECK(g.ThingUID == 0 && std::strcmp(g.Name.text, "ScriptThing") == 0 && std::strcmp(g.DataString.text, "") == 0);
    CHECK(g.W == &w && g.initFrom == 0);
    if (g_fail) { std::printf("008cff60_TEST FAIL\n"); return 1; }
    std::printf("008cff60_TEST PASS\n");
    return 0;
}