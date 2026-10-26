// Behaviour test for CAIStateGroup_SummonerCharge::Update (0x0096FF10): model and body verbatim, every callee stubbed;
// exercises the dead-target exit, the stealth notification, the failed SetCurrentAction, every charge-loop outcome
// (action finished, action code changed, target in reach, target dies, DoNothing interrupt) and both charge-out-of ctors.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define _snprintf snprintf
#endif
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
class C3DVector { public: float x, y, z; };
class C2DVector { public:
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) {}
    float GetLengthSquared() const { return x * x + y * y; }
};
struct CThingPhysicsBody {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPhysicsBody* PBody;                   // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;
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
};
class CCreatureActionBase;
struct CCurrentAction {
    unsigned char _pad[0x61];
    unsigned char Finished;                     // +0x61
    long GetActionCode();
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad94[0x100 - 0x94];
    CCurrentAction* PCurrentAction;             // +0x100
    bool SetCurrentAction(CCreatureActionBase* action);
    bool IsPerformingAnyAction();
    void StopCurrentAction();
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const;
};
struct CIntelligentPointerThing : CBaseIntelligentPointer { };
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& pointer);
inline bool IsTargetAlive(const CIntelligentPointerThing& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(CThing* source, long status);
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00() {}
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);
    ~CCombatActionBase();
};
struct CCreatureAction_SummonerChargeInto : CCombatActionBase {
    virtual void v00() {}
    CCreatureAction_SummonerChargeInto(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBase(owner, thing, 150, CSoundPair()) { }
};
struct CCreatureAction_SummonerChargeOutOf : CCombatActionBase {
    CCreatureAction_SummonerChargeOutOf(CThingCreatureBase* owner, CThing* thing);
    CCreatureAction_SummonerChargeOutOf(CThingCreatureBase* owner);
};
class CWorld { public:
    static long GetWorldFrame();
};
long g_FramesPerSecond = 30;
class CSummonerDef { public:
    unsigned char _pad[0x44];
    float ChargeReach;                          // +0x44
    float ChargeDelay;                          // +0x48
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();
    void Terminate();
    long AIAction_DoNothing();
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);
};
struct CAIStateGroup_SummonerCharge : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    const CSummonerDef* PDef;                   // +0x1c CDefPointer<const CSummonerDef>
    long NextFrameToCharge;                     // +0x20
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    virtual void Update();
};

// ---- body verbatim (final.cpp) ----
void CAIStateGroup_SummonerCharge::Update()
{
    if (!IsTargetAlive(PTarget)) {
        Terminate();
        return;
    }
    CTCStealth* stealth;
    if (PTarget.GetPItem()->GetInterfaceX(&stealth, 0x36))
        stealth->SetAsInDanger(GetCreature(), 3);
    CThingCreatureBase* owner = GetCreature();
    if (!GetCreature()->SetCurrentAction(&CCreatureAction_SummonerChargeInto(owner, PTarget.GetPItem()))) {
        Terminate();
        return;
    }
    CWorld* world = GetPWorld();
    long fps = g_FramesPerSecond;
    const CSummonerDef* def = PDef;
    NextFrameToCharge = (long)((float)world->GetWorldFrame() + (float)fps * def->ChargeDelay);
    bool reached = false;
    bool done = false;
    long actionCode = PBrain->PCreature->PCurrentAction->GetActionCode();
    do {
        float reach = PBrain->PCreature->PBody->Radius + PDef->ChargeReach;
        CCurrentAction* action = PBrain->PCreature->PCurrentAction;
        if (IsTargetAlive(PTarget)) {
            if (action == 0 || action->Finished) {
                reached = true;
                done = true;
            } else if (action->GetActionCode() != actionCode) {
                done = true;
            } else {
                CThingCreatureBase* creature = GetCreature();
                CThing* target = PTarget.GetPItem();
                const C3DVector& from = creature->GetPosition();
                const C3DVector& to = target->GetPosition();
                float distanceSquared = C2DVector(to.x - from.x, to.y - from.y).GetLengthSquared();
                if (distanceSquared < reach * reach) {
                    reached = true;
                    done = true;
                }
            }
        } else {
            done = true;
        }
        if (AIAction_DoNothing() == 1) {
            Terminate();
            return;
        }
    } while (!done);
    if (reached) {
        if (PBrain->PCreature->IsPerformingAnyAction())
            PBrain->PCreature->StopCurrentAction();
        if (IS_THING_ALIVE(PTarget)) {
            CThingCreatureBase* creature = GetCreature();
            AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerChargeOutOf(creature, PTarget.GetPItem()), true);
        } else {
            AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerChargeOutOf(GetCreature()), true);
        }
    }
    Terminate();
}

