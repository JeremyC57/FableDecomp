// Behaviour test for CAIStateGroup_SummonerLightningAttack::IsRunnable (0x0096F770): model and body verbatim, every callee
// stubbed; covers no enemy source / no enemy / dead enemy, the line-of-sight probe (from = creature + 2z, to = enemy + 2z) and
// its failure, the out-of-range latch through CIntelligentPointerCreature, the preemptive frame gate, the seeded percentage
// roll against LightningChance, the LOS chance with CanBeTargeted, and the next-frame scheduling.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
// CAIStateGroup_SummonerLightningAttack::IsRunnable() -- latch a live enemy as the lightning target when it is in line of sight and beyond the def's attack range, or (once the preemptive frame is reached) by a random percentage roll against the def's chances; otherwise schedule the next preemptive check
//   SMatch out dies at the call (retail packs it with the IP temporaries and the LOS distance word: frame 0x44); the world is a
//   named local before the camera-manager call (this straight into ecx); the from-vector reads PBrain->PCreature directly.
// Leaf: IsRunnable
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
class C2DVector { public:
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) {}
    float GetLengthSquared() const { return x * x + y * y; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCEnemySource {                         // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
struct CTCLightningTarget {                     // interface 0x42
    struct SMatch { void* Object; void* Block; };
    bool CanBeTargeted(SMatch* out, CThing* const& attacker);   // 0x00970190
};
struct CTools {
    bool IsLineOfSightFiltered(const C3DVector* from, const C3DVector* to, C3DVector* hitPos, C3DVector* hitNormal,
        float* distance, CThing** hitThing, long test, bool flag);   // 0x00877C40
};
struct CCameraManager {
    unsigned char _pad[0x78];
    CTools Tools;                               // +0x78
};
class CGameCameraManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool GetCameraManager(CCameraManager** out);   // slot 10 (+0x28)
};
class CWorld { public:
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;         // +0x34
    unsigned long* GetPRandomSeed();            // 0x0049D810
    static long __fastcall GetFrame();                // 0x0049D870
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern long g_FramesPerSecond;                  // 0x01375550
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // vptr 0x01238C6C
    virtual void v00();
    explicit __forceinline CIntelligentPointerCreature(CThing* thing) { SetPItem(thing); }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* thing);    // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
    CIntelligentPointerThing& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};
class CSummonerDef { public:
    unsigned char _pad[0x28];
    float LightningRange;                       // +0x28
    float LightningChance;                      // +0x2c
    float LightningLOSChance;                   // +0x30
    float PreemptiveDelaySeconds;               // +0x34
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
};
struct CAIStateGroup_SummonerLightningAttack : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    const CSummonerDef* PDef;                   // +0x1c CDefPointer<const CSummonerDef>
    long NextFrameForPreemptiveUse;             // +0x20
    CThing* GetCreature() const { return PBrain->PCreature; }
    __forceinline bool IsTargetable(CTCLightningTarget* target)
    {
        CTCLightningTarget::SMatch out;
        return target->CanBeTargeted(&out, GetCreature());
    }
    virtual bool IsRunnable();
};

