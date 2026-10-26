#include <cstdio>
#include <cstring>
// Behaviour test for CTCPreCalculatedNavigationRoute::CalculateRouteFromTo (0x00800540): the body of src/ over stand-ins --
// nothing unless the game runs and both ends share a region; otherwise a throwaway navigation creature is created at the
// owner, asked for the route through its navigation interface, and killed.
static int g_liveStrings = 0;
class CCharString { public:
    char text[96];
    CCharString(const char* t, long length = -1) { strcpy(text, t); ++g_liveStrings; }
    CCharString(const CCharString& o) { strcpy(text, o.text); ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
class C3DVector { public: float X, Y, Z; };
struct CRoute { int points; };
class CThingPhysical;
class CThingBuilding;
class CThingAICreatureInit { public:
    CThingPhysical* OriginalThing;
    CThingPhysical* PreviousThing;
    CCharString     ScriptName;
    CCharString     OverridingBrainName;
    bool            PlayerCharacter;
    bool            Drawable;
    bool            PlaceAtEndOfList;
    bool            EnableCreatureAutoPlacing;
    CThingBuilding* HomeBuilding;
    CThingBuilding* WorkBuilding;
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName), OverridingBrainName(overridingBrainName),
          PlayerCharacter(playerCharacter), Drawable(drawable), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};
static const C3DVector* g_routeFrom; static const C3DVector* g_routeTo;
struct CTCNavigationBase {
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoute& route)
    {
        g_routeFrom = &from; g_routeTo = &to; route.points = 3;
    }
};
static CTCNavigationBase g_nav;
class CThing {
public:
    C3DVector pos;
    long requested;
    int kills; bool killSilent;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) { requested = interfaceType; *ppOut = (T*)&g_nav; return true; }
    const C3DVector& GetPosition() const { return pos; }
    void Kill(bool silent) { ++kills; killSilent = silent; }
};
static long g_createDef; static const C3DVector* g_createPos; static long g_createFlags; static bool g_initOk; static int g_creates;
class CThingAICreature : public CThing {
public:
    static CThingAICreature* Create(long defIndex, const C3DVector& p, long flags, const CThingAICreatureInit& init);
};
static CThingAICreature g_creature;
CThingAICreature* CThingAICreature::Create(long defIndex, const C3DVector& p, long flags, const CThingAICreatureInit& init)
{
    ++g_creates; g_createDef = defIndex; g_createPos = &p; g_createFlags = flags;
    g_initOk = init.OriginalThing == 0 && init.PreviousThing == 0 && !init.PlayerCharacter && init.Drawable
        && init.ScriptName.text[0] == 0 && init.OverridingBrainName.text[0] == 0 && init.PlaceAtEndOfList
        && !init.EnableCreatureAutoPlacing && init.HomeBuilding == 0 && init.WorkBuilding == 0;
    return &g_creature;
}
static char g_defName[96];
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) { strcpy(g_defName, name.text); return 123; }
};
static CDefinitionManager g_defs;
class CGameDefinitionManager { public: static CDefinitionManager& Get() { return g_defs; } };
struct CGameRunState { bool running; bool IsRunning() { return running; } };
static CGameRunState g_run = { false };
class CWorldMap { public:
    long GetMapNumberAt(const C3DVector& p) const { return (long)p.X; }
    long GetRegionNumberMapIsIn(long mapNumber) { return mapNumber / 10; }
};
static CWorldMap g_map;
class CTCBase {
public:
    CThing* ParentThing;
    CGameRunState* GetRunState() { return &g_run; }
    CWorldMap* GetWorldMap() { return &g_map; }
};
class CTCPreCalculatedNavigationRoute : public CTCBase {
public:
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoute& route);
};

void CTCPreCalculatedNavigationRoute::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoute& route)
{
    if (!GetRunState()->IsRunning())
        return;
    long fromRegion = GetWorldMap()->GetRegionNumberMapIsIn(GetWorldMap()->GetMapNumberAt(from));
    if (fromRegion != GetWorldMap()->GetRegionNumberMapIsIn(GetWorldMap()->GetMapNumberAt(to)))
        return;
    long defIndex;
    {
        CCharString name("CREATURE_YOURE_ONLY_INTERESTED_IN_ME_FOR_MY_NAVIGATION_PLEASE_DONT_DELETE_ME");
        defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThingAICreature* creature;
    {
        CCharString brain("");
        CCharString script("");
        creature = CThingAICreature::Create(defIndex, ParentThing->GetPosition(), 4,
            CThingAICreatureInit(0, 0, false, script, brain, true));
    }
    CTCNavigationBase* navigation;
    creature->GetInterface(&navigation, 7);
    navigation->CalculateRouteFromTo(from, to, route);
    creature->Kill(false);
}

static int fail(int line) { printf("00800540_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing owner; owner.pos.X = 1; owner.pos.Y = 2; owner.pos.Z = 3;
    CTCPreCalculatedNavigationRoute nav; nav.ParentThing = &owner;
    C3DVector from = { 21, 0, 0 }, to = { 25, 0, 0 }, far = { 35, 0, 0 };
    CRoute route = { 0 };
    g_creature.kills = 0;
    nav.CalculateRouteFromTo(from, to, route);       // not running
    CHECK(g_creates == 0 && route.points == 0);
    g_run.running = true;
    nav.CalculateRouteFromTo(from, far, route);      // different regions
    CHECK(g_creates == 0 && route.points == 0);
    nav.CalculateRouteFromTo(from, to, route);
    CHECK(g_creates == 1 && g_createDef == 123 && g_createPos == &owner.pos && g_createFlags == 4 && g_initOk);
    CHECK(strcmp(g_defName, "CREATURE_YOURE_ONLY_INTERESTED_IN_ME_FOR_MY_NAVIGATION_PLEASE_DONT_DELETE_ME") == 0);
    CHECK(g_creature.requested == 7 && g_routeFrom == &from && g_routeTo == &to && route.points == 3);
    CHECK(g_creature.kills == 1 && !g_creature.killSilent && g_liveStrings == 0);
    printf("00800540_TEST PASS\n");
    return 0;
}