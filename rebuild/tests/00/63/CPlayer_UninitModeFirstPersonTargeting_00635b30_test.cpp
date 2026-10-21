// CPlayer::UninitModeFirstPersonTargeting(EPlayerMode, CProcessedInput*) -- detach the first-person targeting input processes and, for a living hero, take its creature mode manager out of mode 25 and push the camera's current manual mode; then drop player mode 22
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00635B30, length 192 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?UninitModeFirstPersonTargeting@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z` (a
// CPlayer::PlayerModeInfo UninitFunc; InitModeFirstPersonTargeting is 0x00635A50).
// Callees: CAInputProcess::RemoveFromInterface 0x00A0D2D0 (four processes), CPlayer::GetHeroThing 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetInterface, type 0x31 = the creature mode
// manager, bit 17 of +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, the world's +0x34 object's
// slot 9 (GetCameraManager out-parameter, +0x24), CGameCameraManager::PushCurrentManualMode 0x006988B0,
// CPlayer::RemoveMode(EPlayerMode) 0x00634190.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, World +0x1C) through the lane's
// CPlayer_Methods carrier; CThing interface bitset +0x20, interface map +0x44, dead bit +0x91 & 1.
// Shape: the mode manager is zero-initialised and filled by the out-parameter GetInterface (`xor ecx,ecx` before the
// bitset test, the component loaded straight into ecx for its one use); the camera manager out pointer is
// zero-initialised in its frame word and the getter's bool guards the push; RemoveMode(22) runs on every path.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
struct CProcessedInput;
struct CTCBase;

struct CAInputProcess {
    void RemoveFromInterface();                         // 0x00A0D2D0
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
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
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CTCCreatureModeManager {                         // interface 0x31
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};

struct CGameCameraManager {
    void PushCurrentManualMode();                       // 0x006988B0
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                 // +0x34
};

struct CPlayer_Methods : CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                  // 0x00634190
    void UninitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::UninitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessFirstPersonTargeting_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessLeftStickLookAround_ptr)->RemoveFromInterface();
    CThing* hero = GetHeroThing();
    if (IsThingAlive(hero)) {
        CTCCreatureModeManager* modes = 0;
        hero->GetInterface(&modes, 0x31);
        modes->RemoveMode(25);
        CGameCameraManager* camera = 0;
        if (World->CameraSource->GetCameraManager(&camera))
            camera->PushCurrentManualMode();
    }
    RemoveMode(22);
}

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_log[16]; static int g_n = 0;
struct CProc { int id; };
void CAInputProcess::RemoveFromInterface() { if (g_n < 16) g_log[g_n++] = ((CProc*)this)->id; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static int g_modeRemoves = 0, g_pushes = 0, g_playerRemoves = 0;
static unsigned long g_lastMode = 0; static long g_lastPlayerMode = 0;
static const void* g_lastModes = 0;
static bool g_haveCamera = true;
static CThing* g_hero = 0;
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { ++g_modeRemoves; g_lastMode = mode; g_lastModes = this; }
void CGameCameraManager::PushCurrentManualMode() { ++g_pushes; }
static CGameCameraManager g_camera;
void CCameraManagerSource::v00() { } void CCameraManagerSource::v01() { } void CCameraManagerSource::v02() { }
void CCameraManagerSource::v03() { } void CCameraManagerSource::v04() { } void CCameraManagerSource::v05() { }
void CCameraManagerSource::v06() { } void CCameraManagerSource::v07() { } void CCameraManagerSource::v08() { }
bool CCameraManagerSource::GetCameraManager(CGameCameraManager** camera)
{
    if (!g_haveCamera)
        return false;
    *camera = &g_camera;
    return true;
}
CThing* CPlayer_Methods::GetHeroThing() { return g_hero; }
void CPlayer_Methods::RemoveMode(EPlayerMode mode) { ++g_playerRemoves; g_lastPlayerMode = mode; }
static CProc g_p1 = { 1 }, g_p2 = { 2 }, g_p3 = { 3 }, g_p4 = { 4 };
static CCameraManagerSource g_source;
static CWorld g_world;
static CTCCreatureModeManager g_modes;
static CInterfaceMap::SEntry g_entries[2];
static CThing g_heroThing;
static CPlayer_Methods g_player;
int main()
{
    g_world.CameraSource = &g_source;
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x31; g_entries[1].component = (CTCBase*)&g_modes;
    g_heroThing.m_interfaces.first = g_entries; g_heroThing.m_interfaces.last = g_entries + 2;
    g_heroThing.m_presentBits[0x31 >> 5] = 1UL << (0x31 & 31);
    g_player.World = &g_world;
    g_player.InputProcessControlCreatureActivateZTarget_ptr = reinterpret_cast<CInputProcessControlCreatureActivateZTarget*>(&g_p1);
    g_player.InputProcessFirstPersonTargeting_ptr = reinterpret_cast<CInputProcessFirstPersonTargeting*>(&g_p2);
    g_player.InputProcessUseRangedWeapon_ptr = reinterpret_cast<CInputProcessUseRangedWeapon*>(&g_p3);
    g_player.InputProcessLeftStickLookAround_ptr = reinterpret_cast<CInputProcessFirstPersonLookAround*>(&g_p4);
    g_player.UninitModeFirstPersonTargeting(0, 0);    // no hero: the four processes and the player mode only
    static const int order[] = { 1, 2, 3, 4 };
    CHECK(g_n == 4 && memcmp(g_log, order, sizeof order) == 0);
    CHECK(g_modeRemoves == 0 && g_pushes == 0 && g_playerRemoves == 1 && g_lastPlayerMode == 22);
    g_hero = &g_heroThing;
    g_player.UninitModeFirstPersonTargeting(0, 0);
    CHECK(g_modeRemoves == 1 && g_lastMode == 25 && g_lastModes == &g_modes && g_pushes == 1 && g_playerRemoves == 2);
    g_haveCamera = false;                             // no camera manager: no push
    g_player.UninitModeFirstPersonTargeting(0, 0);
    CHECK(g_modeRemoves == 2 && g_pushes == 1);
    g_haveCamera = true; g_heroThing.m_stateFlags = 1;   // dead hero
    g_player.UninitModeFirstPersonTargeting(0, 0);
    CHECK(g_modeRemoves == 2 && g_pushes == 1 && g_playerRemoves == 4 && g_n == 16);
    if (fails == 0) printf("00635b30_TEST PASS\n");
    return fails;
}