bool CAIStateGroup_SummonerLightningAttack::IsRunnable()
{
    float range = PDef->LightningRange;
    CTCEnemySource* source;
    if (GetCreature()->GetInterfaceX(&source, 8)) {
        CThing* enemy = source->GetEnemy();
        if (enemy && !(enemy->m_flags91 & 1)) {
            CCameraManager* camera;
            CWorld* world = GetPWorld();
            if (world->PCameraManager->GetCameraManager(&camera)) {
                const C3DVector& e = enemy->GetPosition();
                C3DVector to(e.x, e.y, e.z + 2.0f);
                const C3DVector& c = PBrain->PCreature->GetPosition();
                C3DVector from(c.x, c.y, c.z + 2.0f);
                C3DVector hitPos, hitNormal;
                float distance;
                CThing* hitThing;
                if (!camera->Tools.IsLineOfSightFiltered(&from, &to, &hitPos, &hitNormal, &distance, &hitThing, 3, false))
                    return false;
            }
            {
                const C3DVector& from = GetCreature()->GetPosition();
                const C3DVector& to = enemy->GetPosition();
                float distanceSquared = C2DVector(to.x - from.x, to.y - from.y).GetLengthSquared();
                if (distanceSquared > range * range) {
                    PTarget = CIntelligentPointerCreature(enemy);
                    return true;
                }
                if (CWorld::GetFrame() > NextFrameForPreemptiveUse) {
                    unsigned long* seed = GetPWorld()->GetPRandomSeed();
                    unsigned long next = *seed * 0x24A1U + 0x24DFU;
                    *seed = next;
                    unsigned long rotated = GFROR13(next);
                    *seed = rotated;
                    float percent = (long)(rotated % 1000) * 0.1f;
                    float losChance = PDef->LightningLOSChance;
                    if (percent < PDef->LightningChance) {
                        PTarget = enemy;
                        return true;
                    }
                    if (percent < losChance) {
                        CTCLightningTarget* target;
                        if (enemy->GetInterfaceC(&target, 0x42)) {
                            if (IsTargetable(target)) {
                                PTarget = enemy;
                                return true;
                            }
                        }
                    }
                    long fps = g_FramesPerSecond;
                    const CSummonerDef* def = PDef;
                    NextFrameForPreemptiveUse = (long)((float)CWorld::GetFrame() + (float)fps * def->PreemptiveDelaySeconds);
                }
            }
        }
    }
    return false;
}

// ---- stubs and harness ----
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
void CTCEnemySource::v00() {}
void CTCEnemySource::v01() {}
void CTCEnemySource::v02() {}
void CTCEnemySource::v03() {}
void CTCEnemySource::v04() {}
void CTCEnemySource::v05() {}
void CTCEnemySource::v06() {}
void CTCEnemySource::v07() {}
void CTCEnemySource::v08() {}
void CTCEnemySource::v09() {}
void CTCEnemySource::v10() {}
void CTCEnemySource::v11() {}
void CTCEnemySource::v12() {}
void CTCEnemySource::v13() {}
void CTCEnemySource::v14() {}
void CTCEnemySource::v15() {}
void CTCEnemySource::v16() {}
void CTCEnemySource::v17() {}
void CTCEnemySource::v18() {}
void CTCEnemySource::v19() {}
void CTCEnemySource::v20() {}
void CTCEnemySource::v21() {}
void CTCEnemySource::v22() {}
void CTCEnemySource::v23() {}
void CTCEnemySource::v24() {}
void CTCEnemySource::v25() {}
void CTCEnemySource::v26() {}
void CTCEnemySource::v27() {}
void CTCEnemySource::v28() {}
void CGameCameraManager::v00() {}
void CGameCameraManager::v01() {}
void CGameCameraManager::v02() {}
void CGameCameraManager::v03() {}
void CGameCameraManager::v04() {}
void CGameCameraManager::v05() {}
void CGameCameraManager::v06() {}
void CGameCameraManager::v07() {}
void CGameCameraManager::v08() {}
void CGameCameraManager::v09() {}