// ---- stubs ----
static CThingCreatureBase g_creature; static CThing g_target; static CThingPhysicsBody g_body; static CCurrentAction g_action;
static CAIBrain g_brain; static CSummonerDef g_def; static CWorld g_world; static CTCStealth g_stealth;
static CInterfaceMap::SEntry g_entries[1];
static C3DVector g_creaturePos = { 0, 0, 0 }, g_targetPos = { 0, 0, 0 };
static int g_terminate, g_danger, g_setAction, g_nothing, g_perform, g_stop, g_isPerforming, g_outOfCtor2, g_outOfCtor1, g_intoCtor, g_combatDtor, g_lookups;
static long g_dangerStatus; static CThing* g_dangerSource; static bool g_setActionResult = true; static long g_nothingResults[16]; static int g_nothingCalls;
static long g_codes[16]; static int g_codeCalls; static int g_killTargetAtCode = -1; static bool g_performWait; static CCreatureActionBase* g_performed;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lookups++; return *type == 0x36 ? &g_entries[0] : last; }
const C3DVector& CThing::GetPosition() const { return this == &g_target ? g_targetPos : g_creaturePos; }
long CCurrentAction::GetActionCode() { long c = g_codes[g_codeCalls]; if (g_codeCalls == g_killTargetAtCode) g_target.m_flags91 = 1; g_codeCalls++; return c; }
bool CThingCreatureBase::SetCurrentAction(CCreatureActionBase* action) { g_setAction++; return g_setActionResult; }
bool CThingCreatureBase::IsPerformingAnyAction() { g_isPerforming++; return g_isPerforming == 1; }
void CThingCreatureBase::StopCurrentAction() { g_stop++; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& pointer) { return pointer.m_pItem != 0 && !(pointer.m_pItem->m_flags91 & 1); }
void CTCStealth::SetAsInDanger(CThing* source, long status) { g_danger++; g_dangerSource = source; g_dangerStatus = status; }
CCombatActionBase::CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound) { if (actionCode == 150 && owner == &g_creature && thing == &g_target && sound.m_first == 0) g_intoCtor++; }
CCombatActionBase::~CCombatActionBase() { g_combatDtor++; }
CCreatureAction_SummonerChargeOutOf::CCreatureAction_SummonerChargeOutOf(CThingCreatureBase* owner, CThing* thing) : CCombatActionBase(owner, thing, 0, CSoundPair()) { if (owner == &g_creature && thing == &g_target) g_outOfCtor2++; }
CCreatureAction_SummonerChargeOutOf::CCreatureAction_SummonerChargeOutOf(CThingCreatureBase* owner) : CCombatActionBase(owner, 0, 0, CSoundPair()) { if (owner == &g_creature) g_outOfCtor1++; }
long CWorld::GetWorldFrame() { return 1000; }
CWorld* CAIStateGroupBase::GetPWorld() { return &g_world; }
void CAIStateGroupBase::Terminate() { g_terminate++; }
long CAIStateGroupBase::AIAction_DoNothing() { g_nothing++; return g_nothingResults[g_nothingCalls++]; }
long CAIStateGroupBase::AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait) { g_perform++; g_performWait = wait; g_performed = action; return 2; }

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static void Reset(CAIStateGroup_SummonerCharge& g)
{
    g_terminate = g_danger = g_setAction = g_nothing = g_perform = g_stop = g_isPerforming = g_outOfCtor2 = g_outOfCtor1 = g_intoCtor = g_combatDtor = g_lookups = 0;
    g_nothingCalls = g_codeCalls = 0; g_killTargetAtCode = -1; g_setActionResult = true;
    for (int i = 0; i < 16; ++i) { g_nothingResults[i] = 0; g_codes[i] = 77; }
    g_target.m_flags91 = 0; g_target.m_presentBits[1] = 1UL << (0x36 & 31); g_action.Finished = 0;
    g.PTarget.m_pItem = &g_target; g.NextFrameToCharge = 0;
    g_creaturePos.x = g_creaturePos.y = 0; g_targetPos.x = 10.0f; g_targetPos.y = 0;
}

