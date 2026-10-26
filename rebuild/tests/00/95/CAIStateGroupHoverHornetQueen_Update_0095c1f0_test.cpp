// Behaviour test for CAIStateGroup_HoverHornetQueen::Update (0x0095C1F0): every boss phase is driven through scripted
// stubs (IsStillRunnable answers come from a script string) and the exact call trace is compared: terminate when not
// runnable, 0 spawn point -> phase 1, 1/4 spawn -> next phase, 2/5 deregister / wait / re-register on the target,
// 3/6 attack loop with knock-down idles then increment, 7 attack when far from the arena centre, dive-bomb when near.
#include <stdio.h>
#include <string.h>

// ---- reconstruction under test (verbatim copy of src/0095c1f0.cpp) ----
// Leaf: Update
// Grade: RELOCATION_MATCH 572/572 on the corrected span 0x0095C1F0+572 (code + embedded jump table; bounds audit) -- catalog row 608 B also swallowed 0x0095C430/0x0095C440
// CAIStateGroup_HoverHornetQueen::Update() -- run the hornet queen boss phase: 0 go to the spawn point, 1/4 spawn babies, 2/5 leave combat while the babies fight then re-engage the target, 3/6 attack until the boss is ready for the phase change, 7 dive-bomb near the arena centre or attack until done; terminate when no longer runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095C1F0, length 540 bytes + an 8-entry jump table at 0x0095C40C.
// Callees: IsStillRunnable @ 0x0095A420, Terminate @ 0x008FCF40, CVectorMap::LowerBound @ 0x0040F020, CTCBoss::GetBossPhase
// @ 0x007AED10 / SetNextBossPhase @ 0x007AEDA0 / IsReadyForPhaseChange @ 0x007AEE90 / IncrementBossPhase @ 0x007AED20,
// GoToSpawnPoint @ 0x0095B020, Spawn @ 0x0095B820, CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CCombatManager::
// DeregisterCombatant @ 0x006ED9C0 / RegisterCombatant @ 0x006EDC50, WaitWhileBabiesFight @ 0x0095C0B0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, Attack @ 0x0095BCF0, IsKnockedDown @ 0x0085F010 (`__fastcall`), PlayIdle
// @ 0x0095B680, CThing::GetPosition @ 0x004C73D0, DiveBomb @ 0x0095B9E0.
// Layout: Ego_r.pdb CAIStateGroup_HoverHornetQueen PTarget +0x14, ArenaCentrePos +0x28; CWorld +0x4c combat manager.
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetXYMagnitude() const { return (float)sqrt(X * X + Y * Y); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
extern float g_HornetDiveBombRange;             // 0x0138D2D8 (15.0f)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
class CThingCreatureBase : public CThing { public: };
bool __fastcall IsKnockedDown(CThing* thing);   // 0x0085F010
class CTCBoss { public:                                // interface 0xad
    long GetBossPhase();                        // 0x007AED10
    void IncrementBossPhase();                  // 0x007AED20
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
    bool IsReadyForPhaseChange();               // 0x007AEE90
};
class CCombatManager { public:
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
};
class CWorld { public:
    unsigned char _pad0[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool IsStillRunnable();                     // 0x0095A420
    bool GoToSpawnPoint();                      // 0x0095B020
    void PlayIdle();                            // 0x0095B680
    void Spawn();                               // 0x0095B820
    void DiveBomb();                            // 0x0095B9E0
    void Attack();                              // 0x0095BCF0
    long WaitWhileBabiesFight();                // 0x0095C0B0
    void Update();
};

void CAIStateGroup_HoverHornetQueen::Update()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
    switch (boss->GetBossPhase()) {
    case 0:
        GoToSpawnPoint();
        boss->SetNextBossPhase(1, false);
        break;
    case 1:
    case 4:
        Spawn();
        if (IsStillRunnable())
            boss->SetNextBossPhase(boss->GetBossPhase() + 1, false);
        break;
    case 2:
    case 5:
        {
            CThingCreatureBase* creature = GetCreature();
            CWorld* world = GetPWorld();
            world->PCombatManager->DeregisterCombatant(creature);
        }
        if (WaitWhileBabiesFight() == 1 || !IsStillRunnable()) {
            Terminate();
            return;
        }
        boss->IsReadyForPhaseChange();
        {
            CThingCreatureBase* creature = GetCreature();
            CCombatManager* combat = GetPWorld()->PCombatManager;
            combat->RegisterCombatant(creature, PTarget.GetPItem(), true);
        }
        boss->IncrementBossPhase();
        break;
    case 3:
    case 6:
        if (IsStillRunnable()) {
            do {
                if (boss->IsReadyForPhaseChange())
                    break;
                Attack();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (IsKnockedDown(PTarget.GetPItem()))
                    PlayIdle();
            } while (IsStillRunnable());
        }
        if (IsStillRunnable())
            boss->IncrementBossPhase();
        break;
    case 7:
        do {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if ((PTarget.GetPItem()->GetPosition() - ArenaCentrePos).GetXYMagnitude() > g_HornetDiveBombRange)
                Attack();
            else
                DiveBomb();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if (IsKnockedDown(PTarget.GetPItem()))
                PlayIdle();
        } while (IsStillRunnable());
        break;
    }
}

// ---- harness ----
float g_HornetDiveBombRange = 15.0f;
static char g_trace[512];
static const char* g_run;           // IsStillRunnable script, '1' / '0'; exhausted -> false
static long g_phase;
static bool g_ready, g_knocked;
static long g_wait;
static CTCBoss* g_boss;
static CThingCreatureBase* g_creature;
static CThing* g_target;
static CCombatManager* g_combat;
static CWorld* g_world;
static C3DVector g_targetPos;
static void T(const char* s) { strcat(g_trace, s); strcat(g_trace, " "); }

CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
const C3DVector& CThing::GetPosition() const { return g_targetPos; }
bool __fastcall IsKnockedDown(CThing* thing) { T(thing == g_target ? "K" : "K?"); return g_knocked; }
long CTCBoss::GetBossPhase() { if (this != g_boss) T("BOSS?"); return g_phase; }
void CTCBoss::IncrementBossPhase() { T("I"); ++g_phase; }
void CTCBoss::SetNextBossPhase(long phase, bool immediate) { char b[16]; sprintf(b, "N%ld%s", phase, immediate ? "!" : ""); T(b); }
bool CTCBoss::IsReadyForPhaseChange() { T("Q"); return g_ready; }
void CCombatManager::DeregisterCombatant(CThingCreatureBase* creature) { T(this == g_combat && creature == g_creature ? "D" : "D?"); }
void CCombatManager::RegisterCombatant(CThing* creature, CThing* target, bool flag)
{
    T(this == g_combat && creature == g_creature && target == g_target && flag ? "C" : "C?");
}
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CAIStateGroupBase::~CAIStateGroupBase() {}
CWorld* CAIStateGroupBase::GetPWorld() { return g_world; }
void CAIStateGroupBase::Terminate() { T("T"); }
bool CAIStateGroup_HoverHornetQueen::IsStillRunnable() { T("R"); return *g_run ? *g_run++ == '1' : false; }
bool CAIStateGroup_HoverHornetQueen::GoToSpawnPoint() { T("G"); return true; }
void CAIStateGroup_HoverHornetQueen::PlayIdle() { T("P"); }
void CAIStateGroup_HoverHornetQueen::Spawn() { T("S"); }
void CAIStateGroup_HoverHornetQueen::DiveBomb() { T("B"); }
void CAIStateGroup_HoverHornetQueen::Attack() { T("A"); }
long CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight() { T("W"); return g_wait; }

static int g_fail;
static void expect(CAIStateGroup_HoverHornetQueen& q, long phase, const char* run, const char* want)
{
    g_trace[0] = 0; g_run = run; g_phase = phase;
    q.Update();
    if (strcmp(g_trace, want) != 0) { printf("FAIL phase %ld run %s: got [%s] want [%s]\n", phase, run, g_trace, want); g_fail = 1; }
}

int main()
{
    static unsigned char bossStore[4], targetStore[0x100], creatureStore[0x100], combatStore[4];
    static CInterfaceMap::SEntry entries[2];
    CWorld world;
    CAIBrain brain;
    CAIStateGroup_HoverHornetQueen q;
    CThingCreatureBase* creature = (CThingCreatureBase*)creatureStore;
    g_boss = (CTCBoss*)bossStore; g_target = (CThing*)targetStore; g_creature = creature; g_combat = (CCombatManager*)combatStore;
    world.PCombatManager = g_combat; g_world = &world;
    memset(creatureStore, 0, sizeof(creatureStore));
    creature->m_presentBits[0xad >> 5] = 1UL << (0xad & 31);
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0xad; entries[1].component = (CTCBase*)g_boss;
    creature->m_interfaces.first = entries; creature->m_interfaces.last = entries + 2;
    brain.PCreature = creature;
    q.PBrain = &brain;
    q.PTarget.m_pItem = g_target;
    q.ArenaCentrePos = C3DVector(100.0f, 100.0f, 0.0f);
    g_ready = false; g_knocked = false; g_wait = 0;

    expect(q, 0, "0", "R T ");
    expect(q, 0, "1", "R G N1 ");
    expect(q, 1, "11", "R S R N2 ");
    expect(q, 4, "11", "R S R N5 ");
    expect(q, 4, "10", "R S R ");
    expect(q, 2, "11", "R D W R Q C I ");
    expect(q, 5, "10", "R D W R T ");
    g_wait = 1; expect(q, 2, "11", "R D W T "); g_wait = 0;
    g_knocked = true;
    expect(q, 3, "11101", "R R Q A R K P R R I ");
    expect(q, 6, "110", "R R Q A R T ");
    g_ready = true;
    expect(q, 6, "111", "R R Q R I ");
    g_ready = false; g_knocked = false;
    g_targetPos = C3DVector(130.0f, 100.0f, 50.0f);         // 30 units away in XY: attack
    expect(q, 7, "1110", "R R A R K R ");
    g_targetPos = C3DVector(110.0f, 105.0f, -40.0f);        // ~11 units away: dive-bomb
    g_knocked = true;
    expect(q, 7, "11110", "R R B R K P R R T ");
    expect(q, 7, "110", "R R B R T ");
    expect(q, 7, "10", "R R T ");
    if (g_fail) return 1;
    printf("0095c1f0_TEST PASS\n");
    return 0;
}