// CAIStateGroup_WorkAtSchool::InitCreaturePosition() -- place the teacher beside the school when the state is runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009429C0, length 57 bytes (+ int3 padding).
// Callees: virtual slot 11 (+0x2c, IsRunnable) on this, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (the creature's work-building pointer at CThingAICreature+0x170, reached through
// its inline accessor: `lea ecx,[esi+170h]`), CThingCreatureBase::AutoPlaceNearBuilding
// (CThingBuilding*, float) @ 0x00666EF0.
// Shape: the IdleAtHome @ 0x00917330 shape with the creature kept in esi (named local read before
// the 5.0f push) and the building taken from the creature itself; long result (`mov eax,1` /
// shared `xor eax,eax`). Base modelled locally (the generated header flattens it).
struct CThingBuilding;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingBuilding* GetPItem();                 // 0x00A01B50
};

struct CThingAICreature {
    unsigned char _pad[0x170];
    CBaseIntelligentPointer PWorkBuilding;      // +0x170 CIntelligentPointer<CThingBuilding>
    CThingBuilding* GetPWorkBuilding() { return PWorkBuilding.GetPItem(); }
    bool AutoPlaceNearBuilding(CThingBuilding* building, float radius);    // 0x00666EF0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_WorkAtSchool : CAIStateGroupBase {
    long InitCreaturePosition();
};

long CAIStateGroup_WorkAtSchool::InitCreaturePosition()
{
    if (IsRunnable()) {
        CThingAICreature* creature = PBrain->Creature;
        if (creature->AutoPlaceNearBuilding(creature->GetPWorkBuilding(), 5.0f))
            return 1;
    }
    return 0;
}