// Grade: DIFFER(422v444) -- VC tail-merges our two 12-byte pos copies (retail keeps one per branch, edx/ecx/edx vs ecx/edx/eax) and the creature for CanSeeTarget is not re-read into ecx before the setg flags
// CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat, CIntelligentPointer<const CThing>, long, float) -- look at a live thing (its helper point or position) until the creature sees it, posting a look-at brain output each pass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097D6F0, length 444 bytes (`ret 14h`).
// Callees: IsStopping @ 0x008FD010, GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10, GetHelperPointOrCentre
//          @ 0x004C1060, CThing::GetPosition @ 0x004C73D0, CanSeeTarget @ 0x006650E0, CBrainOutput @ 0x00835620,
//          CBrainOutput::SetAsLookAtThing @ 0x0093E760, SetBrainOutput @ 0x0088B8F0, brain slot 2 (yield).
// Layout: the base follows engine/CAIStateGroupBase.h (PBrain +4); CBrainOutput per the landed 0x0097D580.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CTCGraphicAppearance {
    C3DVector GetHelperPointOrCentre(float t) const;   // 0x004C1060
};
struct CThing {
    unsigned char _pad0[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector* GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase;
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);
    CCreatureActionBase* m_pObject;
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
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
struct CBrainOutput {
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    unsigned char                       NavigationInput[0x18];   // +0x08
    CCountedPointer_CCreatureActionBase PAction;          // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CIntelligentPointerConstThing       LookAtThing;      // +0x40
    CBrainOutput();                                       // 0x00835620
    void SetAsLookAtThing(const CThing* thing, long lookFlags);   // 0x0093E760
};
class CThingCreatureBase;
struct CThingCreature {
    bool CanSeeTarget(const C3DVector& pos, float tolerance, bool alignBody, bool alignHead);   // 0x006650E0
};
struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThingCreature* Creature;                   // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
enum EAIActionRepeat { AI_ACTION_ONCE = 0, AI_ACTION_REPEAT = 1 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_FAILED = 3, AI_ACTION_RUNNING = 4 };
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    bool IsStopping();                          // 0x008FD010
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointerConstThing target, long lookFlags, float tolerance);
};

EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointerConstThing target, long lookFlags, float tolerance)
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    EAIActionReturn result;
    do {
        if (IS_THING_ALIVE(target)) {
            C3DVector pos;
            if (target.GetPItem()->PGraphicAppearance)
                pos = target.GetPItem()->PGraphicAppearance->GetHelperPointOrCentre(1.0f);
            else
                pos = *target.GetPItem()->GetPosition();
            CThingCreature* creature = PBrain->Creature;
            if (target.GetPItem() != (CThing*)creature && !PBrain->Creature->CanSeeTarget(pos, tolerance, (lookFlags & 2) > 0, (lookFlags & 4) > 0)) {
                CBrainOutput output;
                output.SetAsLookAtThing(target.GetPItem(), lookFlags);
                PBrain->SetBrainOutput(output);
                result = AI_ACTION_RUNNING;
            } else
                result = AI_ACTION_DONE;
        } else
            result = AI_ACTION_FAILED;
        PBrain->Yield();
        if (IsStopping())
            return AI_ACTION_STOPPING;
    } while (repeat == AI_ACTION_REPEAT && result == AI_ACTION_RUNNING);
    return result;
}
