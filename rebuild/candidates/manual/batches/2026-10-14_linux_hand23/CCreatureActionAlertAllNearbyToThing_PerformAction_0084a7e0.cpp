// CCreatureAction_AlertAllNearbyToThing::PerformAction() -- alert everything within Radius of the creature to the stored thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A7E0, length 51 bytes. Vtable slot 3 (PerformAction) of
// CCreatureAction_AlertAllNearbyToThing (0x012736A4).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CCreatureActionBase::GetThingManager
// @ 0x00692980 (`this+8` GetPItem then the global @ 0x013B8A1C), the manager's getter @ 0x0051E490 (`[this+24h]->[+20h]`;
// the manifest label "CPixelShader::CountReferences" is propagated onto this two-load getter), and the __fastcall
// member @ 0x0084A720 (this in ecx, the thing in edx, the alerting creature and the float radius on the stack; the
// manifest "CTCTrapBase::UpdateTriggerThings" label is propagated -- the body reads both stack arguments as a
// creature position and a float).
// Shape: the creature is a named local fetched BEFORE the radius conversion (`fild [this+0A8h]` sits between the
// GetPItem call and the pushes: a memory read that retail keeps ahead of the manager calls); arguments right to left:
// the float, the creature, then the object chain. No engine header exists for this class: Radius (long, +0xa8) and
// PThing (plain pointer, +0xac) follow the 0xa8-byte retail CCreatureActionBase.
struct CThing;
struct CThingCreatureBase;

struct CNearbyAlerter {
    void __fastcall AlertAllNearbyToThing(CThing* thing, CThingCreatureBase* alerter, float radius);   // 0x0084A720
};

struct CThingManager {
    CNearbyAlerter* GetNearbyAlerter();     // 0x0051E490
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;            // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

struct CCreatureAction_AlertAllNearbyToThing {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10 rest of CCreatureActionBase
    long    Radius;                         // +0xa8
    CThing* PThing;                         // +0xac
    CThingManager* GetThingManager();       // 0x00692980
};

void CCreatureAction_AlertAllNearbyToThing::PerformAction()
{
    CThingCreatureBase* creature = PCreature.GetPItem();
    GetThingManager()->GetNearbyAlerter()->AlertAllNearbyToThing(PThing, creature, (float)Radius);
}
