// CAIStateGroupBase::AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase&, CIntelligentPointer<const CThing>, bool) -- post the action, then while the creature is still performing it (and the target lives, and, if asked, the action was not replaced) keep facing the target (look flags 0x11) one brain tick at a time; 1 when stopping, else 2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097DCB0, 688 bytes (`ret 10h`, aligned frame).
//   ?AIAction_PerformActionTargetFaceAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@V?$CIntelligentPointer@$$CBVCThing@@@@_N@Z
// Callees: IsStopping 0x008FD010, CCreatureActionBase::GetActionID 0x00692620, CBrainOutput::CBrainOutput 0x00835620 (the
//   first output only), the action's virtual slot 11 (+0x2c, Clone), CCountedPointer<CCreatureActionBase>::operator=(T*)
//   0x00668D90, CAIBrain::SetBrainOutput 0x0088B8F0, the brain's virtual slot 2 (+8, yield),
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, CBaseIntelligentPointer GetPItem 0x00A01B50 / ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10, operator delete 0x00BFE9BC.
// Layout: the landed AIAction_PerformActionTargetFace 0x0097DF60 model (CBrainOutput per Ego_r.pdb: Types +0, LookFlags +4,
//   PAction +0x20, LookAtThing +0x40); brain creature +0x18, creature CurrentAction +0x100 (landed 0x0097DB70).
// Shape: natural code; the CBrainOutput ctor is plain inline and VC7.1 itself calls it out of line for the first output
//   and inlines it for the per-tick look output; the counted-pointer assignment from Clone() is the out-of-line operator=.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
};
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing() : CBaseIntelligentPointer() { }
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { SetPItem(other.GetPItem()); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CCreatureActionBase* Clone() const; // slot 11 (+0x2c)
    long GetActionID() const;                   // 0x00692620
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* CurrentAction;         // +0x100
    bool IsPerformingAnyAction();               // 0x006622A0
};
void __fastcall CountedDestroy(void* object);   // 0x00668610
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(CountedDestroy), m_pObject(object) { }
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
    CCountedPointer_CCreatureActionBase() : m_pObject(0), m_pCount(0) { }
    __forceinline void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    __forceinline void Reset(CCreatureActionBase* object)
    {
        Release();
        m_pCount = 0;
        m_pObject = object;
    }
    CCountedPointer_CCreatureActionBase& operator=(CCreatureActionBase* object);   // 0x00668D90
    __forceinline ~CCountedPointer_CCreatureActionBase()
    {
        Release();
        m_pObject = 0;
        m_pCount = 0;
    }
};
class CNavigationInput { public:
    float Destination[3];
    float Proximity;
    long  MovementType;
    float Speed;
    CNavigationInput() : Proximity(0.0f), MovementType(0), Speed(0.0f) { }
};
class CBrainOutput { public:
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    CNavigationInput                    NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;          // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CIntelligentPointerConstThing       LookAtThing;      // +0x40
    __forceinline void SetAsLookAtThing(const CThing* thing, long lookFlags)
    {
        Types |= 4;
        LookAtThing = CIntelligentPointerConstThing(thing);
        LookFlags = lookFlags;
    }
    __forceinline void SetAsPerformAction(const CCreatureActionBase* action)
    {
        Types |= 8;
        PAction = action->Clone();
    }
    CBrainOutput() : NavigationInput(), PAction(), LookAtThing()
    {
        Types = 0;
        LookFlags = 0;
        PAction.Reset(0);
    }
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
    unsigned char _pad04[0x18 - 4];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_RUNNING = 4 };
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    bool IsStopping();                          // 0x008FD010
    EAIActionReturn AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase& action,
                                                            CIntelligentPointerConstThing target, bool stopWhenReplaced);
};


EAIActionReturn CAIStateGroupBase::AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase& action,
                                                                         CIntelligentPointerConstThing target,
                                                                         bool stopWhenReplaced)
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    long id = action.GetActionID();
    CBrainOutput output;
    output.SetAsPerformAction(&action);
    PBrain->SetBrainOutput(output);
    PBrain->Yield();
    while (PBrain->Creature->IsPerformingAnyAction() && !IsStopping() && IS_THING_ALIVE(target)
           && (!stopWhenReplaced || id == PBrain->Creature->CurrentAction->GetActionID())) {
        CBrainOutput look;
        look.SetAsLookAtThing(target.GetPItem(), 0x11);
        PBrain->SetBrainOutput(look);
        PBrain->Yield();
    }
    if (IsStopping())
        return AI_ACTION_STOPPING;
    return AI_ACTION_DONE;
}