// Behaviour test for CWorld::Init (0x004A6E30): the subsystem creation order, the progress / profile labels, the
// scoped_ptr reset semantics (new object stored, old one destroyed after the store) and the navigator class registrations.
#include <cstdio>
#include <cstring>

static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

template <class T> class scoped_ptr {
public:
    scoped_ptr() : ptr(0) { }
    ~scoped_ptr() { delete ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
    T* get() const { return ptr; }
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
private:
    T* ptr;
};

static int g_destroyed = 0;
struct CManagerBase { virtual ~CManagerBase() { ++g_destroyed; } };
struct CWorldMap : CManagerBase { CWorldMap(long x1, long y1, long x2, long y2, unsigned long levels) { if (x1 == 0 && y1 == 0 && x2 == 0x2000 && y2 == 0x2000 && levels == 5) Log("WorldMap"); } };
struct CGameTimeManager : CManagerBase { CGameTimeManager() { Log("GameTime"); } };
struct CEnvironment : CManagerBase { CEnvironment() { Log("Environment"); } };
struct CNavigatorManager : CManagerBase {
    CNavigatorManager(unsigned long layers) { if (layers == 4) Log("Navigator"); }
    void AddNewClassType(long type, const char* name) { char b[64]; std::sprintf(b, "Class%ld:%s", type, name); Log(b); }
};
struct CFactionManager : CManagerBase { CFactionManager() { Log("Factions"); } };
struct CCombatManager : CManagerBase { CCombatManager() { Log("Combat"); } void Init() { Log("CombatInit"); } };
struct CThingSearchTools : CManagerBase { CThingSearchTools(void* things, CWorldMap* map) { if (things && map) Log("SearchTools"); } };
struct CCamera : CManagerBase { CCamera(const char* n) { Log(n); } };

struct CWorld {
    scoped_ptr<CWorldMap> PWorldMap;
    scoped_ptr<CEnvironment> PEnvironment;
    scoped_ptr<CGameTimeManager> PGameTimeManager;
    scoped_ptr<CThingSearchTools> PThingSearchTools;
    scoped_ptr<CCamera> PGameCamera;
    scoped_ptr<CCamera> PGameCameraManager;
    CCamera* PCurrentGameCamera;
    scoped_ptr<CNavigatorManager> PNavigationManager;
    scoped_ptr<CCombatManager> PThingCombatManager;
    void* PThingManager;
    scoped_ptr<CFactionManager> PFactionManager;
    long ConsolePauseAtFrameNumber;
    CWorld() : PCurrentGameCamera(0), PThingManager(0), ConsolePauseAtFrameNumber(-1) { }
    void Progress(const char* s, bool profile) { if (profile) { Log("P"); } Log(s); }
    void InitThingManager() { PThingManager = this; Log("ThingManager"); }
    void Init()
    {
        Progress("Init World Map", true);
        PWorldMap.reset(new CWorldMap(0, 0, 0x2000, 0x2000, 5));
        Progress("Init Environment", true);
        PGameTimeManager.reset(new CGameTimeManager);
        PEnvironment.reset(new CEnvironment);
        Progress("Init Navigation Manager", true);
        PNavigationManager.reset(new CNavigatorManager(4));
        PNavigationManager->AddNewClassType(3, "Navigator A Star");
        PNavigationManager->AddNewClassType(4, "Navigator flyer");
        PFactionManager.reset(new CFactionManager);
        Progress("Init Global Console", false);
        Progress("Adding Console Commands", false);
        ConsolePauseAtFrameNumber = 0;
        Progress("Init Combat Manager", true);
        PThingCombatManager.reset(new CCombatManager);
        PThingCombatManager->Init();
        Progress("Init Thing Manager", true);
        InitThingManager();
        Progress("Init Search Tools", false);
        PThingSearchTools.reset(new CThingSearchTools(PThingManager, PWorldMap.get()));
        Progress("Init Game Camera Manager", true);
        PGameCameraManager.reset(new CCamera("CameraManager"));
        Progress("Init Game Camera", false);
        PGameCamera.reset(new CCamera("Camera"));
        PCurrentGameCamera = PGameCameraManager.get();
    }
};

int main()
{
    CWorld world;
    world.Init();
    const char* expect = "P;Init World Map;WorldMap;P;Init Environment;GameTime;Environment;P;Init Navigation Manager;Navigator;"
        "Class3:Navigator A Star;Class4:Navigator flyer;Factions;Init Global Console;Adding Console Commands;P;Init Combat Manager;"
        "Combat;CombatInit;P;Init Thing Manager;ThingManager;Init Search Tools;SearchTools;P;Init Game Camera Manager;CameraManager;"
        "Init Game Camera;Camera;";
    if (std::strcmp(g_log, expect) != 0 || world.ConsolePauseAtFrameNumber != 0 || world.PCurrentGameCamera != world.PGameCameraManager.get() || g_destroyed != 0) {
        std::printf("004a6e30_TEST FAIL\n");
        return 1;
    }
    world.Init();                                      // a second Init replaces (and destroys) every owned manager
    if (g_destroyed != 9) {
        std::printf("004a6e30_TEST FAIL\n");
        return 1;
    }
    std::printf("004a6e30_TEST PASS\n");
    return 0;
}
