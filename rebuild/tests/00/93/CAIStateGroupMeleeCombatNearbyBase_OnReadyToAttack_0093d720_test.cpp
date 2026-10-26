// Behaviour test for 0093d720: src/0093d720.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAIStateGroup_MeleeCombatNearbyBase::OnReadyToAttack()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_MeleeCombatNearbyBase::OnReadyToAttack() -- in combat mode 3, when the current region forbids fighting, ask for money and terminate unless the group may continue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D720, length 128 bytes. Vtable slot 15 of
// CAIStateGroup_MeleeCombatNearbyBase (0x0128B954).
// Callees: the creature's virtual slot 48 (+0xc0, a mode query), CAIStateGroupBase::PeekWorldMap @ 0x008FCFF0 (x2),
// CWorldMap's current-region index getter @ 0x004FB150 (`[this+9Ch]`; manifest "CWorld::DrawGetEnvironment" is
// propagated), CWorldMap::GetRegion(long) @ 0x004FC180 (0x58-byte regions), CAIStateGroup_MeleeCombatNearbyBase::
// RequestMoney @ 0x0093D580, this group's virtual slot 5 (+0x14), CAIStateGroupBase::Terminate @ 0x008FCF40, the
// region def's virtual slot 1 (destroy, tail-jumped).
//
// Shape: `GetRegion(PeekWorldMap()->index())` with PeekWorldMap re-called for the object; the region's counted def
// pointer (+0x24) is copy-constructed (from a named region reference) into a CDefPointer local whose null test guards
// the whole body; the copy ctor add-refs through an inline AddRef() (`inc dword ptr [esi+4]`)
// and the dtor releases (`dec [esi+4] ; jne`, then the virtual destroy) at both exits. The def
// flag at +0x9d and the region/def classes are descriptive. No engine header models the vptr chain: local model.
class CRegionDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    unsigned char _pad[0x95];                   // +0x08..+0x9c
    unsigned char m_noFighting;                 // +0x9d (descriptive)
    void AddRef() { ++m_refCount; }
};

struct CRegionDefPointer {                       // CDefPointer<CRegionDef>
    CRegionDef* m_pDef;
    CRegionDefPointer(const CRegionDefPointer& o) : m_pDef(o.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CRegionDefPointer() { if (m_pDef && --m_pDef->m_refCount == 0) m_pDef->Destroy(); }
    CRegionDef* operator->() const { return m_pDef; }
};
class CRegion { public:                                // 0x58 bytes
    unsigned char _pad[0x24];
    CRegionDefPointer PDef;                     // +0x24
};

class CWorldMap { public:
    long GetCurrentRegionIndex();               // 0x004FB150
    CRegion& GetRegion(long index);             // 0x004FC180
};

class CThingCreatureBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47();
    virtual long GetCombatMode();               // slot 48 (+0xc0, descriptive)
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool CanContinue();                 // slot 5 (+0x14, descriptive)
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void OnReadyToAttack();             // slot 15 (+0x3c)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CWorldMap* PeekWorldMap();                  // 0x008FCFF0
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    void RequestMoney();                        // 0x0093D580
    virtual void OnReadyToAttack();
};

void CAIStateGroup_MeleeCombatNearbyBase::OnReadyToAttack()
{
    if (PBrain->PCreature->GetCombatMode() == 3) {
        CRegion& region = PeekWorldMap()->GetRegion(PeekWorldMap()->GetCurrentRegionIndex());
        CRegionDefPointer def = region.PDef;
        if (def.m_pDef) {
            if (def->m_noFighting) {
                RequestMoney();
                if (!CanContinue()) {
                    Terminate();
                    return;
                }
            }
        }
    }
}

