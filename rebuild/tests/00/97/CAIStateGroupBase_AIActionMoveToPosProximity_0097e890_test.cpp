// Behaviour test for 0097e890: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CAIStateGroupBase::AIAction_MoveToPosProximity
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat, C3DVector, float, bool&, EMovementType, bool, bool, bool, const CThing*, bool, float) -- steer the creature along its navigation route to within `proximity` of a position, one brain output per tick (move, look at the best face target or look around), until arrived (2), failed (3) or stopped (1)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097E890, 880 bytes (ret 34h). Twin of AIAction_MoveToPos 0x0097EC00.
// Callees: SetNavigationIgnoreThing 0x0097E270 / SetNavigationSpeedScale 0x0097E2D0 (state-group helpers on the creature's
//   navigation component; catalog "AssignCreatureToTask_Helper"/"SetTaskProperty_AtOffset" are propagated), IsStopping
//   0x008FD010, GetPBestTargetToFaceForCreature 0x0097E140 (__fastcall), CTCLook::TrackNearestInterestingThing 0x00753840,
//   CTCCreatureNavigation::GetNextNavPosOnRoute 0x006CBAC0 (CCreatureNavOutput by hidden pointer),
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, the current action's virtual slot 49 (+0xC4),
//   CBrainOutput::CBrainOutput 0x00835620, CThingCreatureBase::GetPosition 0x006A4D00, CBrainOutput::SetAsLookAtThing
//   0x0093E760, CAIBrain::SetBrainOutput 0x0088B8F0, ~CBaseIntelligentPointer 0x00A01C10, operator delete 0x00BFE9BC,
//   the brain's virtual slot 2 (+8, yield). Epsilon 0.0001f @ 0x0129BA3C.
// Layout: state group per engine/CAIStateGroupBase.h (PBrain +4), brain creature +0x18; creature interface bitset +0x20
// (CTCLook 0x43, CTCCreatureNavigation 7) and interface map +0x44; current action +0x100; CBrainOutput per the landed
// 0x0097DF60 model (Types +0, LookFlags +4, NavigationInput +8, PAction +0x20, MoveToPos +0x28, LookAtThing +0x40).
// Shape: in this TU the interface map's lower_bound is inline (no 0x0040F020 call); the CTCLook GetTC runs the loop on its result iterator initialised from the member AFTER the length (the first pointer is read once into eax and copied to esi, `it = last` reuses esi), the type-7 lookup goes through the by-value LowerBound helper (no copy); the CBrainOutput constructor is out
// of line and its destructor inline; the arrival test is `status == 0 || |offset|^2 > eps^2`.
class CThing;
class C3DVector { public:
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
C3DVector operator+(const C3DVector& a, const C3DVector& b);
inline C3DVector operator+(const C3DVector& a, const C3DVector& b)
{
    C3DVector r;
    r.X = a.X + b.X;
    r.Y = a.Y + b.Y;
    r.Z = a.Z + b.Z;
    return r;
}
extern const float EPSILON;                     // 0x0129BA3C (0.0001f)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
inline CInterfaceMap::SEntry* LowerBound(CInterfaceMap::SEntry* first, CInterfaceMap::SEntry* last, long key)
{
    long count = last - first;
    while (0 < count) {
        long count2 = count / 2;
        CInterfaceMap::SEntry* mid = first;
        mid += count2;
        if (mid->type < key) {
            first = ++mid;
            count -= count2 + 1;
        } else
            count = count2;
    }
    return first;
}

class CNavigationInput { public:
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long  MovementType;                         // +0x10
    float Speed;                                // +0x14
};
struct CCreatureNavOutput {
    long Status;                                // 0 arrived, 1 failed, 2 moving
    C3DVector Offset;
};
class CTCLook { public:
    bool TrackNearestInterestingThing(bool track);   // 0x00753840
};
class CTCCreatureNavigation { public:
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool b1, bool b2, bool b3, bool b4, bool b5);   // 0x006CBAC0
};
class CCreatureActionBase { public:
    virtual void v00();
    bool BlocksMovement();                      // virtual slot 49 (+0xc4), declared below
};
struct CCreatureActionVirtuals {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48();
    virtual bool BlocksMovement();              // slot 49 (+0xc4)
};

