// Behaviour test for CPlayer::InitModeViewHero (0x00636E90): three input processes are attached to the player
// interface, the GUI's spirit display is reset and its bottom half hidden, the view-hero camera definition is looked
// up, a new move-on-cylinder mode built from it is pushed (wrapped in a counted pointer whose block holds one
// reference), and the local definition reference is released afterwards.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CGamePlayerInterface { public: int id; };
class CAInputProcess { public: const char* name; CGamePlayerInterface* attachedTo;
    CAInputProcess(const char* n) : name(n), attachedTo(0) {}
    virtual bool AddToInterface(CGamePlayerInterface* pi) { attachedTo = pi; Log(name); return true; } };
class CPlayerGui { public: bool DisplayBottomHalfGUI; void ResetDrawSpiritsHealth() { Log("spirits"); } };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CDefPointee {
    mutable long RefCount; int deleted;
    virtual void DeleteThis() const { const_cast<CDefPointee*>(this)->deleted = 1; }
    void Release() const { --RefCount; if (RefCount == 0) DeleteThis(); }
};
class CCameraModeDef : public CDefPointee { public: };
template <class T> class CDefPointer { public: T* Data; CDefPointer() : Data(0) {} ~CDefPointer() { if (Data) Data->Release(); } };
static CCameraModeDef g_def;
class CGameDefinitionManager { public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    bool GetDef(const CCharString& n, CDefPointer<const CCameraModeDef>& d)
    { if (std::strcmp(n.t, "CAMERA_MODE_PC_VIEW_HERO")) return false; d.Data = &g_def; ++g_def.RefCount; return true; }
};
struct SCountBlock { long References; void* Object; SCountBlock(void* o) : References(1), Object(o) {} };
template <class T> class CCountedPointer { public:
    T* Data; SCountBlock* Info;
    CCountedPointer(T* p) : Data(p) { if (Data) Info = new SCountBlock(Data); else Info = 0; }
};
class CGameCameraManager;
namespace NCameraModes {
    class CBase { public: CGameCameraManager* manager; const CCameraModeDef* def;
        CBase(CGameCameraManager* m, const CDefPointer<const CCameraModeDef>& d) : manager(m), def(d.Data) {} virtual ~CBase() {} };
    struct CMoveOnCylinder : CBase { CMoveOnCylinder(CGameCameraManager* m, const CDefPointer<const CCameraModeDef>& d) : CBase(m, d) {} };
}
class CGameCameraManager { public:
    NCameraModes::CBase* pushed; long refs;
    void PushCameraMode(CCountedPointer<NCameraModes::CBase> mode) { pushed = mode.Data; refs = mode.Info ? mode.Info->References : 0; delete mode.Info; Log("push"); }
};
static CGameCameraManager g_cam;
struct CCameraManagerSource { bool GetCameraManager(CGameCameraManager** c) { *c = &g_cam; return true; } };
class CWorld { public: CCameraManagerSource* CameraSource; };
class CPlayer { public:
    CGamePlayerInterface* PlayerInterface; CWorld* World;
    CAInputProcess *InputProcessLeftStickLookAround_ptr, *InputProcessBButtonExitMode_ptr, *InputProcessToggleViewHeroMode_ptr;
    void InitModeViewHero(long mode, void* input);
};
void CPlayer::InitModeViewHero(long mode, void* input)
{
    (void)mode; (void)input;
    InputProcessLeftStickLookAround_ptr->AddToInterface(PlayerInterface);
    InputProcessBButtonExitMode_ptr->AddToInterface(PlayerInterface);
    InputProcessToggleViewHeroMode_ptr->AddToInterface(PlayerInterface);
    g_pPlayerGui->ResetDrawSpiritsHealth();
    g_pPlayerGui->DisplayBottomHalfGUI = false;
    CDefPointer<const CCameraModeDef> def;
    CGameDefinitionManager::Get().GetDef("CAMERA_MODE_PC_VIEW_HERO", def);
    CGameCameraManager* camera;
    World->CameraSource->GetCameraManager(&camera);
    camera->PushCameraMode(CCountedPointer<NCameraModes::CBase>(new NCameraModes::CMoveOnCylinder(camera, def)));
}
int main()
{
    int bad = 0;
    CGamePlayerInterface pi; CAInputProcess a("ls"), b("bx"), c("tv");
    CCameraManagerSource src; CWorld w; w.CameraSource = &src;
    CPlayer p; p.PlayerInterface = &pi; p.World = &w;
    p.InputProcessLeftStickLookAround_ptr = &a; p.InputProcessBButtonExitMode_ptr = &b; p.InputProcessToggleViewHeroMode_ptr = &c;
    g_gui.DisplayBottomHalfGUI = true; g_def.RefCount = 1; g_def.deleted = 0; g_log[0] = 0;
    p.InitModeViewHero(0, 0);
    if (std::strcmp(g_log, "ls;bx;tv;spirits;push;") != 0 || a.attachedTo != &pi || c.attachedTo != &pi || g_gui.DisplayBottomHalfGUI) bad = 1;
    if (!g_cam.pushed || g_cam.pushed->manager != &g_cam || g_cam.pushed->def != &g_def || g_cam.refs != 1) bad = 1;
    if (g_def.RefCount != 1 || g_def.deleted) bad = 1;
    delete g_cam.pushed;
    if (bad) { std::printf("00636e90_TEST FAIL\n"); return 1; }
    std::printf("00636e90_TEST PASS\n");
    return 0;
}