// Behaviour test for CTCQuestCompletionUI::~CTCQuestCompletionUI @ 0x005E41E0: the quest completion screen leaves the global TC
// list, deletes the black backdrop and forgets the frame, re-enables 3D scene rendering and stamps the world's start-3D frame,
// deletes and empties its UI components, resets the UI manager (observers, action params, the list selection history, the bastard
// child), turns cutscene preloading off and reopens the player GUI only when it is closed.
#include <stdio.h>
#include <string.h>
static char g_log[512];
static void logc(const char* s) { strcat(g_log, s); strcat(g_log, ","); }
static int g_currentFrame = 0;
class CComponent { public: int Id; virtual ~CComponent() { char b[16]; sprintf(b, "del%d", Id); logc(b); } };
class C3DEngine { public:
    void Enable3DSceneRendering(bool on) { logc(on ? "enable3d" : "disable3d"); }
    void CutscenePreload(bool a, bool b) { logc(!a && !b ? "preloadoff" : "preloadon"); }
};
class CDisplayEngine { public: C3DEngine Engine; C3DEngine& Get3DEngine() { return Engine; } };
class CMainGameComponent { public: CDisplayEngine Display; CDisplayEngine& GetDisplayEngine() { return Display; } };
CMainGameComponent* g_pMainGameComponent = 0;
class CPlayerGui { public: bool Opened; bool IsOpen() const { return Opened; } void Open() { Opened = true; logc("guiopen"); } };
CPlayerGui* g_pPlayerGui = 0;
class CWorld { public:
    int FrameStarted3DRendering;
    static int GetFrame() { logc("getframe"); return g_currentFrame; }
    void SetAsStarting3DRendering() { FrameStarted3DRendering = GetFrame(); }
};
static CWorld g_world;
namespace NUISystem {
class CManager { public:
    void ClearObservers() { logc("clearobs"); }
    void ClearActionParams() { logc("clearparams"); }
    void ClearBastardChild() { logc("clearbastard"); }
    static CManager* GetInstance() { static CManager m; return &m; }
};
class CList { public: static int GPreviousSelectedCount; static void ClearPrevious() { GPreviousSelectedCount = 0; logc("clearprev"); } };
int CList::GPreviousSelectedCount = 3;
}
template <class T> class vector { public:
    T items[8]; unsigned int count;
    unsigned int size() const { return count; }
    T& operator[](unsigned int n) { return items[n]; }
    void clear() { count = 0; }
};
class CTCBase { public:
    virtual ~CTCBase() { logc("base"); }
    void RemoveFromGlobalTCs() { logc("removeglobal"); }
    CWorld& GetWorld() const { return g_world; }
};
class CTCQuestCompletionUI : public CTCBase { public:
    vector<CComponent*> Components;
    CComponent* PFrame;
    CComponent* PBlack;
    virtual ~CTCQuestCompletionUI();
};
CTCQuestCompletionUI::~CTCQuestCompletionUI()
{
    RemoveFromGlobalTCs();
    delete PBlack;
    PFrame = 0;
    g_pMainGameComponent->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
    GetWorld().SetAsStarting3DRendering();
    for (unsigned int i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::ClearPrevious();
    NUISystem::CManager::GetInstance()->ClearBastardChild();
    g_pMainGameComponent->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
    if (!g_pPlayerGui->IsOpen())
        g_pPlayerGui->Open();
}
static int fail(int line) { printf("005e41e0_TEST FAIL\nline %d\n%s\n", line, g_log); return 1; }
int main()
{
    CMainGameComponent game; g_pMainGameComponent = &game;
    CPlayerGui gui; gui.Opened = false; g_pPlayerGui = &gui;
    g_currentFrame = 4242; g_world.FrameStarted3DRendering = 0;
    CTCQuestCompletionUI* ui = new CTCQuestCompletionUI;
    ui->Components.count = 2;
    ui->Components.items[0] = new CComponent; ui->Components.items[0]->Id = 1;
    ui->Components.items[1] = new CComponent; ui->Components.items[1]->Id = 2;
    ui->PBlack = new CComponent; ui->PBlack->Id = 9;
    ui->PFrame = ui->PBlack;
    delete ui;
    const char* want = "removeglobal,del9,enable3d,getframe,del1,del2,clearobs,clearparams,clearprev,clearbastard,preloadoff,guiopen,base,";
    if (strcmp(g_log, want) != 0 || g_world.FrameStarted3DRendering != 4242 || !gui.Opened ||
        NUISystem::CList::GPreviousSelectedCount != 0) return fail(__LINE__);
    g_log[0] = 0;                                           // GUI already open; no backdrop
    CTCQuestCompletionUI* ui2 = new CTCQuestCompletionUI;
    ui2->Components.count = 0; ui2->PBlack = 0; ui2->PFrame = 0;
    delete ui2;
    if (strcmp(g_log, "removeglobal,enable3d,getframe,clearobs,clearparams,clearprev,clearbastard,preloadoff,base,") != 0)
        return fail(__LINE__);
    printf("005e41e0_TEST PASS\n");
    return 0;
}