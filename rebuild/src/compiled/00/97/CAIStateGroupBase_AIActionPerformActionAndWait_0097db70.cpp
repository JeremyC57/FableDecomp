// CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced) -- wait until the creature is free, post the action in a brain output (flag 8), then yield while the creature performs (optionally only while it is still this action's id); 1 = stopping, 2 = done
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097DB70, 293 bytes (`ret 8`).
// Callees: CAIStateGroupBase::IsStopping 0x008FD010, CCreatureActionBase::GetActionID 0x00692620 (manifest
// "CWorld::DrawGetEnvironment" is propagated), CThingCreatureBase::IsFreeToPerformAction 0x00662210, PBrain virtual
// slot 2 (+0x08, yield), CBrainOutput::CBrainOutput 0x00835620 / ~CBrainOutput 0x006D60C0 (out of line here, shared
// by both returns), the action's virtual slot 11 (+0x2c), CCountedPointer<..>::Reset 0x00668D90,
// CAIBrain::SetBrainOutput 0x0088B8F0, CThingCreatureBase::IsPerformingAnyAction 0x006622A0.
// Shape: the landed AIAction_PerformAction 0x0097D8B0 (free-to-perform wait loop, out-of-line ~CBrainOutput) followed
// by the AIAction_CombatPerformActionAndWait wait loop (the bool hoisted into bl, `id == current->GetActionID()`).
// Layout: the creature's current action at +0x100, the action id at +0x28.
// Leaf: AIAction_PerformActionAndWait
class CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
};

struct CCountBlock                                  // CCountedPointer's shared block
{
    long Count;                                     // +0x00
    void (__fastcall* Destroy)(void* object);       // +0x04
    void* Object;                                   // +0x08
};

struct CCountedPointer                              // CCountedPointer<..> {object, block}
{
    void* Object;                                   // +0x00
    CCountBlock* Block;                             // +0x04
    void Reset(void* object);                       // 0x00668D90
    ~CCountedPointer()
    {
        if (Block)
        {
            --Block->Count;
            if (Block->Count == 0)
            {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};

class CBrainOutput                                 // 0x48 bytes
{ public:
    unsigned long Flags;                            // +0x00 (8: perform action, 0x20: pump combat actions)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                        // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                // +0x40 CIntelligentPointer<const CThing>
    CBrainOutput();                                 // 0x00835620
    ~CBrainOutput();                                // 0x006D60C0
};

class CAIBrain
{ public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                           // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThing* Creature;                               // +0x18
    void SetBrainOutput(const CBrainOutput& output);  // 0x0088B8F0 (manifest "CQuestManager::RemoveAvailableQuestCardType" is propagated)
};

class CCreatureActionBase
{ public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                    // slot 11 (+0x2c)
    long GetActionID() const;                       // 0x00692620
};

class CThingCreatureBase
{ public:
    unsigned char _pad[0x100];
    CCreatureActionBase* CurrentAction;             // +0x100
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
    bool IsPerformingAnyAction();                   // 0x006622A0
};

enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2 };

class CAIStateGroupBase
{ public:
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    bool IsStopping();                              // 0x008FD010
protected:
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& actionRef, bool stopWhenReplaced);
};

EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase& actionRef, bool stopWhenReplaced)
{
    CCreatureActionBase* action = (CCreatureActionBase*)&actionRef;
    if (IsStopping())
        return AI_ACTION_STOPPING;
    long id = action->GetActionID();
    while (!((CThingCreatureBase*)PBrain->Creature)->IsFreeToPerformAction(action) && !IsStopping())
        PBrain->Yield();
    if (IsStopping())
        return AI_ACTION_STOPPING;
    CBrainOutput output;
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
    PBrain->SetBrainOutput(output);
    PBrain->Yield();
    while (((CThingCreatureBase*)PBrain->Creature)->IsPerformingAnyAction() && !IsStopping() &&
           (!stopWhenReplaced || id == ((CThingCreatureBase*)PBrain->Creature)->CurrentAction->GetActionID()))
        PBrain->Yield();
    if (IsStopping())
        return AI_ACTION_STOPPING;
    return AI_ACTION_DONE;
}