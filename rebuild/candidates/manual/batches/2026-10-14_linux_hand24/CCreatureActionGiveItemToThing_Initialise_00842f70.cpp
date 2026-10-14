// CCreatureAction_GiveItemToThing::Initialise() -- base Initialise, then posts a type-0x14 message event (flags 0x10, priority 5) from the creature's position with the creature and the receiving thing as the two things
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842F70, length 96 bytes. Vtable slot 12.
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8 twice, PReceivingThing at +0xa8 once), CCreatureActionBase::GetPWorld @ 0x00692940 (`PCreature.GetPItem()`
// then the creature's GetWorld @ 0x004C7AA0; manifest label is STL noise), CThing::GetPosition @ 0x006A4D00 (`[this+0x60]+0xc`,
// label propagated), CMessageEventManager::AddMessage(EMessageEventType, ulong, C3DVector*, CThing*, CThing*, float, float, long)
// @ 0x00687540 (manifest prototype).
// Shape: the creature is fetched twice into named locals (ebx for the thing argument, edi for the position call), then the
// manager into a third (ebp; a bare object expression would be evaluated after the arguments), then the eight arguments go right to left: 5, 0.0f, 0.0f, the receiving thing, the creature,
// its position, 0x10, 0x14. Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4): PReceivingThing (engine-header name,
// PDB +0xb4) is at +0xa8, so `this` is modelled locally.
struct C3DVector;
struct CMessageEvent;

struct CThing {                             // engine/CThing.h exists but is not used: only the position accessor is modelled
    const C3DVector& GetPosition() const;   // 0x006A4D00
};

struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority);   // 0x00687540
};

struct CWorld {                             // engine/CWorld.h exists but is not used: only the manager pointer at +0x60 is read
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageEventManager;   // +0x60
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCreatureActionBase {                // engine/CCreatureActionBase.h exists but is not used: retail sizeof 0xa8, the PDB build's 0xb4
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CWorld* GetPWorld() const;              // 0x00692940
};

struct CCreatureAction_GiveItemToThing : CCreatureActionBase {
    CBaseIntelligentPointer PReceivingThing;   // +0xa8 CIntelligentPointer<CThing> (PDB +0xb4)
    CBaseIntelligentPointer PItemBeingGiven;   // +0xb0 CIntelligentPointer<CThing> (PDB +0xbc; untouched here)
    virtual void Initialise();
};

void CCreatureAction_GiveItemToThing::Initialise()
{
    CCreatureActionBase::Initialise();
    CThing* pCreatureThing = PCreature.GetPItem();
    CThing* pCreature = PCreature.GetPItem();
    CMessageEventManager* pManager = GetPWorld()->m_pMessageEventManager;   // named: retail resolves the manager (ebp) before the arguments
    pManager->AddMessage(0x14, 0x10, &pCreature->GetPosition(), pCreatureThing, PReceivingThing.GetPItem(), 0.0f, 0.0f, 5);
}
