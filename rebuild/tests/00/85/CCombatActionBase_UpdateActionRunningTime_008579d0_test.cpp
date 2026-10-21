// Behaviour test for 008579d0: src/008579d0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CCombatActionBase::UpdateActionRunningTime
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatActionBase::UpdateActionRunningTime() -- while a scripted camera runs on a camera point, advance the running time scaled by the point's speed and drive bullet time; otherwise the base update
// Leaf: ?UpdateActionRunningTime@CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008579D0, 416 bytes, plain `ret` (virtual).
// Callees: CCreatureActionBase::GetPWorld @ 0x00692940, the current game camera (world +0x34) slot 9 (+0x24, its camera
//   manager by out pointer), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CGameCameraManager::IsUsingScriptedCameraMode
//   @ 0x00697AE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (const copy) @ 0x004365B0, the interface-0x3F component
//   slot 28 (+0x70, its point type), virtual GetAnimDelayTime (slot 6) / GetAnimIndex (slot 7),
//   CTCAnimationComplex::GetAnimDuration(long) @ 0x006627F0 (result dropped), CBulletTimeManager::IsTimeForFullUpdate
//   @ 0x004C6060, SetBulletTimeSlowdownMultiplier(long, CCombatActionBase*, float) @ 0x004C6170, __ftol2 @ 0x00BFEA70,
//   CCreatureActionBase::UpdateActionRunningTime @ 0x00692480.
// Globals: frames per second @ 0x01375550; float pool 1.0f @ 0x0122DED8, 1e-4f @ 0x0129BA3C.
// Layout (retail): CCreatureActionBase is 0xA8 bytes: PCreature +0x08, RunningTimeSeconds +0x40,
//   LastRunningTimeSeconds +0x44; CCombatActionBase PCameraPoint +0xB0 (PDB 0xBC - 0x0C), the strike-interrupt time
//   +0xC4 (PDB StrikeEventInterruptTime 0xD8 - 0x14); CWorld PCurrentGameCamera +0x34, bullet-time manager +0x68;
//   CThing dead flag byte +0x91 bit 0, interface bitset +0x20, interface map +0x44.
// Shape: the point component is fetched through an out-parameter GetInterface left unset when absent; the scripted
//   point is a second uninitialised local in the same frame word.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    template <class T> __forceinline void GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
        }
    }
};

struct CTCCameraPoint {                             // interface 0x3F component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetPointType();                    // slot 28 (+0x70)
    unsigned char _pad04[0xbc - 4];
    float m_speed;                                  // +0xbc
};

struct CGameCameraManager {
    bool IsUsingScriptedCameraMode();               // 0x00697AE0
};

struct CGameCamera {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};

struct CBulletTimeManager {
    unsigned char _pad0[4];
    long m_active;                                  // +0x04
    unsigned char _pad8[4];
    bool m_slowing;                                 // +0x0c
    __forceinline bool IsRunning() const { return m_slowing || m_active != 0; }
    bool IsTimeForFullUpdate();                     // 0x004C6060
    void SetBulletTimeSlowdownMultiplier(long frames, struct CCombatActionBase* source, float multiplier);   // 0x004C6170
};

struct CWorld {
    unsigned char _pad0[0x34];
    CGameCamera* PCurrentGameCamera;                // +0x34
    unsigned char _pad38[0x68 - 0x38];
    CBulletTimeManager* PBulletTime;                // +0x68
    CGameCamera* GetCurrentGameCamera() const { return PCurrentGameCamera; }
    CBulletTimeManager* GetBulletTime() const { return PBulletTime; }
};

struct CThingCreatureBase {
    float GetAnimDuration(long animIndex);          // 0x006627F0
};

struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CCreaturePointer {
    void* __vftable;
    void* m_pItem;
    CThingCreatureBase* GetPItem() const;           // 0x00A01B50
};

extern long g_framesPerSecond;                      // 0x01375550

struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetAnimDelayTime();                // slot 6 (+0x18)
    virtual long GetAnimIndex();                    // slot 7 (+0x1c)
    unsigned char _pad04[4];
    CCreaturePointer PCreature;                     // +0x08
    unsigned char _pad10[0x40 - 0x10];
    float RunningTimeSeconds;                       // +0x40
    float LastRunningTimeSeconds;                   // +0x44
    unsigned char _pad48[0xa8 - 0x48];
    CWorld* GetPWorld();                            // 0x00692940
    void UpdateActionRunningTime();                 // 0x00692480
};

