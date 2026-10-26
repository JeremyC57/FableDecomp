// Behaviour test for 00635a50: the model below is a copy of src/00635a50.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
// test-side stand-in for engine/CPlayer.h (only the members these bodies touch; host layout)
class CGamePlayerInterface; class CWorld;
class CPlayer { public:
    void* __vftable;
    CGamePlayerInterface* PlayerInterface;
    CWorld* World;
    void* PlayerCharacter[2];
    void* InputProcessBoastUI_ptr; void* InputProcessControlCreature_ptr; void* InputProcessRightStickLookAround_ptr;
    void* InputProcessCycleSpecialCameraModes_ptr; void* InputProcessCreatureMovement_ptr;
    void* InputProcessWatchForControlAngleChange_ptr; void* InputProcessControlCreatureActivateZTarget_ptr;
    void* InputProcessFirstPersonTargeting_ptr; void* InputProcessUseRangedWeapon_ptr; void* InputProcessLeftStickLookAround_ptr;
};

typedef long EPlayerMode;
class CProcessedInput;
class CTCBase;

class CAInputProcess { public:
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CMode {
    virtual void v00();
    virtual unsigned long GetModeType();                // slot 1
};
class CTCCreatureModeManager { public:                         // interface 0x31
    CMode* GetCurrentMode();                            // 0x0073A6E0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};

class CGameCameraManager { public:
    void PushSpecialCameraMode();                       // 0x00698F50
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                 // +0x34
};

struct CPlayer_Methods : CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
    void InitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessFirstPersonTargeting_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessLeftStickLookAround_ptr)->AddToInterface(PlayerInterface);
    CThing* hero = GetHeroThing();
    if (IsThingAlive(hero)) {
        CTCCreatureModeManager* modes = 0;
        hero->GetInterface(&modes, 0x31);
        if (modes->GetCurrentMode()->GetModeType() != 25)
            modes->AddMode(25);
        CGameCameraManager* camera = 0;
        if (World->CameraSource->GetCameraManager(&camera))
            camera->PushSpecialCameraMode();
    }
}

// ---- test doubles and driver ----
static int g_log[16]; static int g_n = 0;
static void Log(int e) { if (g_n < 16) g_log[g_n++] = e; }
static unsigned long g_modeType = 0; static unsigned long g_added = 0; static int g_adds = 0; static int g_pushes = 0;
static bool g_haveCamera = true; static CThing* g_hero = 0;
struct CProc : CAInputProcess { int id; CProc(int i) : id(i) {} bool AddToInterface(CGamePlayerInterface*) { Log(id); return true; } };
void CAInputProcess::VFunc00() {}
bool CAInputProcess::AddToInterface(CGamePlayerInterface*) { return false; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    for (SEntry* e = first; e != last; ++e) if (e->type >= *type) return e;
    return last;
}
struct CTestMode : CMode { unsigned long GetModeType() { return g_modeType; } };
static CTestMode g_mode;
void CMode::v00() {}
unsigned long CMode::GetModeType() { return 0; }
CMode* CTCCreatureModeManager::GetCurrentMode() { return &g_mode; }
bool CTCCreatureModeManager::AddMode(unsigned long m) { g_added = m; ++g_adds; return true; }
static CGameCameraManager g_cam;
void CGameCameraManager::PushSpecialCameraMode() { ++g_pushes; }
struct CSrc : CCameraManagerSource { bool GetCameraManager(CGameCameraManager** c) { if (g_haveCamera) { *c = &g_cam; return true; } return false; } };
#define V(n) void CCameraManagerSource::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08)
bool CCameraManagerSource::GetCameraManager(CGameCameraManager**) { return false; }
CThing* CPlayer_Methods::GetHeroThing() { return g_hero; }
int main()
{
    static CProc p1(1), p2(2), p3(3), p4(4);
    static CSrc src; static CWorld world; world.CameraSource = &src;
    static CTCCreatureModeManager modes;
    static CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0; entries[1].type = 0x31; entries[1].component = (CTCBase*)&modes;
    static CThing hero; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    hero.m_presentBits[0x31 >> 5] = 1UL << (0x31 & 31);
    static CPlayer_Methods p; p.World = &world;
    p.InputProcessControlCreatureActivateZTarget_ptr = &p1; p.InputProcessFirstPersonTargeting_ptr = &p2;
    p.InputProcessUseRangedWeapon_ptr = &p3; p.InputProcessLeftStickLookAround_ptr = &p4;
    bool ok = true;
    p.InitModeFirstPersonTargeting(0, 0);        // no hero: processes only
    static const int order[] = { 1, 2, 3, 4 };
    if (g_n != 4 || std::memcmp(g_log, order, sizeof(order)) || g_adds || g_pushes) ok = false;
    g_hero = &hero; g_modeType = 3;
    p.InitModeFirstPersonTargeting(0, 0);
    if (g_adds != 1 || g_added != 25 || g_pushes != 1) ok = false;
    g_modeType = 25;                              // already in mode 25: no AddMode
    p.InitModeFirstPersonTargeting(0, 0);
    if (g_adds != 1 || g_pushes != 2) ok = false;
    g_haveCamera = false;                         // camera manager not available: no push
    p.InitModeFirstPersonTargeting(0, 0);
    if (g_pushes != 2) ok = false;
    g_haveCamera = true; hero.m_stateFlags = 1;   // dead hero
    p.InitModeFirstPersonTargeting(0, 0);
    if (g_pushes != 2 || g_adds != 1) ok = false;
    if (!ok) { std::printf("00635a50_TEST FAIL\n"); return 1; }
    std::printf("00635a50_TEST PASS\n");
    return 0;
}