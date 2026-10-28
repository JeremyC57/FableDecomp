// Behaviour test for CWorld::PostInit (0x004A6550): creation order of the post-init managers, the script interface
// replacing (and deleting) the old one, the quest manager wired to the script info manager and script interface, the
// world handed to the 3D engine through its CIDrawWorld base, and the three sound-category refreshes only when a sound
// manager exists.
#include <cstdio>

static char g_log[64]; static int g_logLen = 0;
static void Log(char c) { if (g_logLen < 63) g_log[g_logLen++] = c; g_log[g_logLen] = 0; }

class CIDrawWorld { public: virtual ~CIDrawWorld() { } int Tag; };
class CScriptInfoManager { public: bool Initialised; CScriptInfoManager() : Initialised(false) { Log('s'); } void Init() { Initialised = true; Log('i'); } };
class CGameScriptInterface { public: int Id; static int Deleted; CGameScriptInterface(int id) : Id(id) { Log('g'); } ~CGameScriptInterface() { ++Deleted; } };
int CGameScriptInterface::Deleted = 0;
class CQuestManager { public: CScriptInfoManager* ScriptInfo; CGameScriptInterface* Script;
    CQuestManager(CScriptInfoManager& s, CGameScriptInterface& g) : ScriptInfo(&s), Script(&g) { Log('q'); } };
class CIEngine { public: const CIDrawWorld* World; CIEngine() : World(0) { } void SetWorld(const CIDrawWorld* w) { World = w; Log('w'); } };
class CDisplayEngine { public: CIEngine* Engine3D; CIEngine& Get3DEngine() { return *Engine3D; } };
struct CSound { long Categories[3]; int Count; CSound() : Count(0) { } void RefreshCategory(long c, float, float) { Categories[Count++] = c; } };

static CQuestManager* g_QuestManager = 0;
static CSound* g_SoundManager = 0;

struct CWorldBase { virtual ~CWorldBase() { } int Pad; };
class CWorld : public CWorldBase, public CIDrawWorld { public:
    bool Atmos, Music; CGameScriptInterface* Script; CScriptInfoManager* ScriptInfo;
    CWorld() : Atmos(false), Music(false), Script(0), ScriptInfo(0) { }
    void PostInit(CDisplayEngine& displayEngine)
    {
        Log('A'); Atmos = true;
        Log('S');
        CGameScriptInterface* old = Script; Script = new CGameScriptInterface(7); delete old;
        ScriptInfo = new CScriptInfoManager; ScriptInfo->Init();
        g_QuestManager = new CQuestManager(*ScriptInfo, *Script);
        Log('E');
        displayEngine.Get3DEngine().SetWorld(this);
        Log('M'); Music = true;
        if (g_SoundManager) {
            g_SoundManager->RefreshCategory(0x10, 0, 0);
            g_SoundManager->RefreshCategory(0x40, 0, 0);
            g_SoundManager->RefreshCategory(0x80, 0, 0);
        }
    }
};

static bool Same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }

int main()
{
    CIEngine engine; CDisplayEngine display; display.Engine3D = &engine;
    CWorld world; world.Script = new CGameScriptInterface(1); g_logLen = 0; g_log[0] = 0;
    CSound sound; g_SoundManager = &sound;
    world.PostInit(display);
    bool ok = Same(g_log, "ASgsiqEwM") && CGameScriptInterface::Deleted == 1 && world.Script->Id == 7
        && world.ScriptInfo->Initialised && g_QuestManager->ScriptInfo == world.ScriptInfo && g_QuestManager->Script == world.Script
        && engine.World == static_cast<CIDrawWorld*>(&world) && (const void*)engine.World != (const void*)&world
        && world.Atmos && world.Music && sound.Count == 3 && sound.Categories[0] == 0x10 && sound.Categories[1] == 0x40
        && sound.Categories[2] == 0x80;
    g_SoundManager = 0; CWorld quiet; quiet.PostInit(display);
    ok = ok && sound.Count == 3 && quiet.Script != 0 && engine.World == static_cast<CIDrawWorld*>(&quiet);
    if (!ok) { std::printf("004a6550_TEST FAIL\n"); return 1; }
    std::printf("004a6550_TEST PASS\n");
    return 0;
}