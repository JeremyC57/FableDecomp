// CAIStateGroup_VillageTaskSecuritySweep::IsStillValid() const -- the task thing is live and either active or valid for this creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093FAD0, length 89 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (five times on the task pointer at
// +0x18), the task's virtual slots 3 (+0xc), 4 (+0x10) and 7 (+0x1c, takes PBrain->Creature).
// Shape: `item && item->IsEnabled() && (item->IsActive() || item->IsValidFor(creature))` with the
// item re-fetched for every use (each GetPItem is a call, so nothing is CSE'd); the virtual call
// with an argument evaluates the object first and pushes the creature afterwards. Bool result
// (`mov al,1` / shared `xor al,al`); the pointer's address is formed once into esi.
struct CThing;

struct CVillageTaskThing {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool IsEnabled();                   // slot 3 (+0x0c)
    virtual bool IsActive();                    // slot 4 (+0x10)
    virtual void v05(); virtual void v06();
    virtual bool IsValidFor(CThing* creature);  // slot 7 (+0x1c)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CVillageTaskThing* GetPItem();              // 0x00A01B50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_VillageTaskSecuritySweep : CAIStateGroupBase {
    void* PTask;                                // +0x14
    CBaseIntelligentPointer PTaskThing;         // +0x18 CIntelligentPointer<CThing>
    bool IsStillValid() const;
};

bool CAIStateGroup_VillageTaskSecuritySweep::IsStillValid() const
{
    CAIStateGroup_VillageTaskSecuritySweep* self = const_cast<CAIStateGroup_VillageTaskSecuritySweep*>(this);
    if (self->PTaskThing.GetPItem() != 0 && self->PTaskThing.GetPItem()->IsEnabled()) {
        if (self->PTaskThing.GetPItem()->IsActive() || self->PTaskThing.GetPItem()->IsValidFor(PBrain->Creature))
            return true;
    }
    return false;
}