struct CCombatActionBase : CCreatureActionBase {
    unsigned char _pada8[8];
    CBaseIntelligentPointer PCameraPoint;           // +0xb0
    unsigned char _padb8[0xc4 - 0xb8];
    float m_scriptedTime;                           // +0xc4
    void UpdateActionRunningTime();
};

void CCombatActionBase::UpdateActionRunningTime()
{
    CGameCameraManager* cameraManager;
    if (GetPWorld()->GetCurrentGameCamera()->GetCameraManager(&cameraManager)
        && PCameraPoint.GetPItem() != 0
        && !(PCameraPoint.GetPItem()->m_flags91 & 1)
        && cameraManager->IsUsingScriptedCameraMode()) {
        CTCCameraPoint* point;
        PCameraPoint.GetPItem()->GetInterface(0x3f, &point);
        CTCCameraPoint* scripted;
        if (point->GetPointType() == 7)
            scripted = point;
        float speed = scripted->m_speed;
        long framesPerSecond = g_framesPerSecond;
        float fps = (float)framesPerSecond;
        float delay = (float)GetAnimDelayTime() / fps;
        CThingCreatureBase* creature = PCreature.GetPItem();
        creature->GetAnimDuration(GetAnimIndex());
        RunningTimeSeconds = (m_scriptedTime - delay) * speed + delay;
        float step = RunningTimeSeconds - LastRunningTimeSeconds;
        float frameTime = 1.0f / framesPerSecond;
        if (step > 0.0001f) {
            if (!GetPWorld()->PBulletTime->IsRunning() || GetPWorld()->PBulletTime->IsTimeForFullUpdate()) {
                long frames = (long)(1.0f / (step / frameTime));
                if (frames < 1)
                    frames = 1;
                GetPWorld()->GetBulletTime()->SetBulletTimeSlowdownMultiplier(frames, this, 0.5f);
            }
        }
        return;
    }
    CCreatureActionBase::UpdateActionRunningTime();
}