// ---- stubs and driver ----
void CThingCreatureBase::v00() {}
void CThingCreatureBase::v01() {}
void CThingCreatureBase::v02() {}
void CThingCreatureBase::v03() {}
void CThingCreatureBase::v04() {}
void CThingCreatureBase::v05() {}
void CThingCreatureBase::v06() {}
void CThingCreatureBase::v07() {}
void CThingCreatureBase::v08() {}
void CThingCreatureBase::v09() {}
void CThingCreatureBase::v10() {}
void CThingCreatureBase::v11() {}
void CThingCreatureBase::v12() {}
void CThingCreatureBase::v13() {}
void CThingCreatureBase::v14() {}
void CThingCreatureBase::v15() {}
void CThingCreatureBase::v16() {}
void CThingCreatureBase::v17() {}
void CThingCreatureBase::v18() {}
void CThingCreatureBase::v19() {}
void CThingCreatureBase::v20() {}
void CThingCreatureBase::v21() {}
void CThingCreatureBase::v22() {}
void CThingCreatureBase::v23() {}
void CThingCreatureBase::v24() {}
void CThingCreatureBase::v25() {}
void CThingCreatureBase::v26() {}
void CThingCreatureBase::v27() {}
void CThingCreatureBase::v28() {}
void CThingCreatureBase::v29() {}
void CThingCreatureBase::v30() {}
void CThingCreatureBase::v31() {}
void CThingCreatureBase::v32() {}
void CThingCreatureBase::v33() {}
void CThingCreatureBase::v34() {}
void CThingCreatureBase::v35() {}
void CThingCreatureBase::v36() {}
void CThingCreatureBase::v37() {}
void CThingCreatureBase::v38() {}
void CThingCreatureBase::v39() {}
void CThingCreatureBase::v40() {}
void CThingCreatureBase::v41() {}
void CThingCreatureBase::v42() {}
void CThingCreatureBase::v43() {}
void CThingCreatureBase::v44() {}
void CThingCreatureBase::v45() {}
void CThingCreatureBase::v46() {}
void CThingCreatureBase::v47() {}
void CAIStateGroupBase::v01() {}
void CAIStateGroupBase::v02() {}
void CAIStateGroupBase::v03() {}
void CAIStateGroupBase::v04() {}
void CAIStateGroupBase::v06() {}
void CAIStateGroupBase::v07() {}
void CAIStateGroupBase::v08() {}
void CAIStateGroupBase::v09() {}
void CAIStateGroupBase::v10() {}
void CAIStateGroupBase::v11() {}
void CAIStateGroupBase::v12() {}
void CAIStateGroupBase::v13() {}
void CAIStateGroupBase::v14() {}
static int g_destroyed = 0;
struct CDef : CRegionDef { void Destroy() { ++g_destroyed; } };
void CRegionDef::Destroy() {}
void CRegionDef::v00() {}
static unsigned char g_rs[3][sizeof(CRegion)]; static CRegion* const g_regions = (CRegion*)g_rs; static long g_cur = 1; static CWorldMap g_map;
long CWorldMap::GetCurrentRegionIndex() { return g_cur; }
CRegion& CWorldMap::GetRegion(long index) { return g_regions[index]; }
CWorldMap* CAIStateGroupBase::PeekWorldMap() { return &g_map; }
static int g_terminated = 0, g_requests = 0; static bool g_canContinue = true; static long g_mode = 3; static long g_countDuring;
void CAIStateGroupBase::Terminate() { ++g_terminated; }
struct CCreature : CThingCreatureBase { long GetCombatMode() { return g_mode; } };
long CThingCreatureBase::GetCombatMode() { return 0; }
struct CGroup : CAIStateGroup_MeleeCombatNearbyBase { bool CanContinue() { return g_canContinue; } };
bool CAIStateGroupBase::CanContinue() { return true; }
CAIStateGroupBase::~CAIStateGroupBase() {}
void CAIStateGroupBase::OnReadyToAttack() {}
static CDef* g_def;
void CAIStateGroup_MeleeCombatNearbyBase::RequestMoney() { ++g_requests; g_countDuring = g_def->m_refCount; }
int main()
{
    static CDef def; def.m_refCount = 1; def.m_noFighting = 1; g_def = &def;
    g_regions[1].PDef.m_pDef = &def;
    static CCreature creature; static CAIBrain brain; brain.PCreature = &creature;
    static CGroup g; g.PBrain = &brain;
    g.OnReadyToAttack();
    CHECK(g_requests == 1); CHECK(g_countDuring == 2); CHECK(def.m_refCount == 1); CHECK(g_terminated == 0); CHECK(g_destroyed == 0);
    g_canContinue = false; g.OnReadyToAttack();
    CHECK(g_requests == 2); CHECK(g_terminated == 1); CHECK(def.m_refCount == 1);
    def.m_noFighting = 0; g.OnReadyToAttack(); CHECK(g_requests == 2); CHECK(def.m_refCount == 1);
    def.m_refCount = 0; g.OnReadyToAttack(); CHECK(g_destroyed == 1);       // the last reference released destroys it
    g_mode = 2; def.m_refCount = 1; g.OnReadyToAttack(); CHECK(def.m_refCount == 1); CHECK(g_requests == 2);
    g_mode = 3; g_cur = 2; g.OnReadyToAttack(); CHECK(g_requests == 2);          // region without a def
    if (fails == 0) printf("0093d720_TEST PASS\n"); else printf("0093d720_TEST FAIL\n");
    return fails;
}