// Behaviour test for CThingManager::CThingManager (0x00523540): the back pointers, the world-map query, the zeroed
// containers / flags, the self-linked map headers, the current-manager registration, the UID seed, the draw
// distance, the type-table build and the save format version -- through a copy of the model.
#include <cstdio>
#include <cstdlib>

struct MapNode { bool color; MapNode* parent; MapNode* left; MapNode* right; };
struct Map {
    MapNode* header; unsigned int count;
    Map() : header(0) { header = (MapNode*)std::malloc(0x20); count = 0; header->color = false; header->parent = 0; header->left = header; header->right = header; }
};
struct Vec { void* start; void* finish; void* eos; Vec() : start(0), finish(0), eos(0) {} };

struct World;
static int g_worldMapQueries = 0;
struct WorldMap { int id; };
struct Environment { int id; };
struct World {
    WorldMap* map; Environment* PEnvironment;
    virtual WorldMap* GetWorldMap() { ++g_worldMapQueries; return map; }
    virtual ~World() {}
};

struct ClassFactory {
    unsigned int MaxClassTypeID;
    ClassFactory(unsigned char maxClassTypeID) : MaxClassTypeID(maxClassTypeID) {}
};

struct ThingManager;
static ThingManager* g_current = 0;
static void SetCurrentThingManager(ThingManager* m) { g_current = m; }

struct ThingManager : ClassFactory {
    const int* Component; const int* DefinitionManager; World* PWorld; WorldMap* PWorldMap; unsigned int* WorldSeed;
    int* PlayerManager; int* NavigationManager; Environment* PEnvironment;
    Vec ThingTypeInfo, DeadThingList;
    bool NonScriptedEntitesPauseMode, AllEntitesPauseMode;
    Map UIDToThingMap;
    unsigned int CurrentUniqueIDLo, CurrentUniqueIDHi, CurrentLocalLo, CurrentLocalHi;
    bool DrawCombatCollisionDebug, DrawAttitudeDebug;
    int MaxThingDrawDist, GlobalThingLoadingBehaviour;
    void* RenderHead; void* RenderTail;
    Vec UpdateLists;
    bool UseMapSpecificUIDs, UpdateAndRenderAllFlag, CurrentlyLoading, LoadingEntitiesFromSaveGame;
    Vec LevelScripts, GameScripts; Map PersistedDeadEntities; int SaveFormatVersion;
    bool ErrorFlag; Map NPCNameMap;
    int typesBuilt;
    void InitThingTypes() { ++typesBuilt; }
    ThingManager(const int& component, const int& defs, World& world, int& players, int& navigator, unsigned int* seed,
                 unsigned int, unsigned int, unsigned int)
        : ClassFactory(0x10), Component(&component), DefinitionManager(&defs), PWorld(&world), PWorldMap(world.GetWorldMap()),
          WorldSeed(seed), PlayerManager(&players), NavigationManager(&navigator), NonScriptedEntitesPauseMode(false),
          AllEntitesPauseMode(false), CurrentLocalLo(0), CurrentLocalHi(0), DrawCombatCollisionDebug(false),
          DrawAttitudeDebug(false), GlobalThingLoadingBehaviour(1), LoadingEntitiesFromSaveGame(false), ErrorFlag(false),
          typesBuilt(0)
    {
        RenderHead = 0; RenderTail = 0;
        SetCurrentThingManager(this);
        CurrentlyLoading = false;
        UseMapSpecificUIDs = false;
        PEnvironment = PWorld->PEnvironment;
        CurrentUniqueIDLo = 1; CurrentUniqueIDHi = 0;
        MaxThingDrawDist = 60;
        UpdateAndRenderAllFlag = false;
        InitThingTypes();
        SaveFormatVersion = 2;
    }
};

int main()
{
    int component = 1, defs = 2, players = 3, navigator = 4;
    unsigned int seed = 77;
    WorldMap wm; wm.id = 5; Environment env; env.id = 6;
    World world; world.map = &wm; world.PEnvironment = &env;
    ThingManager m(component, defs, world, players, navigator, &seed, 0, 0, 0);
    if (m.MaxClassTypeID != 0x10 || m.Component != &component || m.DefinitionManager != &defs || m.PWorld != &world) { std::printf("00523540_TEST FAIL\n"); return 1; }
    if (m.PWorldMap != &wm || g_worldMapQueries != 1 || m.WorldSeed != &seed || m.PlayerManager != &players || m.NavigationManager != &navigator) { std::printf("00523540_TEST FAIL\n"); return 1; }
    if (m.PEnvironment != &env || g_current != &m || m.typesBuilt != 1 || m.SaveFormatVersion != 2) { std::printf("00523540_TEST FAIL\n"); return 1; }
    if (m.CurrentUniqueIDLo != 1 || m.CurrentUniqueIDHi != 0 || m.CurrentLocalLo != 0 || m.MaxThingDrawDist != 60 || m.GlobalThingLoadingBehaviour != 1) { std::printf("00523540_TEST FAIL\n"); return 1; }
    if (m.UIDToThingMap.header->left != m.UIDToThingMap.header || m.UIDToThingMap.header->parent != 0 || m.UIDToThingMap.count != 0) { std::printf("00523540_TEST FAIL\n"); return 1; }
    if (m.NPCNameMap.header->right != m.NPCNameMap.header || m.ThingTypeInfo.start != 0 || m.UpdateLists.eos != 0 || m.RenderHead != 0) { std::printf("00523540_TEST FAIL\n"); return 1; }
    if (m.NonScriptedEntitesPauseMode || m.AllEntitesPauseMode || m.ErrorFlag || m.CurrentlyLoading || m.UseMapSpecificUIDs || m.UpdateAndRenderAllFlag) { std::printf("00523540_TEST FAIL\n"); return 1; }
    std::printf("00523540_TEST PASS\n");
    return 0;
}