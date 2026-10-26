// Behaviour test for 007e7760: src/007e7760.cpp copied verbatim; physics, scripted control, script interface and the
// move action stubbed so the unchanged-position early-out, the instant move (physics SetPosition to the marker) and the
// scripted move (StartScriptingEntity when a runner exists, a move-to action, Moving set) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCFireheartMinigame::SetHeroPos(long x, long y, bool instantly) -- move the hero to grid marker [x][y]: teleport through the physics component, or (after putting the hero under scripted control) queue a CActionMoveTo to the marker
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: SetHeroPos
//
// Retail 0x007E7760, 288 bytes incl. padding (`ret 0Ch`). Ego_r.pdb `?SetHeroPos@CTCFireheartMinigame@@...`.
// Callees: CThing::GetPosition @ 0x004C73D0 (twice), the physics component's SetPosition(const C3DVector&) (slot 30, +0x78),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCScriptedControl, type 0x1F =
// bit 31 of the bitset word +0x20, `test ecx,ecx ; jns`; unchecked: the miss path uses the never-written slot),
// CTCBase::GetWorld @ 0x00686A70, CThing::GetScriptThing @ 0x004C7E50 (by value), CGameScriptInterface::StartScriptingEntity
// (slot 8, +0x20), ~CScriptThing @ 0x004AA840, operator new @ 0x00BFEA1A, CActionMoveTo::CActionMoveTo(const C3DVector&,
// EMovementType, float, bool, bool) @ 0x007E71D0, CTCScriptedControl::AddAction(CActionBase*) @ 0x007137D0.
// Global: the script runner @ 0x013D2838 (only tested for null).
// Layout = Ego_r.pdb CTCFireheartMinigame: Markers[4][4] +0x0C, XPos +0x68, YPos +0x6C, Moving +0x70, ScriptData +0x78
// (CScriptGameResourceObjectScriptedThingBase); CWorld PGameScriptInterface +0x38 (scoped_ptr); retail CThing PhysicsTC +0x60.
// Shape: the physics component is a named local whose vtable VC parks in the dead `instantly` slot across GetPosition; the
// interface key and the out pointer also live in that slot; the CGameScriptInterface pointer is a named local (fetched with
// its vtable before the pushes), the CScriptThing an unnamed temporary; `new` keeps its null test.
class CThing;
class CTCBase;
class C3DVector;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CTCScriptedControl;
class CScriptThing { public:
    unsigned char _body[12];
    ~CScriptThing();                            // 0x004AA840
};

class CTCPhysicsBase {
public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29)
#undef V
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CScriptThing GetScriptThing();              // 0x004C7E50
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    __forceinline CTCScriptedControl* GetScriptedControl()
    {
        CTCScriptedControl* control;
        GetInterface(&control, 0x1f);
        return control;
    }
};

class CScriptGameResourceObjectScriptedThingBase { public:
    unsigned char _body[0x10];
};

class CGameScriptInterface {
public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07)
#undef V
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& data,
                                      long priority);   // slot 8 (+0x20)
};

class CWorld { public:
    unsigned char _pad00[0x38];
    CGameScriptInterface* PGameScriptInterface; // +0x38 (scoped_ptr<CGameScriptInterface>: its one pointer)
};

struct CActionBase;
struct CActionMoveTo {                          // 0x50 bytes
    unsigned char _body[0x50];
    CActionMoveTo(const C3DVector& position, long movementType, float distance, bool a, bool b);   // 0x007E71D0
};
class CTCScriptedControl { public:
    void AddAction(CActionBase* action);        // 0x007137D0
};

struct CScriptRunner;
extern CScriptRunner* g_pScriptRunner;          // 0x013D2838

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    CThing* GetParentThing() const { return ParentThing; }
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CWorld* GetWorld();                         // 0x00686A70
};

class CTCFireheartMinigame : public CTCBase {
public:
    CThing* Markers[4][4];                      // +0x0C
    bool InSession;                             // +0x4C
    bool ForceQuit;                             // +0x4D
    CThing* CurrentMarker;                      // +0x50
    bool Initialised;                           // +0x54
    bool BlocksActive[4][4];                    // +0x55
    long XPos;                                  // +0x68
    long YPos;                                  // +0x6C
    bool Moving;                                // +0x70
    CTCBase* PTCOldPhysics;                     // +0x74
    CScriptGameResourceObjectScriptedThingBase ScriptData;   // +0x78
    bool QuitWithDelay;                         // +0x88
    void SetHeroPos(long x, long y, bool instantly);
};

