// 00636b00 behaviour test for CPlayer::UninitModeTakePhotojournalScreenshot(EPlayerMode, CProcessedInput*)
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
class CTCPhotojournalCapture : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0x10E };
    void ClosePhotojournalCapture() { Log("[close]"); }
};
class CPlayer {
public:
    CGamePlayerInterface* PlayerInterface;
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;
    CAInputProcess* InputProcessMain_ptr;
    CAInputProcess* InputProcessPhotojournalCapture_ptr;
    void UninitModeTakePhotojournalScreenshot(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::UninitModeTakePhotojournalScreenshot(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    CIntelligentPointer<CThingPlayerCreature>& character = PlayerCharacter;
    CTCPhotojournalCapture* capture = 0;
    character->GetTC(&capture);
    capture->ClosePhotojournalCapture();
    InputProcessPhotojournalCapture_ptr->RemoveFromInterface();
    InputProcessMain_ptr->AddToInterface(*PlayerInterface);
}
static void run()
{
    CGamePlayerInterface gpi; CAInputProcess mainProcess("MAIN"), photo("PHOTO");
    CThingPlayerCreature hero; CTCPhotojournalCapture cap; hero.Add(0x10E, &cap);
    CPlayer p; p.PlayerInterface = &gpi; p.InputProcessMain_ptr = &mainProcess; p.InputProcessPhotojournalCapture_ptr = &photo;
    p.PlayerCharacter.Item = &hero;
    p.UninitModeTakePhotojournalScreenshot(PLAYER_MODE_NONE, 0);
    CHECK(std::strcmp(g_log, "[close]-PHOTO+MAIN") == 0);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00636b00_TEST FAIL\n");
        return 1;
    }
    std::printf("00636b00_TEST PASS\n");
    return 0;
}