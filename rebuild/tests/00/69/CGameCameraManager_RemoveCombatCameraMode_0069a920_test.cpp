// Behaviour test for 0069a920: src/0069a920.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CGameCameraManager::RemoveCombatCameraMode()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameCameraManager::RemoveCombatCameraMode() -- unless the main player is in mode 0x2f, look up the camera manager set's main camera manager def, install it (SetCameraManagerDef) and clear ObservedThingInMelee
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069A920, length 104 bytes (+ int3 padding).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::HasMode(EPlayerMode) @ 0x00634160 (as the landed RemoveBuildingCameraMode
// 0x0069B610), CDefinitionManager::GetDef(long, CDefPointer&) @ 0x0069CE80 (the world's definition manager at +0x10),
// CGameCameraManager::SetCameraManagerDef @ 0x0069A850 (landed), the def's virtual slot 1 (Destroy) in the local's destructor.
// Globals: the engine singleton @ 0x013B86A0 (player manager at +0x1c).
// Layout: retail CGameCameraManager as the landed SetCameraManagerDef / RemoveBuildingCameraMode (World +0x74, ObservedThingInMelee
// +0xa9, PCameraManagerSet +0x130); the set def's main manager def index at +0x44.
// Shape: `push 2Fh` before the GetMainPlayer call (the HasMode argument is evaluated before the object expression); the local
// CDefPointer's zero initialisation is scheduled after the GetDef argument pushes; its destructor releases with `dec [ecx+4] ; jne`.
class CPlayer { public:
    bool HasMode(long mode);                        // 0x00634160
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                       // 0x00449970
};
class CEngine { public:
    unsigned char _pad[0x1c];
    CPlayerManager* m_pPlayerManager;               // +0x1c
};
extern CEngine* g_pEngine;                          // 0x013B86A0
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1: runs when the last reference goes
    long m_refCount;                                // +4
    void Release()
    {
        long& count = m_refCount;
        if (--count == 0)
            Destroy();
    }
};
class CCameraManagerSetDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x3c];
    long          MainCameraManagerDef;             // +0x44 global def index of the set's main camera manager def
};
template <class T>
class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        T* object = m_pObject;
        if (object)
            object->Release();
    }
    T* operator->() const { return m_pObject; }
    void Release()
    {
        T* old = m_pObject;                         // a local copy: retail decrements through the register (`dec [ecx+4] ; jne`)
        if (old) {
            if (--old->m_refCount == 0) old->Destroy();
            m_pObject = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            Release();
            m_pObject = other.m_pObject;            // re-read after Destroy (`mov eax,[edi]`)
            if (m_pObject) ++m_pObject->m_refCount;
        }
        return *this;
    }
};
class CCameraManagerDef : public CDefPointeeBase { public: };

class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CCameraManagerDef>& out);   // 0x0069CE80 (manifest names the CThingPhysicalSwitchDef instantiation: identical body)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CCameraManagerDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
class CWorld { public:
    unsigned char       _pad[0x10];
    CDefinitionManager* m_pDefinitionManager;      // +0x10
};
class CGameCameraManager { public:
    void*         __vftable;                        // +0x00
    unsigned char _pad04[0x70];                     // +0x04 Camera, OldCamera
    CWorld*       World;                            // +0x74
    unsigned char _pad78[0x30];                     // +0x78 CamTools, CurrentMode, PendingMode
    bool          NewAutoMode;                      // +0xa8
    bool          ObservedThingInMelee;             // +0xa9
    bool          ObservedThingInBuilding;          // +0xaa
    unsigned char _padab[0x85];                     // +0xab
    CDefPointer<CCameraManagerSetDef> PCameraManagerSet;   // +0x130
    CDefPointer<CCameraManagerDef>    PDef;                // +0x134
    void PopulateSwitchList();                      // 0x0069A010
    void PopulateListFromData();                    // 0x0069A1A0
    void PushCurrentManualMode();                   // 0x006988B0
    void SetCameraManagerDef(const CDefPointer<CCameraManagerDef>& def);   // 0x0069A850 (landed)
    void RemoveCombatCameraMode();
};

// Same-TU body of the landed SetCameraManagerDef (0x0069A850, out of line in retail): with the body in view VC7.1 knows
// the def pointer's address goes no further than the copy, so the release loads the slot straight into ecx.
#pragma auto_inline(off)
__declspec(noinline) void CGameCameraManager::SetCameraManagerDef(const CDefPointer<CCameraManagerDef>& def)
{
    PDef = def;
    PopulateSwitchList();
    PopulateListFromData();
    PushCurrentManualMode();
}
#pragma auto_inline(on)
void CGameCameraManager::RemoveCombatCameraMode()
{
    if (!g_pEngine->m_pPlayerManager->GetMainPlayer()->HasMode(0x2f)) {
        CDefPointer<CCameraManagerDef> def;
        World->m_pDefinitionManager->GetDef(PCameraManagerSet->MainCameraManagerDef, def);
        SetCameraManagerDef(def);
        ObservedThingInMelee = false;
    }
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0; static bool g_hasMode = false; static long g_modeAsked = -1; static int g_seq = 0, g_switch = 0, g_list = 0, g_push = 0;
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; }
bool CPlayer::HasMode(long mode) { g_modeAsked = mode; return g_hasMode; }
static CPlayer g_player; static CPlayerManager g_pm;
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
static CEngine g_engine; CEngine* g_pEngine = &g_engine;
void CGameCameraManager::PopulateSwitchList() { g_switch = ++g_seq; }
void CGameCameraManager::PopulateListFromData() { g_list = ++g_seq; }
void CGameCameraManager::PushCurrentManualMode() { g_push = ++g_seq; }
int main()
{
    g_engine.m_pPlayerManager = &g_pm;
    CCameraManagerSetDef set; set.m_refCount = 100; set.MainCameraManagerDef = 9;
    CCameraManagerDef camDef; camDef.m_refCount = 1; g_def = &camDef;
    CDefinitionManager dm; CWorld world; memset(&world, 0, sizeof(world)); world.m_pDefinitionManager = &dm;
    static CGameCameraManager m; memset(&m, 0, sizeof(m)); m.World = &world; m.PCameraManagerSet.m_pObject = &set; m.ObservedThingInMelee = true;
    g_hasMode = true; m.RemoveCombatCameraMode(); CHECK(g_modeAsked == 0x2f); CHECK(m.ObservedThingInMelee); CHECK(m.PDef.m_pObject == 0);
    g_hasMode = false; m.RemoveCombatCameraMode();
    CHECK(!m.ObservedThingInMelee); CHECK(m.PDef.m_pObject == &camDef); CHECK(camDef.m_refCount == 1); CHECK(g_switch == 1 && g_list == 2 && g_push == 3); CHECK(g_destroyed == 0);
    // the same def again: no reassignment, the lookup's reference is released
    camDef.m_refCount = 2; m.RemoveCombatCameraMode(); CHECK(camDef.m_refCount == 1); CHECK(g_destroyed == 0);
    // no def: the old one is released by the assignment
    g_def = 0; m.RemoveCombatCameraMode(); CHECK(m.PDef.m_pObject == 0); CHECK(g_destroyed == 1);
    m.PCameraManagerSet.m_pObject = 0;
    if (fails == 0) printf("0069a920_TEST PASS\n"); else printf("0069a920_TEST FAIL\n");
    return fails;
}