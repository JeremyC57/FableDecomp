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
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CCreatureActionBase;
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
struct C3DVector { float X, Y, Z; };
struct CNavigationInput {
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long MovementType;                          // +0x10
    float Speed;                                // +0x14
};
struct CBrainOutput {
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

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCScriptedControl {
    unsigned char _pad00[0x18];
    bool m_forceUpdate;                         // +0x18
    unsigned char _pad19[3];
    long ScriptPriority;                        // +0x1c
    bool IsScriptTaskRunning();                 // 0x007122C0
    void InterruptControl();                    // 0x00713850
    void Update();                              // 0x00713910
};
struct CTCLook {
    void TrackNearestInterestingThing(bool now);   // 0x00753840
};
struct CAIBrain {
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

struct CThing {
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
