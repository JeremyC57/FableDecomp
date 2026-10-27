// 00633530 behaviour test for CPlayer::InitModeFirstPerson(EPlayerMode, CProcessedInput*)
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
class CPlayer {
public:
    CGamePlayerInterface* PlayerInterface;
    CWorld* World;
    CAInputProcess* InputProcessFirstPerson_ptr;
    CAInputProcess* InputProcessRightStickLookAround_ptr;
    CAInputProcess* InputProcessCycleSpecialCameraModes_ptr;
    void InitModeFirstPerson(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::InitModeFirstPerson(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    InputProcessFirstPerson_ptr->AddToInterface(*PlayerInterface);
    InputProcessRightStickLookAround_ptr->AddToInterface(*PlayerInterface);
    InputProcessCycleSpecialCameraModes_ptr->AddToInterface(*PlayerInterface);
    CGameCameraManager* camera = 0;
    if (World->CameraSource->GetCameraManager(&camera))
        camera->PushSpecialCameraMode();
}
static void run()
{
    CGamePlayerInterface gpi; CGameCameraManager cam; CCameraManagerSource src; src.Camera = &cam; CWorld world; world.CameraSource = &src;
    CAInputProcess fp("FP"), look("LOOK"), cyc("CYC");
    CPlayer p; p.PlayerInterface = &gpi; p.World = &world;
    p.InputProcessFirstPerson_ptr = &fp; p.InputProcessRightStickLookAround_ptr = &look; p.InputProcessCycleSpecialCameraModes_ptr = &cyc;
    p.InitModeFirstPerson(PLAYER_MODE_NONE, 0);
    CHECK(std::strcmp(g_log, "+FP+LOOK+CYC[special]") == 0);
    g_log[0] = 0; src.Camera = 0;
    p.InitModeFirstPerson(PLAYER_MODE_NONE, 0);
    CHECK(std::strcmp(g_log, "+FP+LOOK+CYC") == 0);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00633530_TEST FAIL\n");
        return 1;
    }
    std::printf("00633530_TEST PASS\n");
    return 0;
}