class CThingCreatureBase { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionVirtuals* CurrentAction;     // +0x100
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long count = m_interfaces.last - m_interfaces.first;
        CInterfaceMap::SEntry* it = m_interfaces.first;
        while (0 < count) {
            long count2 = count / 2;
            CInterfaceMap::SEntry* mid = it + count2;
            if (mid->type < type) {
                it = mid + 1;
                count = count - count2 - 1;
            } else
                count = count2;
        }
        if (it == m_interfaces.last || type < it->type)
            it = m_interfaces.last;
        return it->component;
    }
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            CInterfaceMap::SEntry* it = LowerBound(m_interfaces.first, m_interfaces.last, type);
            *ppOut = (T*)(it == m_interfaces.last || type < it->type ? m_interfaces.last : it)->component;
            return true;
        }
        return false;
    }
    bool IsPerformingAnyAction();               // 0x006622A0
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
CThing* __fastcall GetPBestTargetToFaceForCreature(CThingCreatureBase* creature);   // 0x0097E140

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:
    unsigned long           Types;              // +0x00
    long                    LookFlags;          // +0x04
    CNavigationInput        NavigationInput;    // +0x08
    CCountedPointer         PAction;            // +0x20
    C3DVector               MoveToPos;          // +0x28
    C3DVector               LookAtPos;          // +0x34
    CBaseIntelligentPointer LookAtThing;        // +0x40
    CBrainOutput();                             // 0x00835620
    void SetAsLookAtThing(const CThing* thing, long lookFlags);   // 0x0093E760
    void SetAsMoveToPos(const C3DVector& pos, const CNavigationInput& input)
    {
        Types |= 1;
        MoveToPos = pos;
        NavigationInput = input;
    }
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x14];
    CThingCreatureBase* Creature;               // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};

enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT };
inline bool IsDead(const CThing* thing) { return (((const CThingCreatureBase*)thing)->m_flags91 & 1) != 0; }

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    bool IsStopping();                          // 0x008FD010
    void SetNavigationIgnoreThing(const CThing* thing);   // 0x0097E270
    void SetNavigationSpeedScale(float scale);            // 0x0097E2D0
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& moving,
                                                EMovementType anim, bool lookAround, bool b7, bool b8, const CThing* ignoreThing,
                                                bool b10, float speedScale);
};

EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& moving,
                                                               EMovementType anim, bool lookAround, bool b7, bool b8,
                                                               const CThing* ignoreThing, bool b10, float speedScale)
{
    SetNavigationIgnoreThing(ignoreThing);
    SetNavigationSpeedScale(speedScale);
    if (IsStopping())
        return AI_ACTION_STOPPING;
    EAIActionReturn result = AI_ACTION_RUNNING;
    CNavigationInput input;
    input.Destination = pos;
    input.Proximity = proximity;
    input.MovementType = anim;
    input.Speed = 0.0f;
    do {
        CThing* faceTarget = GetPBestTargetToFaceForCreature(GetCreature());
        if (GetCreature()->HasTC(0x43))
            ((CTCLook*)GetCreature()->GetTC(0x43))->TrackNearestInterestingThing(true);
        CTCCreatureNavigation* navigation;
        GetCreature()->GetInterface(&navigation, 7);
        CCreatureNavOutput output = navigation->GetNextNavPosOnRoute(input, b7, b8, b10, true, false);
        moving = output.Status == 0 || output.Offset.GetLengthSquared() > EPSILON * EPSILON;
        if (!GetCreature()->IsPerformingAnyAction() || !GetCreature()->CurrentAction->BlocksMovement()) {
            switch (output.Status) {
            case 0:
                result = AI_ACTION_DONE;
                break;
            case 1:
                result = AI_ACTION_ABORTED;
                break;
            case 2: {
                CBrainOutput brainOutput;
                brainOutput.SetAsMoveToPos(output.Offset + GetCreature()->GetPosition(), input);
                if (faceTarget && !IsDead(faceTarget))
                    brainOutput.SetAsLookAtThing(faceTarget, 0x97);
                else if (lookAround) {
                    brainOutput.Types |= 0x10;
                    brainOutput.LookFlags = 0x51;
                }
                PBrain->SetBrainOutput(brainOutput);
            }
                result = AI_ACTION_RUNNING;
                break;
            }
        }
        PBrain->Yield();
        if (IsStopping())
            return AI_ACTION_STOPPING;
    } while (repeat == AI_ACTION_REPEAT_UNTIL_DONE && result == AI_ACTION_RUNNING);
    return result;
}

