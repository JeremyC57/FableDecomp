// CAIStateGroupBase::AIAction_PumpCombatActions() -- unless stopping, post a brain output with the pump-combat-actions flag (0x20), yield once, and report 1 (stopping) or 4 (pumped)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097D980, length 177 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010 (twice), CBrainOutput::CBrainOutput @ 0x00835620,
// CAIBrain::SetBrainOutput(const CBrainOutput&) @ 0x0088B8F0, PBrain virtual slot 2 (+0x08, yield),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (output +0x40), operator delete @ 0x00BFE9BC.
// Shape: the implicit ~CBrainOutput is inlined on both returns (member order reversed: +0x40 pointer, then the
// CCountedPointer at +0x20 whose block {count, destroy, object} is released with the member re-read after each store;
// the dead zero stores of the counted pointer are dropped on the stack object).
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

struct CBrainOutput                                 // 0x48 bytes
{
    unsigned long Flags;                            // +0x00 (8: perform action, 0x20: pump combat actions)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                        // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                // +0x40 CIntelligentPointer<const CThing>
    CBrainOutput();                                 // 0x00835620

};

struct CAIBrain
{
    virtual void v00(); virtual void v01();
    virtual void Yield();                           // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThing* Creature;                               // +0x18
    void SetBrainOutput(const CBrainOutput& output);  // 0x0088B8F0 (manifest "CQuestManager::RemoveAvailableQuestCardType" is propagated)
};

struct CAIStateGroupBase
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    bool IsStopping();                              // 0x008FD010
    long AIAction_PumpCombatActions();
};

long CAIStateGroupBase::AIAction_PumpCombatActions()
{
    if (IsStopping())
        return 1;
    CBrainOutput output;
    output.Flags |= 0x20;
    PBrain->SetBrainOutput(output);
    PBrain->Yield();
    if (IsStopping())
        return 1;
    return 4;
}
