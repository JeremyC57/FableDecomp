// Behaviour test for 00487ef0: the model below is a copy of src/00487ef0.cpp (compiled separately), driven by test doubles.
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
struct CBaseObject;
struct CThingManager;
struct CWorldMap;
struct CThingCreatureBase;

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngle();                     // slot 63 (+0xFC; CTCPhysicsStandard 0x00723B30: atan2 of the facing)
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60 (PDB +0x70)
    void SetPlayerNumber(long number);                  // 0x004C7440
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
struct CThingPlayerCreature;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CGameCameraManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26();
    virtual void SetMainThing(const CThingCreatureBase* thing);   // slot 27 (+0x6C, 0x00697C10)
    void ResetCamera();                                 // 0x0069A990
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();           // slot 3 (+0xC)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                   // slot 12 (+0x30)
    unsigned char         _pad04[0x34 - 0x04];
    CCameraManagerSource* CameraSource;                 // +0x34
};

struct CPlayerManager {
    long GetMainPlayerNumber();                         // 0x00449700
};

namespace NPlayerManager { struct CPlayerSave; }
typedef long EPlayerMode;

struct CPlayer {
    void*           __vftable;
    unsigned char   _pad04[0x10 - 0x04];
    CPlayerManager* PlayerManager;                      // +0x10
    unsigned char   _pad14[0x1c - 0x14];
    CWorld*         World;                              // +0x1c
    CThingManager*  ThingManager;                       // +0x20
    CWorldMap*      PWorldMap;                          // +0x24
    unsigned char   _pad28[0x34 - 0x28];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    bool LoadPlayerCharacterFromGameState(const NPlayerManager::CPlayerSave& save);   // 0x00487C20
    void SetControlledCreature(CThingPlayerCreature* creature);  // 0x00487CF0
    void AddMode(EPlayerMode mode, bool flag);          // 0x006345C0
    bool InitCharacterFromSave(const NPlayerManager::CPlayerSave& save);
};

struct CThingPlayerCreature : CThing { };

bool CPlayer::InitCharacterFromSave(const NPlayerManager::CPlayerSave& save)
{
    ThingManager = World->GetThingManager();
    PWorldMap = World->GetWorldMap();
    if (LoadPlayerCharacterFromGameState(save)) {
        SetControlledCreature(PlayerCharacter.GetPItem());
        PlayerCharacter.GetPItem()->SetPlayerNumber(PlayerManager->GetMainPlayerNumber());
        AddMode(1, true);
        PlayerCharacter.GetPItem()->GetPhysicsTC()->GetFacingAngle();
        {
            CGameCameraManager* camera;
            World->CameraSource->GetCameraManager(&camera);
            camera->SetMainThing((const CThingCreatureBase*)PlayerCharacter.GetPItem());
            camera->ResetCamera();
        }
        return true;
    }
    return false;
}

// ---- test doubles and driver ----
namespace NPlayerManager { struct CPlayerSave { long id; }; }
static int g_log[16]; static int g_n = 0;
static void Log(int e) { if (g_n < 16) g_log[g_n++] = e; }
static bool g_loadOk = true; static long g_number = -1; static long g_mode = -1; static bool g_modeFlag = false;
static CThingPlayerCreature* g_controlled = 0; static const CThingCreatureBase* g_mainThing = 0;
static CThingManager* const kTM = (CThingManager*)0x1234; static CWorldMap* const kWM = (CWorldMap*)0x5678;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::v00() {}
void CThing::SetPlayerNumber(long n) { g_number = n; Log(3); }
long CPlayerManager::GetMainPlayerNumber() { return 2; }
bool CPlayer::LoadPlayerCharacterFromGameState(const NPlayerManager::CPlayerSave&) { Log(1); return g_loadOk; }
void CPlayer::SetControlledCreature(CThingPlayerCreature* c) { g_controlled = c; Log(2); }
void CPlayer::AddMode(EPlayerMode m, bool f) { g_mode = m; g_modeFlag = f; Log(4); }
struct CPhys : CTCPhysicsBase { float GetFacingAngle() { Log(5); return 1.5f; } };
struct CCam : CGameCameraManager { void SetMainThing(const CThingCreatureBase* t) { g_mainThing = t; Log(7); } };
static CCam g_cam;
void CGameCameraManager::ResetCamera() { Log(8); }
struct CSrc : CCameraManagerSource { bool GetCameraManager(CGameCameraManager** c) { *c = &g_cam; Log(6); return true; } };
struct CW : CWorld { CThingManager* GetThingManager() { return kTM; } CWorldMap* GetWorldMap() { return kWM; } };
#define V(n) void CTCPhysicsBase::v##n() {} void CGameCameraManager::v##n() {} void CCameraManagerSource::v##n() {} void CWorld::v##n() {}
V(00) V(01) V(02) V(04) V(05) V(06) V(07) V(08)
#undef V
#define V(n) void CTCPhysicsBase::v##n() {} void CGameCameraManager::v##n() {} void CWorld::v##n() {}
V(09) V(10) V(11)
#undef V
void CCameraManagerSource::v03() {} void CTCPhysicsBase::v03() {} void CGameCameraManager::v03() {}
#define V(n) void CTCPhysicsBase::v##n() {} void CGameCameraManager::v##n() {}
V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25) V(26)
#undef V
#define V(n) void CTCPhysicsBase::v##n() {}
V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44)
V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62)
float CTCPhysicsBase::GetFacingAngle() { return 0; }
void CGameCameraManager::SetMainThing(const CThingCreatureBase*) {}
bool CCameraManagerSource::GetCameraManager(CGameCameraManager**) { return false; }
CThingManager* CWorld::GetThingManager() { return 0; }
CWorldMap* CWorld::GetWorldMap() { return 0; }
int main()
{
    static CPhys phys; static CSrc src; static CW world; static CPlayerManager pm;
    static CThingPlayerCreature hero; hero.PhysicsTC = &phys;
    world.CameraSource = &src;
    static CPlayer p; p.World = &world; p.PlayerManager = &pm; p.PlayerCharacter.PItem = (CBaseObject*)&hero;
    NPlayerManager::CPlayerSave save = { 1 };
    bool ok = true;
    if (!p.InitCharacterFromSave(save)) ok = false;
    if (p.ThingManager != kTM || p.PWorldMap != kWM || g_controlled != &hero || g_number != 2) ok = false;
    if (g_mode != 1 || !g_modeFlag || g_mainThing != (const CThingCreatureBase*)&hero) ok = false;
    static const int order[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    if (g_n != 8 || std::memcmp(g_log, order, sizeof(order)) != 0) ok = false;
    g_n = 0; g_loadOk = false; p.ThingManager = 0;
    if (p.InitCharacterFromSave(save)) ok = false;
    if (g_n != 1 || p.ThingManager != kTM) ok = false;
    if (!ok) { std::printf("00487ef0_TEST FAIL\n"); return 1; }
    std::printf("00487ef0_TEST PASS\n");
    return 0;
}