// ---- stubs ----
const float EPSILON = 0.0001f;
static int g_stoppingAt = -1;          // IsStopping returns true on its Nth call (0-based), never when -1
static int g_stoppingCalls = 0;
static CThing* g_faceTarget = 0;
static bool g_performing = false, g_blocks = false;
static int g_navScript[8]; static int g_navIndex = 0;   // status per GetNextNavPosOnRoute call
static int g_brainOutputs = 0, g_lookAtThing = 0, g_destroyed = 0, g_ipDtors = 0;
static unsigned long g_lastTypes = 0; static long g_lastLookFlags = 0; static C3DVector g_lastMove; static CNavigationInput g_lastInput;
static CThingCreatureBase g_creature;
static C3DVector g_creaturePos = { 10.0f, 20.0f, 30.0f };
void CAIStateGroupBase::v00() {}
bool CAIStateGroupBase::IsStopping() { note("stopping"); return g_stoppingCalls++ == g_stoppingAt; }
void CAIStateGroupBase::SetNavigationIgnoreThing(const CThing* thing) { char b[64]; _snprintf(b, sizeof b, "ignore%d", thing != 0); note(b); }
void CAIStateGroupBase::SetNavigationSpeedScale(float scale) { char b[64]; _snprintf(b, sizeof b, "scale%.2f", scale); note(b); }
CThing* __fastcall GetPBestTargetToFaceForCreature(CThingCreatureBase* creature) { CHECK(creature == &g_creature); note("face"); return g_faceTarget; }
bool CTCLook::TrackNearestInterestingThing(bool track) { CHECK(track); note("track"); return true; }
CCreatureNavOutput CTCCreatureNavigation::GetNextNavPosOnRoute(const CNavigationInput& input, bool b1, bool b2, bool b3, bool b4, bool b5)
{
    CHECK(b1 == true && b2 == false && b3 == true && b4 == true && b5 == false);
    CHECK(input.Destination.X == 1.0f && input.Destination.Y == 2.0f && input.Destination.Z == 3.0f);
    CHECK(input.Proximity == 1.5f && input.MovementType == MOVEMENT_DEFAULT && input.Speed == 0.0f);
    CCreatureNavOutput o; o.Status = g_navScript[g_navIndex++]; o.Offset.X = 0.5f; o.Offset.Y = 0.0f; o.Offset.Z = 0.0f;
    if (o.Status == 1) { o.Offset.X = 0.0f; }       // a failed step reports no offset
    char b[64]; _snprintf(b, sizeof b, "nav%d", o.Status); note(b);
    return o;
}
bool CThingCreatureBase::IsPerformingAnyAction() { note("performing"); return g_performing; }
const C3DVector& CThingCreatureBase::GetPosition() const { note("pos"); return g_creaturePos; }
void CCreatureActionBase::v00() {}
#define VD(n) void CCreatureActionVirtuals::v##n() {}
VD(00) VD(01) VD(02) VD(03) VD(04) VD(05) VD(06) VD(07) VD(08) VD(09) VD(10) VD(11) VD(12) VD(13) VD(14) VD(15) VD(16) VD(17)
VD(18) VD(19) VD(20) VD(21) VD(22) VD(23) VD(24) VD(25) VD(26) VD(27) VD(28) VD(29) VD(30) VD(31) VD(32) VD(33) VD(34) VD(35)
VD(36) VD(37) VD(38) VD(39) VD(40) VD(41) VD(42) VD(43) VD(44) VD(45) VD(46) VD(47) VD(48)
bool CCreatureActionVirtuals::BlocksMovement() { note("blocks"); return g_blocks; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtors; }
static void __fastcall DestroyObject(void* object) { ++g_destroyed; (void)object; }
static bool g_giveAction = false;
CBrainOutput::CBrainOutput()
{
    Types = 0; LookFlags = 0; memset(&NavigationInput, 0, sizeof NavigationInput);
    PAction.Object = 0; PAction.Block = 0;
    if (g_giveAction) { PAction.Block = new CCountBlock; PAction.Block->Count = 1; PAction.Block->Destroy = DestroyObject; PAction.Block->Object = this; }
    LookAtThing.m_pItem = 0;
}
void CBrainOutput::SetAsLookAtThing(const CThing* thing, long lookFlags) { ++g_lookAtThing; CHECK(thing == g_faceTarget); CHECK(lookFlags == 0x97); note("lookat"); }
void CAIBrain::v00() {}
void CAIBrain::v01() {}
void CAIBrain::Yield() { note("yield"); }
void CAIBrain::SetBrainOutput(const CBrainOutput& output) { ++g_brainOutputs; g_lastTypes = output.Types; g_lastLookFlags = output.LookFlags; g_lastMove = output.MoveToPos; g_lastInput = output.NavigationInput; note("brain"); }