// ---- stubs ----
void CTCCameraPoint::v00() {}
void CTCCameraPoint::v01() {}
void CTCCameraPoint::v02() {}
void CTCCameraPoint::v03() {}
void CTCCameraPoint::v04() {}
void CTCCameraPoint::v05() {}
void CTCCameraPoint::v06() {}
void CTCCameraPoint::v07() {}
void CTCCameraPoint::v08() {}
void CTCCameraPoint::v09() {}
void CTCCameraPoint::v10() {}
void CTCCameraPoint::v11() {}
void CTCCameraPoint::v12() {}
void CTCCameraPoint::v13() {}
void CTCCameraPoint::v14() {}
void CTCCameraPoint::v15() {}
void CTCCameraPoint::v16() {}
void CTCCameraPoint::v17() {}
void CTCCameraPoint::v18() {}
void CTCCameraPoint::v19() {}
void CTCCameraPoint::v20() {}
void CTCCameraPoint::v21() {}
void CTCCameraPoint::v22() {}
void CTCCameraPoint::v23() {}
void CTCCameraPoint::v24() {}
void CTCCameraPoint::v25() {}
void CTCCameraPoint::v26() {}
void CTCCameraPoint::v27() {}
void CGameCamera::v00() {}
void CGameCamera::v01() {}
void CGameCamera::v02() {}
void CGameCamera::v03() {}
void CGameCamera::v04() {}
void CGameCamera::v05() {}
void CGameCamera::v06() {}
void CGameCamera::v07() {}
void CGameCamera::v08() {}
void CCreatureActionBase::v00() {}
void CCreatureActionBase::v01() {}
void CCreatureActionBase::v02() {}
void CCreatureActionBase::v03() {}
void CCreatureActionBase::v04() {}
void CCreatureActionBase::v05() {}
static long g_ptype; long CTCCameraPoint::GetPointType() { return g_ptype; }
static bool g_scripted; bool CGameCameraManager::IsUsingScriptedCameraMode() { return g_scripted; }
static CGameCameraManager g_cm; static bool g_hasCm;
bool CGameCamera::GetCameraManager(CGameCameraManager** out) { *out = &g_cm; return g_hasCm; }
static bool g_full; bool CBulletTimeManager::IsTimeForFullUpdate() { return g_full; }
static int g_bt; static long g_btFrames; static CCombatActionBase* g_btSource; static float g_btMul;
void CBulletTimeManager::SetBulletTimeSlowdownMultiplier(long frames, CCombatActionBase* source, float multiplier)
{ ++g_bt; g_btFrames = frames; g_btSource = source; g_btMul = multiplier; }
static long g_durIndex; static CThingCreatureBase* g_durThis;
float CThingCreatureBase::GetAnimDuration(long animIndex) { g_durIndex = animIndex; g_durThis = this; return 9.0f; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
CThingCreatureBase* CCreaturePointer::GetPItem() const { return (CThingCreatureBase*)m_pItem; }
long g_framesPerSecond;
long CCreatureActionBase::GetAnimDelayTime() { return 15; }
long CCreatureActionBase::GetAnimIndex() { return 3; }
static CWorld g_world;
CWorld* CCreatureActionBase::GetPWorld() { return &g_world; }
static int g_baseUpd; void CCreatureActionBase::UpdateActionRunningTime() { ++g_baseUpd; }
static void Reset() { g_bt = 0; g_btFrames = 0; g_btSource = 0; g_btMul = 0; g_durIndex = 0; g_durThis = 0; g_baseUpd = 0; }
int main()
{
    static CThing pointThing; static CThingCreatureBase creature; static CBulletTimeManager bt; static CGameCamera camera;
    static CTCCameraPoint point;
    point.m_speed = 2.0f; g_ptype = 7; g_framesPerSecond = 30;
    CInterfaceMap::SEntry pe[] = { { 2, 0 }, { 0x3f, (CTCBase*)&point }, { 0x50, 0 } };
    for (int i = 0; i < 3; ++i) pointThing.m_presentBits[pe[i].type >> 5] |= 1UL << (pe[i].type & 31);
    pointThing.m_interfaces.first = pe; pointThing.m_interfaces.last = pe + 3;
    g_world.PCurrentGameCamera = &camera; g_world.PBulletTime = &bt;
    CCombatActionBase act;
    act.PCreature.m_pItem = &creature; act.PCameraPoint.m_pItem = &pointThing; act.m_scriptedTime = 1.5f;
    act.LastRunningTimeSeconds = 2.49f;
    g_hasCm = true; g_scripted = true; bt.m_active = 0; bt.m_slowing = false;
    // scripted camera: running time = (1.5 - 0.5) * 2 + 0.5; small step -> bullet time 3 frames
    Reset();
    act.UpdateActionRunningTime();
    CHECK(act.RunningTimeSeconds == 2.5f && g_baseUpd == 0 && g_durIndex == 3 && g_durThis == &creature);
    CHECK(g_bt == 1 && g_btFrames == 3 && g_btSource == &act && g_btMul == 0.5f);
    // big step -> at least one frame
    act.LastRunningTimeSeconds = 1.0f; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_bt == 1 && g_btFrames == 1);
    // no forward step: no bullet time
    act.LastRunningTimeSeconds = 2.5f; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_bt == 0 && act.RunningTimeSeconds == 2.5f && g_baseUpd == 0);
    // bullet time already running: only on a full update
    act.LastRunningTimeSeconds = 2.49f; bt.m_active = 1; g_full = false; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_bt == 0 && g_baseUpd == 0);
    g_full = true; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_bt == 1 && g_btFrames == 3);
    bt.m_active = 0; bt.m_slowing = true; g_full = false; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_bt == 0);
    bt.m_slowing = false;
    // fall back to the base update
    g_scripted = false; Reset(); act.RunningTimeSeconds = -1.0f;
    act.UpdateActionRunningTime();
    CHECK(g_baseUpd == 1 && g_bt == 0 && act.RunningTimeSeconds == -1.0f);
    g_scripted = true; pointThing.m_flags91 = 1; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_baseUpd == 1 && g_bt == 0);
    pointThing.m_flags91 = 0; act.PCameraPoint.m_pItem = 0; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_baseUpd == 1);
    act.PCameraPoint.m_pItem = &pointThing; g_hasCm = false; Reset();
    act.UpdateActionRunningTime();
    CHECK(g_baseUpd == 1);
    if (fails == 0) printf("008579d0_TEST PASS\n"); else printf("008579d0_TEST FAIL\n");
    return fails;
}