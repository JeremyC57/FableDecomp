// Behaviour test for 0x008326A0: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// CThingAICreature::UpdateMovement(float dt) -- a scripted-control creature (interface 0x1f) whose script task is idle or forced either yields to a higher-priority AI state group (interrupting the control) or force-resets its brain, runs one AI update, moves, updates the control and tracks the nearest interesting thing; otherwise (unless brain updates are suppressed) runs the brain and AI update and moves
// Leaf: UpdateMovement
// Callees: LowerBound @ 0x0040F020, CTCScriptedControl::IsScriptTaskRunning @ 0x007122C0, _ftol @ 0x00BFEA70,
// CAIBrain::IsReadyToRunStateGroupWithHigherScriptPriorityThan(long, long) @ 0x0088BAC0, CTCScriptedControl::InterruptControl
// @ 0x00713850, CBrainOutput::CBrainOutput @ 0x00835620, CAIBrain::ForceReset(CBrainOutput*) @ 0x0088BA40,
// CThingAICreature::UpdateAI @ 0x006D5D30, CThingAICreature::UpdateBrain @ 0x006D5A90, the movement component's slot 33 (+0x84),
// CTCScriptedControl::Update @ 0x00713910, CTCLook::TrackNearestInterestingThing @ 0x00753840, ~CBrainOutput @ 0x006D60C0,
// ~CBaseIntelligentPointer @ 0x00A01C10, operator delete @ 0x00BFE9BC.
// Shape: both component lookups are the out-parameter __forceinline GetInterface (the 0x43 one leaves the component in eax:
// `mov eax,[eax+4] ; push 1 ; mov ecx,eax`); the script priority is a named value local read before the _ftol; the
// ForceReset brain comes from an inline GetBrain() accessor (loaded before the output's address); the output's destructor is
// called out of line on the scripted exit and inlined on the brain path (the inline-once band).
// Layout: CBrainOutput from the landed 0x00835620 / 0x006D60C0 / 0x0093DDE0 models; retail CThing interface bitset +0x20,
// CVectorMap +0x44; CThingAICreature movement component +0x60, Brain +0x1a8, flag byte +0x1c8 (descriptive).
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
class CCreatureActionBase;
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;             // +0x08
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;             // +0x00
    SCountBlock*         m_pCount;              // +0x04
    __forceinline ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