static CAIBrain g_brain; static CAIStateGroupBase g_group; static CTCLook g_look; static CTCCreatureNavigation g_nav; static CCreatureActionVirtuals g_action;
static CInterfaceMap::SEntry g_entries[4];
static CThingCreatureBase g_deadThing, g_liveThing; static unsigned char g_ignore[4];
static void setup(bool withLook)
{
    memset(&g_creature, 0, sizeof g_creature); g_creature.CurrentAction = &g_action;
    int n = 0;
    g_entries[n].type = 7; g_entries[n].component = (CTCBase*)&g_nav; ++n;
    if (withLook) { g_entries[n].type = 0x43; g_entries[n].component = (CTCBase*)&g_look; ++n; }
    g_creature.m_interfaces.first = g_entries; g_creature.m_interfaces.last = g_entries + n;
    g_creature.m_presentBits[7 >> 5] |= 1UL << (7 & 31);
    if (withLook) g_creature.m_presentBits[0x43 >> 5] |= 1UL << (0x43 & 31);
    g_brain.Creature = &g_creature; g_group.PBrain = &g_brain;
    g_log[0] = 0; g_stoppingCalls = 0; g_stoppingAt = -1; g_navIndex = 0; g_brainOutputs = g_lookAtThing = g_destroyed = g_ipDtors = 0;
    g_performing = g_blocks = false; g_faceTarget = 0; g_giveAction = false;
    memset(&g_deadThing, 0, sizeof g_deadThing); g_deadThing.m_flags91 = 1; memset(&g_liveThing, 0, sizeof g_liveThing);
}
static EAIActionReturn run(EAIActionRepeat repeat, bool& moving, bool lookAround)
{
    CAIStateGroupBase& g = g_group; C3DVector pos = { 1.0f, 2.0f, 3.0f }; unsigned char* ignore = g_ignore;
    return g.AIAction_MoveToPosProximity(repeat, pos, 1.5f, moving, MOVEMENT_DEFAULT, lookAround, true, false, (const CThing*)&ignore, true, 0.75f);
}