void CTCFireheartMinigame::SetHeroPos(long x, long y, bool instantly)
{
    if (XPos == x && YPos == y)
        return;
    XPos = x;
    YPos = y;
    if (instantly) {
        CTCPhysicsBase* physics = GetParentThing()->PhysicsTC;
        physics->SetPosition(Markers[x][y]->GetPosition());
        return;
    }
    CTCScriptedControl* control = GetParentThing()->GetScriptedControl();
    if (g_pScriptRunner) {
        GetWorld()->PGameScriptInterface->StartScriptingEntity(GetParentThing()->GetScriptThing(), ScriptData, 4);
    }
    control->AddAction((CActionBase*)new CActionMoveTo(Markers[x][y]->GetPosition(), 3, 0.0f, true, false));
    Moving = true;
}
// ---- end source ----
void CTCPhysicsBase::v00() {} void CTCPhysicsBase::v01() {} void CTCPhysicsBase::v02() {} void CTCPhysicsBase::v03() {} void CTCPhysicsBase::v04() {} void CTCPhysicsBase::v05() {} void CTCPhysicsBase::v06() {} void CTCPhysicsBase::v07() {} void CTCPhysicsBase::v08() {} void CTCPhysicsBase::v09() {} void CTCPhysicsBase::v10() {} void CTCPhysicsBase::v11() {} void CTCPhysicsBase::v12() {} void CTCPhysicsBase::v13() {} void CTCPhysicsBase::v14() {} void CTCPhysicsBase::v15() {} void CTCPhysicsBase::v16() {} void CTCPhysicsBase::v17() {} void CTCPhysicsBase::v18() {} void CTCPhysicsBase::v19() {} void CTCPhysicsBase::v20() {} void CTCPhysicsBase::v21() {} void CTCPhysicsBase::v22() {} void CTCPhysicsBase::v23() {} void CTCPhysicsBase::v24() {} void CTCPhysicsBase::v25() {} void CTCPhysicsBase::v26() {} void CTCPhysicsBase::v27() {} void CTCPhysicsBase::v28() {} void CTCPhysicsBase::v29() {}
void CGameScriptInterface::v00() {} void CGameScriptInterface::v01() {} void CGameScriptInterface::v02() {} void CGameScriptInterface::v03() {} void CGameScriptInterface::v04() {} void CGameScriptInterface::v05() {} void CGameScriptInterface::v06() {} void CGameScriptInterface::v07() {}
static const C3DVector* g_setPos = 0; static int g_sets = 0;
void CTCPhysicsBase::SetPosition(const C3DVector& p) { ++g_sets; g_setPos = &p; }
static int g_markerPos[16][3];
const C3DVector& CThing::GetPosition() const { return *(const C3DVector*)&g_markerPos[((const unsigned char*)this)[0]]; }
static int g_scriptThings = 0, g_scriptThingDtors = 0;
CScriptThing CThing::GetScriptThing() { ++g_scriptThings; CScriptThing s; memset(&s, 0, sizeof(s)); return s; }
CScriptThing::~CScriptThing() { ++g_scriptThingDtors; }
static int g_starts = 0; static long g_priority = 0;
bool CGameScriptInterface::StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, long priority) { ++g_starts; g_priority = priority; return true; }
static CGameScriptInterface g_gsi; static CWorld g_world;
CWorld* CTCBase::GetWorld() { return &g_world; }
static const C3DVector* g_moveTo = 0; static long g_moveType = 0;
CActionMoveTo::CActionMoveTo(const C3DVector& position, long movementType, float, bool, bool) { g_moveTo = &position; g_moveType = movementType; }
static int g_actions = 0; static CActionBase* g_lastAction = 0;
void CTCScriptedControl::AddAction(CActionBase* a) { ++g_actions; g_lastAction = a; }
CScriptRunner* g_pScriptRunner = 0;
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 0x1f ? first : last; }
int main()
{
    g_world.PGameScriptInterface = &g_gsi;
    static CTCPhysicsBase physics; static CTCScriptedControl control;
    g_entries[0].type = 0x1f; g_entries[0].component = (CTCBase*)&control;
    static CThing hero; memset(&hero, 0, sizeof(hero));
    hero.m_interfaces.first = g_entries; hero.m_interfaces.last = g_entries + 1; hero.PhysicsTC = &physics;
    hero.m_presentBits[0x1f >> 5] |= 1UL << (0x1f & 31);
    static unsigned char markersRaw[16][0x80];
    static unsigned char raw[sizeof(CTCFireheartMinigame)]; memset(raw, 0, sizeof(raw));
    CTCFireheartMinigame* m = (CTCFireheartMinigame*)raw; m->ParentThing = &hero;
    for (int i = 0; i < 16; ++i) { markersRaw[i][0] = (unsigned char)i; m->Markers[i / 4][i % 4] = (CThing*)markersRaw[i]; }
    m->SetHeroPos(0, 0, true); CHECK(g_sets == 0 && g_actions == 0);          // already there
    m->SetHeroPos(1, 2, true);
    CHECK(m->XPos == 1 && m->YPos == 2 && g_sets == 1 && g_setPos == (const C3DVector*)&g_markerPos[6] && g_actions == 0);
    m->SetHeroPos(2, 3, false);
    CHECK(g_starts == 0 && g_actions == 1 && g_moveTo == (const C3DVector*)&g_markerPos[11] && g_moveType == 3 && m->Moving);
    g_pScriptRunner = (CScriptRunner*)1; m->Moving = false;
    m->SetHeroPos(3, 0, false);
    CHECK(g_starts == 1 && g_priority == 4 && g_scriptThings == 1 && g_scriptThingDtors >= 1 && g_actions == 2 && m->Moving);
    if (fails == 0) printf("007e7760_TEST PASS\n"); else printf("007e7760_TEST FAIL\n");
    return fails;
}