int main()
{
    memset(&g_creature, 0, sizeof(g_creature)); memset(&g_target, 0, sizeof(g_target));
    g_creature.PBody = &g_body; g_body.Radius = 1.0f; g_creature.PCurrentAction = &g_action;
    g_brain.PCreature = &g_creature; g_def.ChargeReach = 2.0f; g_def.ChargeDelay = 0.5f;
    g_entries[0].type = 0x36; g_entries[0].component = (CTCBase*)&g_stealth;
    g_target.m_interfaces.first = g_entries; g_target.m_interfaces.last = g_entries + 1;
    CAIStateGroup_SummonerCharge group; group.PBrain = &g_brain; group.PDef = &g_def;

    // 1. dead target: terminate at once
    Reset(group); g_target.m_flags91 = 1; group.Update();
    CHECK(g_terminate == 1 && g_setAction == 0 && g_danger == 0);
    // 2. stealth found, SetCurrentAction refused: terminate (the charge-into temp is built and destroyed)
    Reset(group); g_setActionResult = false; group.Update();
    CHECK(g_danger == 1 && g_dangerSource == &g_creature && g_dangerStatus == 3 && g_lookups == 1);
    CHECK(g_setAction == 1 && g_intoCtor == 1 && g_combatDtor == 1 && g_terminate == 1 && g_nothing == 0);
    // 3. no stealth interface: no notification; action finishes on the second pass -> reached, charge-out-of with the live target
    Reset(group); g_target.m_presentBits[1] = 0; g_action.Finished = 0; g_codes[1] = 77;
    g_nothingResults[0] = 0; g_action.Finished = 0;
    { // first pass: same code, far away (100 > 9); then the action finishes
        g_codes[0] = 77; g_codes[1] = 77; g_codes[2] = 77;
        // make the action finish before the second iteration through the DoNothing stub side effect: emulate via Finished flag set after first DoNothing
    }
    g_action.Finished = 1; group.Update();
    CHECK(g_danger == 0 && g_lookups == 0 && group.NextFrameToCharge == 1015);
    CHECK(g_nothing == 1 && g_isPerforming == 1 && g_stop == 1 && g_outOfCtor2 == 1 && g_perform == 1 && g_performWait == true && g_terminate == 1);
    CHECK(g_combatDtor == 2);
    // 4. action code changes: done without reached -> no charge-out-of, terminate
    Reset(group); g_codes[0] = 77; g_codes[1] = 78; group.Update();
    CHECK(g_nothing == 1 && g_perform == 0 && g_stop == 0 && g_terminate == 1);
    // 5. target comes within reach (dist 1 < reach 3): reached; target dies before the reached block -> single-arg charge-out-of
    Reset(group); g_targetPos.x = 1.0f; g_killTargetAtCode = 1; g_codes[0] = 77; g_codes[1] = 77; group.Update();
    CHECK(g_nothing == 1 && g_outOfCtor1 == 1 && g_outOfCtor2 == 0 && g_perform == 1 && g_terminate == 1 && g_isPerforming == 1);
    // 6. target far: loop keeps idling until DoNothing returns 1 -> terminate without the reached block
    Reset(group); g_nothingResults[2] = 1; group.Update();
    CHECK(g_nothing == 3 && g_codeCalls == 4 && g_perform == 0 && g_terminate == 1);
    // 7. target dies inside the loop: done, not reached
    Reset(group); g_killTargetAtCode = 1; group.Update();
    CHECK(g_nothing == 2 && g_perform == 0 && g_terminate == 1);

    char line[64];
    _snprintf(line, sizeof(line), "0096ff10_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}