int main()
{
    bool moving;
    // 1. stopping at entry
    setup(true); g_stoppingAt = 0; moving = true;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, false) == AI_ACTION_STOPPING);
    CHECK(strcmp(g_log, "ignore1;scale0.75;stopping;") == 0); CHECK(moving == true);

    // 2. one tick, arrived (status 0): done, moving reported, look tracked, no brain output
    setup(true); g_navScript[0] = 0; moving = false;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, false) == AI_ACTION_DONE);
    CHECK(strcmp(g_log, "ignore1;scale0.75;stopping;face;track;nav0;performing;yield;stopping;") == 0); CHECK(moving == true);

    // 3. failed (status 1), no look component: aborted, not moving
    setup(false); g_navScript[0] = 1;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, false) == AI_ACTION_ABORTED);
    CHECK(strcmp(g_log, "ignore1;scale0.75;stopping;face;nav1;performing;yield;stopping;") == 0); CHECK(moving == false);

    // 4. moving (status 2) with a live face target: brain output = move + look at thing; a counted action block is released
    setup(true); g_navScript[0] = 2; g_faceTarget = (CThing*)&g_liveThing; g_giveAction = true;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, true) == AI_ACTION_RUNNING);
    CHECK(strcmp(g_log, "ignore1;scale0.75;stopping;face;track;nav2;performing;pos;lookat;brain;yield;stopping;") == 0);
    CHECK(moving == true); CHECK(g_brainOutputs == 1 && g_lookAtThing == 1); CHECK((g_lastTypes & 1) == 1 && (g_lastTypes & 0x10) == 0);
    CHECK(g_lastMove.X == 10.5f && g_lastMove.Y == 20.0f && g_lastMove.Z == 30.0f);
    CHECK(g_lastInput.Destination.Z == 3.0f && g_lastInput.Proximity == 1.5f); CHECK(g_destroyed == 1 && g_ipDtors == 1);

    // 5. moving with a dead face target and lookAround: look-around flags instead
    setup(true); g_navScript[0] = 2; g_faceTarget = (CThing*)&g_deadThing;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, true) == AI_ACTION_RUNNING);
    CHECK(g_lookAtThing == 0 && g_brainOutputs == 1); CHECK((g_lastTypes & 0x11) == 0x11 && g_lastLookFlags == 0x51); CHECK(g_ipDtors == 1 && g_destroyed == 0);

    // 6. moving, no face target, no lookAround: plain move output
    setup(true); g_navScript[0] = 2;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, false) == AI_ACTION_RUNNING);
    CHECK(g_brainOutputs == 1 && (g_lastTypes & 0x10) == 0 && g_lastLookFlags == 0);

    // 7. a blocking action in progress: the nav result is ignored (result stays running), only yield
    setup(true); g_navScript[0] = 0; g_performing = true; g_blocks = true;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, false) == AI_ACTION_RUNNING);
    CHECK(strcmp(g_log, "ignore1;scale0.75;stopping;face;track;nav0;performing;blocks;yield;stopping;") == 0); CHECK(g_brainOutputs == 0);
    // 7b. performing but not blocking: the switch runs
    setup(true); g_navScript[0] = 0; g_performing = true; g_blocks = false;
    CHECK(run(AI_ACTION_REPEAT_ONCE, moving, false) == AI_ACTION_DONE);

    // 8. repeat until done: two moving ticks then arrival
    setup(true); g_navScript[0] = 2; g_navScript[1] = 2; g_navScript[2] = 0;
    CHECK(run(AI_ACTION_REPEAT_UNTIL_DONE, moving, false) == AI_ACTION_DONE);
    CHECK(g_navIndex == 3 && g_brainOutputs == 2 && g_ipDtors == 2);

    // 9. repeat until done, stopped after the second yield
    setup(true); g_navScript[0] = 2; g_navScript[1] = 2; g_navScript[2] = 2; g_stoppingAt = 2;
    CHECK(run(AI_ACTION_REPEAT_UNTIL_DONE, moving, false) == AI_ACTION_STOPPING);
    CHECK(g_navIndex == 2 && g_brainOutputs == 2);

    // 10. poll mode never loops
    setup(true); g_navScript[0] = 2; g_navScript[1] = 0;
    CHECK(run(AI_ACTION_REPEAT_POLL, moving, false) == AI_ACTION_RUNNING); CHECK(g_navIndex == 1);

    if (fails == 0) printf("0097e890_TEST PASS\n"); else printf("0097e890_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}