// 00634560 behaviour test for CPlayer::UninitModeViewHero(EPlayerMode, CProcessedInput*)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
#include <cstring>
static char g_log[128];
static void Log(const char* s) { std::strcat(g_log, s); }
class CGamePlayerInterface { };
class CAInputProcess {
public:
    const char* Name;
    explicit CAInputProcess(const char* name) : Name(name) { }
    virtual ~CAInputProcess() { }
    virtual bool AddToInterface(CGamePlayerInterface& manager) { (void)manager; Log("+"); Log(Name); return true; }
    void RemoveFromInterface() { Log("-"); Log(Name); }
};
class CTCBase { public: virtual ~CTCBase() { } };
class CThing {
public:
    unsigned char Dead : 1;
    long Types[4];
    CTCBase* Components[4];
    int Count;
    CThing() : Dead(0), Count(0) { }
    void Add(long type, CTCBase* tc) { Types[Count] = type; Components[Count] = tc; ++Count; }
    bool IsDead() const { return Dead; }
    template <class T> bool GetTC(T** out)
    {
        for (int i = 0; i < Count; ++i)
            if (Types[i] == T::INTERFACE_TYPE) {
                *out = (T*)Components[i];
                return true;
            }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
static int g_getPItem = 0;
template <class T> class CIntelligentPointer {
public:
    CThing* Item;
    CThing* GetPItem() const { ++g_getPItem; return Item; }
    T* operator->() const { return (T*)GetPItem(); }
};
class CGameCameraManager {
public:
    void PushSpecialCameraMode() { Log("[special]"); }
    void SetAutomaticCameraControl() { Log("[auto]"); }
};
class CCameraManagerSource {
public:
    CGameCameraManager* Camera;
    bool GetCameraManager(CGameCameraManager** camera) { if (!Camera) return false; *camera = Camera; return true; }
};
class CWorld { public: CCameraManagerSource* CameraSource; };
enum EPlayerMode { PLAYER_MODE_NONE = 0 };
class CProcessedInput;
static long g_removedMode = -1;
class CPlayerGui {
public:
    bool DisplayBottomHalfGUI;
    void SetDisplayBottomHalfGUI(bool display) { DisplayBottomHalfGUI = display; }
};
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CPlayer {
public:
    CWorld* World;
    CAInputProcess* InputProcessLeftStickLookAround_ptr;
    CAInputProcess* InputProcessBButtonExitMode_ptr;
    CAInputProcess* InputProcessToggleViewHeroMode_ptr;
    void RemoveMode(EPlayerMode mode) { g_removedMode = mode; Log("[remove]"); }
    void UninitModeViewHero(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::UninitModeViewHero(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    InputProcessLeftStickLookAround_ptr->RemoveFromInterface();
    InputProcessBButtonExitMode_ptr->RemoveFromInterface();
    InputProcessToggleViewHeroMode_ptr->RemoveFromInterface();
    g_pPlayerGui->SetDisplayBottomHalfGUI(true);
    CGameCameraManager* camera;
    World->CameraSource->GetCameraManager(&camera);
    camera->SetAutomaticCameraControl();
    RemoveMode((EPlayerMode)0x2f);
}
static void run()
{
    CGameCameraManager cam; CCameraManagerSource src; src.Camera = &cam; CWorld world; world.CameraSource = &src;
    CAInputProcess ls("LS"), b("B"), tog("TOG");
    CPlayer p; p.World = &world;
    p.InputProcessLeftStickLookAround_ptr = &ls; p.InputProcessBButtonExitMode_ptr = &b; p.InputProcessToggleViewHeroMode_ptr = &tog;
    g_gui.DisplayBottomHalfGUI = false;
    p.UninitModeViewHero(PLAYER_MODE_NONE, 0);
    CHECK(std::strcmp(g_log, "-LS-B-TOG[auto][remove]") == 0);
    CHECK(g_gui.DisplayBottomHalfGUI);
    CHECK(g_removedMode == 0x2f);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00634560_TEST FAIL\n");
        return 1;
    }
    std::printf("00634560_TEST PASS\n");
    return 0;
}