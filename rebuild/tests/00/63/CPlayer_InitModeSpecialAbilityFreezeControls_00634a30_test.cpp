// 00634a30 behaviour test for CPlayer::InitModeSpecialAbilityFreezeControls(EPlayerMode, CProcessedInput*)
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
static int g_cancels = 0;
class CTCSpecialAbilities : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0xA7 };
    void CancelCurrentMutexAbility() { ++g_cancels; }
};
class CPlayer {
public:
    CGamePlayerInterface* PlayerInterface;
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;
    CAInputProcess* InputProcessFreezeControls_ptr;
    void InitModeSpecialAbilityFreezeControls(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::InitModeSpecialAbilityFreezeControls(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    InputProcessFreezeControls_ptr->AddToInterface(*PlayerInterface);
    CIntelligentPointer<CThingPlayerCreature>& character = PlayerCharacter;
    if (character.GetPItem() && !character->IsDead()) {
        CTCSpecialAbilities* abilities;
        if (character->GetTC(&abilities))
            abilities->CancelCurrentMutexAbility();
    }
}
static void run()
{
    CGamePlayerInterface gpi; CAInputProcess fr("FR");
    CThingPlayerCreature hero; CTCSpecialAbilities sa;
    CPlayer p; p.PlayerInterface = &gpi; p.InputProcessFreezeControls_ptr = &fr; p.PlayerCharacter.Item = 0;
    p.InitModeSpecialAbilityFreezeControls(PLAYER_MODE_NONE, 0);          // no character: only the process
    CHECK(std::strcmp(g_log, "+FR") == 0 && g_cancels == 0);
    p.PlayerCharacter.Item = &hero;
    p.InitModeSpecialAbilityFreezeControls(PLAYER_MODE_NONE, 0);          // no component
    CHECK(g_cancels == 0);
    hero.Add(0xA7, &sa);
    p.InitModeSpecialAbilityFreezeControls(PLAYER_MODE_NONE, 0);
    CHECK(g_cancels == 1);
    hero.Dead = 1;
    p.InitModeSpecialAbilityFreezeControls(PLAYER_MODE_NONE, 0);
    CHECK(g_cancels == 1);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00634a30_TEST FAIL\n");
        return 1;
    }
    std::printf("00634a30_TEST PASS\n");
    return 0;
}