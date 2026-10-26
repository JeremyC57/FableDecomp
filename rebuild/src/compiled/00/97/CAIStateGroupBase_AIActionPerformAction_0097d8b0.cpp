// CAIStateGroupBase::AIAction_PerformAction(CCreatureActionBase* action) -- wait until the creature is free to perform the action (or the group stops), post it in a brain output (flag 8), yield once, and report 1 (stopping) or 2 (performed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097D8B0, length 203 bytes (`ret 4`).
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010, CThingCreatureBase::IsFreeToPerformAction(const CCreatureActionBase*)
// @ 0x00662210, PBrain virtual slot 2 (+0x08, yield), CBrainOutput::CBrainOutput @ 0x00835620, the action's virtual slot 11
// (+0x2c, a counted copy of itself), CCountedPointer<..>::Reset @ 0x00668D90 (output +0x20; manifest label names the
// CIPhysicsFacesSegmentBase instantiation), CAIBrain::SetOutput @ 0x0088B8F0, ~CBrainOutput @ 0x006D60C0.
// Shape: `while (!free && !IsStopping()) Yield();` rotates into retail's loop; the CBrainOutput dtor is emitted out of
// line here (the sibling 0x0097D980 inlines it) and shared by both returns (`lea ecx` before the `je`).
// Fix (sweep 3): the callee 0x0088B8F0 is CAIBrain::SetOutput (PDB name; FINDINGS chunk 482), bound with the PDB class keys
//   (class CAIBrain / CBrainOutput) instead of the invented SetBrainOutput. Same bytes.
struct CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    ~CBaseIntelligentPointer();                     // 0x00A01C10
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

class CBrainOutput                                  // 0x48 bytes
{
public:
    unsigned long Flags;                            // +0x00 (8: perform action, 0x20: pump combat actions)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                        // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                // +0x40 CIntelligentPointer<const CThing>
    CBrainOutput();                                 // 0x00835620
    ~CBrainOutput();                                // 0x006D60C0
};

class CAIBrain
{
public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                           // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThing* Creature;                               // +0x18
    void SetOutput(const CBrainOutput& output);     // 0x0088B8F0 (Ego_r/FableWin ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z; the
                                                    // manifest's "CQuestManager::RemoveAvailableQuestCardType" is propagated)
};

struct CCreatureActionBase
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                    // slot 11 (+0x2c)
};

struct CThingCreatureBase
{
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
};

struct CAIStateGroupBase
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    bool IsStopping();                              // 0x008FD010
    long AIAction_PerformAction(CCreatureActionBase* action);
};

long CAIStateGroupBase::AIAction_PerformAction(CCreatureActionBase* action)
{
    if (IsStopping())
        return 1;
    while (!((CThingCreatureBase*)PBrain->Creature)->IsFreeToPerformAction(action) && !IsStopping())
        PBrain->Yield();
    if (IsStopping())
        return 1;
    CBrainOutput output;
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
    PBrain->SetOutput(output);
    PBrain->Yield();
    if (IsStopping())
        return 1;
    return 2;
}