static CThing* g_enemy = 0; static bool g_haveCamera = true; static bool g_los = true; static int g_losCalls = 0;
static C3DVector g_from, g_to; static CCameraManager g_cameraManager; static unsigned long g_seed = 0;
static long g_frame = 0; static bool g_targetable = false; static int g_targetCalls = 0; static CThing* g_attacker = 0;
static int g_ipCtor = 0, g_ipDtor = 0, g_ipThingCtor = 0; static CWorld g_world;
long g_FramesPerSecond = 30;
CThing* CTCEnemySource::GetEnemy() { return 0; }
bool CGameCameraManager::GetCameraManager(CCameraManager**) { return false; }
struct TestSource : CTCEnemySource { CThing* GetEnemy() { return g_enemy; } };
struct TestCamera : CGameCameraManager { bool GetCameraManager(CCameraManager** out) { if (g_haveCamera) *out = &g_cameraManager; return g_haveCamera; } };
static TestSource g_source; static TestCamera g_camera; static CTCLightningTarget g_target;
struct Entry { long type; void* component; };
template<class T> static T* bsearchEntries(T* first, T* last, const long* type) { while (first != last && first->type < *type) ++first; return first; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return bsearchEntries(first, last, type); }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { return bsearchEntries(first, last, type); }
static C3DVector g_creaturePos(1.0f, 2.0f, 3.0f), g_enemyPos(4.0f, 6.0f, 5.0f);
static CThing* g_creature; static CThing* g_enemyThing;
const C3DVector& CThing::GetPosition() const { return this == g_creature ? g_creaturePos : g_enemyPos; }
bool CTCLightningTarget::CanBeTargeted(SMatch* out, CThing* const& attacker) { ++g_targetCalls; g_attacker = attacker; CHECK(this == &g_target); out->Object = 0; out->Block = 0; return g_targetable; }
bool CTools::IsLineOfSightFiltered(const C3DVector* from, const C3DVector* to, C3DVector* hitPos, C3DVector* hitNormal, float* distance, CThing** hitThing, long test, bool flag)
{ ++g_losCalls; g_from = *from; g_to = *to; CHECK(this == &g_cameraManager.Tools && test == 3 && !flag); *distance = 1.0f; *hitThing = 0; hitPos->x = hitNormal->x = 0.0f; return g_los; }
unsigned long* CWorld::GetPRandomSeed() { CHECK(this == &g_world); return &g_seed; }
long CWorld::GetFrame() { return g_frame; }
unsigned long __fastcall GFROR13(unsigned long value) { return (value >> 13) | (value << 19); }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerCreature::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; ++g_ipCtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; m_pItem = 0; }
CIntelligentPointerThing::CIntelligentPointerThing(CThing* thing) { m_pItem = thing; ++g_ipThingCtor; }
CIntelligentPointerThing& CIntelligentPointerThing::operator=(const CIntelligentPointerThing& other) { m_pItem = other.m_pItem; return *this; }
CAIStateGroupBase::~CAIStateGroupBase() {}
CWorld* CAIStateGroupBase::GetPWorld() { return &g_world; }
static CThing* makeThing(long type, void* component, unsigned char flags)
{
    unsigned char* raw = new unsigned char[sizeof(CThing)]; memset(raw, 0, sizeof(CThing));
    CThing* t = (CThing*)raw;
    CInterfaceMap::SEntry* e = new CInterfaceMap::SEntry[1]; e[0].type = type; e[0].component = (CTCBase*)component;
    t->m_interfaces.first = e; t->m_interfaces.last = e + 1;
    t->m_presentBits[type >> 5] |= 1UL << (type & 31);
    t->m_flags91 = flags;
    return t;
}
int main()
{
    CSummonerDef def; memset(&def, 0, sizeof(def)); def.LightningRange = 10.0f; def.LightningChance = 20.0f; def.LightningLOSChance = 60.0f; def.PreemptiveDelaySeconds = 2.5f;
    g_world.PCameraManager = &g_camera;
    CAIBrain brain; memset(&brain, 0, sizeof(brain));
    g_creature = makeThing(8, &g_source, 0); brain.PCreature = g_creature;
    g_enemyThing = makeThing(0x42, &g_target, 0);
    unsigned char rawGroup[sizeof(CAIStateGroup_SummonerLightningAttack)]; memset(rawGroup, 0, sizeof(rawGroup));
    CAIStateGroup_SummonerLightningAttack& g = *(CAIStateGroup_SummonerLightningAttack*)rawGroup;
    g.PBrain = &brain; g.PDef = &def; g.NextFrameForPreemptiveUse = 100; g.PTarget.m_pItem = 0;
    // creature without the enemy-source interface
    CThing* plain = makeThing(9, 0, 0); brain.PCreature = plain; g_creature = plain;
    CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g_losCalls == 0);
    brain.PCreature = g_creature = makeThing(8, &g_source, 0);
    // no enemy, dead enemy
    g_enemy = 0; CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable());
    g_enemy = makeThing(0x42, &g_target, 1); CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g_losCalls == 0);
    // live enemy, no line of sight
    g_enemy = g_enemyThing; g_los = false; CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable()); CHECK(g_losCalls == 1);
    CHECK(g_from.x == 1.0f && g_from.y == 2.0f && g_from.z == 5.0f && g_to.x == 4.0f && g_to.y == 6.0f && g_to.z == 7.0f);
    // out of range (dist 5 > range 10? no: make range small)
    g_los = true; def.LightningRange = 4.0f; g_ipCtor = g_ipDtor = 0;
    CHECK(g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g.PTarget.m_pItem == g_enemy && g_ipCtor == 1 && g_ipDtor == 1);
    // in range, preemptive frame not reached
    def.LightningRange = 10.0f; g.PTarget.m_pItem = 0; g_frame = 100; CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g.PTarget.m_pItem == 0);
    // no camera manager: LOS skipped, still gated by the frame
    g_haveCamera = false; g_losCalls = 0; CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g_losCalls == 0); g_haveCamera = true;
    // frame reached: roll below LightningChance -> latch through CIntelligentPointerThing
    g_frame = 101;
    // find seeds whose roll lands below LightningChance, between the two chances, and above the LOS chance (same arithmetic as the body)
    unsigned long lowSeed = 0, midSeed = 0, highSeed = 0;
    for (unsigned long s = 1; s < 100000 && (!lowSeed || !midSeed || !highSeed); ++s) {
        unsigned long rot = GFROR13(s * 0x24A1U + 0x24DFU); float pct = (long)(rot % 1000) * 0.1f;
        if (!lowSeed && pct < 20.0f) lowSeed = s;
        if (!midSeed && pct >= 20.0f && pct < 60.0f) midSeed = s;
        if (!highSeed && pct >= 60.0f) highSeed = s;
    }
    CHECK(lowSeed && midSeed && highSeed);
    g_seed = lowSeed; g_ipThingCtor = 0; g_ipDtor = 0;
    CHECK(g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g.PTarget.m_pItem == g_enemy && g_ipThingCtor == 1 && g_ipDtor == 1);
    CHECK(g_seed == GFROR13(lowSeed * 0x24A1U + 0x24DFU));
    // mid roll, not targetable: schedule the next check, no latch
    g.PTarget.m_pItem = 0; g_seed = midSeed; g_targetable = false; g_targetCalls = 0; g_frame = 101;
    CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g_targetCalls == 1 && g_attacker == g_creature && g.PTarget.m_pItem == 0);
    CHECK(g.NextFrameForPreemptiveUse == (long)(101.0f + 30.0f * 2.5f));
    // mid roll, targetable: latch
    g_seed = midSeed; g_targetable = true; g.NextFrameForPreemptiveUse = 100; g_ipThingCtor = 0;
    CHECK(g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g.PTarget.m_pItem == g_enemy && g_ipThingCtor == 1);
    // mid roll, enemy without the lightning-target interface: schedule
    g_enemy = makeThing(0x41, 0, 0); g.PTarget.m_pItem = 0; g_seed = midSeed; g_targetCalls = 0; g.NextFrameForPreemptiveUse = 100;
    CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g_targetCalls == 0 && g.NextFrameForPreemptiveUse == 176);
    // high roll: schedule only
    g_enemy = g_enemyThing; g_seed = highSeed; g_targetCalls = 0; g.NextFrameForPreemptiveUse = 50; g_frame = 200; g_FramesPerSecond = 10;
    CHECK(!g.CAIStateGroup_SummonerLightningAttack::IsRunnable() && g_targetCalls == 0 && g.NextFrameForPreemptiveUse == 225 && g.PTarget.m_pItem == 0);
    printf("0096f770_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail;
}