class C3DVector { public: float X, Y, Z; };
class CNavigationInput { public:
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long MovementType;                          // +0x10
    float Speed;                                // +0x14
};
class CBrainOutput { public:
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    CNavigationInput                    NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;          // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CBaseIntelligentPointer             LookAtThing;      // +0x40
    CBrainOutput();                             // 0x00835620
    ~CBrainOutput();                            // 0x006D60C0 (body below, this TU)
};
inline CBrainOutput::~CBrainOutput()
{
}

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CTCScriptedControl { public:
    unsigned char _pad00[0x18];
    bool m_forceUpdate;                         // +0x18
    unsigned char _pad19[3];
    long ScriptPriority;                        // +0x1c
    bool IsScriptTaskRunning();                 // 0x007122C0
    void InterruptControl();                    // 0x00713850
    void Update();                              // 0x00713910
};
class CTCLook { public:
    void TrackNearestInterestingThing(bool now);   // 0x00753840
};
class CAIBrain { public:
    bool IsReadyToRunStateGroupWithHigherScriptPriorityThan(long priority, long zone);   // 0x0088BAC0
    bool ForceReset(CBrainOutput* output);      // 0x0088BA40
};
struct CMovement {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void Move(const C3DVector* destination);   // slot 33 (+0x84)
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingAICreature : public CThing {
public:
    unsigned char _pad4c[0x60 - 0x4c];
    CMovement* Movement;                        // +0x60
    unsigned char _pad64[0x1a8 - 0x64];
    CAIBrain* Brain;                            // +0x1a8
    unsigned char _pad1ac[0x1c8 - 0x1ac];
    unsigned char Flags1c8;                     // +0x1c8
    CAIBrain* GetBrain() const { return Brain; }
    bool UpdateAI(bool forced, CBrainOutput* output, C3DVector* destination);   // 0x006D5D30
    bool UpdateBrain(long dt, CBrainOutput* output);   // 0x006D5A90
    void UpdateMovement(float dt);
};

void CThingAICreature::UpdateMovement(float dt)
{
    CTCScriptedControl* control;
    if (GetInterface(&control, 0x1f) && (control->m_forceUpdate || !control->IsScriptTaskRunning())) {
        long priority = control->ScriptPriority;
        if (Brain->IsReadyToRunStateGroupWithHigherScriptPriorityThan(priority, (long)dt)) {
            control->InterruptControl();
            return;
        }
        C3DVector destination;
        CBrainOutput output;
        if (GetBrain()->ForceReset(&output) && UpdateAI(true, &output, &destination))
            Movement->Move(&destination);
        control->Update();
        CTCLook* look;
        if (GetInterface(&look, 0x43))
            look->TrackNearestInterestingThing(true);
        return;
    }
    if (!(Flags1c8 & 2)) {
        C3DVector destination;
        CBrainOutput output;
        bool updated = UpdateBrain((long)dt, &output);
        if (UpdateAI(updated, &output, &destination))
            Movement->Move(&destination);
    }
}

// ---- recording stubs ----
void CBaseIntelligentPointer::v00() { }
static int g_ipDtors = 0;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtors; }
CBrainOutput::CBrainOutput() { Types = 0; LookFlags = 0; PAction.m_pObject = 0; PAction.m_pCount = 0; LookAtThing.m_pItem = 0; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static bool g_taskRunning = false;
static int g_interrupts = 0, g_updates = 0, g_tracks = 0, g_moves = 0;
static long g_priority = -1, g_zone = -1, g_brainDt = -1;
static bool g_ready = false, g_reset = false, g_ai = false, g_brain = false, g_forced = false;
static float g_moveX = 0;
bool CTCScriptedControl::IsScriptTaskRunning() { return g_taskRunning; }
void CTCScriptedControl::InterruptControl() { ++g_interrupts; }
void CTCScriptedControl::Update() { ++g_updates; }
void CTCLook::TrackNearestInterestingThing(bool now) { if (now) ++g_tracks; }
bool CAIBrain::IsReadyToRunStateGroupWithHigherScriptPriorityThan(long priority, long zone) { g_priority = priority; g_zone = zone; return g_ready; }
bool CAIBrain::ForceReset(CBrainOutput* output) { output->Types = 7; return g_reset; }
bool CThingAICreature::UpdateAI(bool forced, CBrainOutput* output, C3DVector* destination) { g_forced = forced; destination->X = 12.5f; return g_ai; }
bool CThingAICreature::UpdateBrain(long dt, CBrainOutput* output) { g_brainDt = dt; return g_brain; }
#define DEFV(n) void CMovement::v##n() { }
DEFV(00) DEFV(01) DEFV(02) DEFV(03) DEFV(04) DEFV(05) DEFV(06) DEFV(07) DEFV(08) DEFV(09)
DEFV(10) DEFV(11) DEFV(12) DEFV(13) DEFV(14) DEFV(15) DEFV(16) DEFV(17) DEFV(18) DEFV(19)
DEFV(20) DEFV(21) DEFV(22) DEFV(23) DEFV(24) DEFV(25) DEFV(26) DEFV(27) DEFV(28) DEFV(29)
DEFV(30) DEFV(31) DEFV(32)
void CMovement::Move(const C3DVector* destination) { ++g_moves; g_moveX = destination->X; }

static void Reset()
{
    g_interrupts = g_updates = g_tracks = g_moves = 0;
    g_priority = g_zone = g_brainDt = -1;
    g_moveX = 0;
}

int main()
{
    static CThingAICreature creature;
    static CMovement movement;
    static CAIBrain brain;
    static CTCScriptedControl control;
    static CTCLook look;
    static CInterfaceMap::SEntry entries[2];
    memset(&creature, 0, sizeof(creature));
    memset(&control, 0, sizeof(control));
    entries[0].type = 0x1f; entries[0].component = (CTCBase*)&control;
    entries[1].type = 0x43; entries[1].component = (CTCBase*)&look;
    creature.Components.first = entries;
    creature.Components.last = entries + 2;
    creature.Movement = &movement;
    creature.Brain = &brain;
    control.ScriptPriority = 42;
    bool ok = true;

    // A: scripted, idle task, a higher-priority group is ready -> interrupt only
    creature.UsingComponents[0] = 0x80000000UL; creature.UsingComponents[2] = 8;
    g_ready = true; g_taskRunning = false;
    Reset(); creature.UpdateMovement(3.75f);
    ok = ok && g_interrupts == 1 && g_priority == 42 && g_zone == 3 && g_updates == 0 && g_moves == 0;

    // B: scripted, forced, no higher group: reset + AI + move + update + look
    control.m_forceUpdate = true; g_taskRunning = true; g_ready = false; g_reset = true; g_ai = true;
    Reset(); creature.UpdateMovement(2.0f);
    ok = ok && g_interrupts == 0 && g_moves == 1 && g_moveX == 12.5f && g_forced == true && g_updates == 1 && g_tracks == 1;

    // B2: no look component, reset fails -> no move, update still runs
    creature.UsingComponents[2] = 0; g_reset = false;
    Reset(); creature.UpdateMovement(2.0f);
    ok = ok && g_moves == 0 && g_updates == 1 && g_tracks == 0;

    // C: script task running and not forced -> brain path
    control.m_forceUpdate = false; g_taskRunning = true; g_brain = false; g_ai = true;
    Reset(); creature.UpdateMovement(5.9f);
    ok = ok && g_brainDt == 5 && g_forced == false && g_moves == 1 && g_updates == 0;

    // D: no scripted control, brain suppressed -> nothing
    creature.UsingComponents[0] = 0; creature.Flags1c8 = 2;
    Reset(); creature.UpdateMovement(1.0f);
    ok = ok && g_brainDt == -1 && g_moves == 0;

    // E: no scripted control, brain runs and reports an update, AI declines the move
    creature.Flags1c8 = 0; g_brain = true; g_ai = false;
    Reset(); creature.UpdateMovement(7.0f);
    ok = ok && g_brainDt == 7 && g_forced == true && g_moves == 0;

    ok = ok && g_ipDtors >= 4;
    if (ok) printf("008326a0_TEST PASS\n");
    else printf("008326a0_TEST FAIL\n");
    return ok ? 0 : 1;
}