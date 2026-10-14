// CCreatureAction_UseSwitch::PerformAction() -- if the switch (+0xa8) is set and not dead, trigger it with the creature as the user
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842E80, length 64 bytes. Vtable slot 3 (PerformAction).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PSwitch at +0xa8 once into a named local, PCreature at +8 for the argument),
// CThingPhysicalSwitch::TriggerMe(CThingCreatureBase*) @ 0x0083DDC0 (manifest prototype).
// Shape: unlike the re-fetching IS_THING_ALIVE idiom, the switch is fetched ONCE into a local (esi) and both tests read it; `this` rides in edi.
// Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so PSwitch (PDB +0xb4) is at +0xa8 and `this` is modelled locally.
struct CThingCreatureBase;
struct CThingPhysicalSwitch {              // only the dead bit and the call are modelled
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void TriggerMe(CThingCreatureBase* user);   // 0x0083DDC0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +0x04
    CThingPhysicalSwitch* GetPItem() const; // 0x00A01B50
};

struct CCreatureAction_UseSwitch {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CBaseIntelligentPointer PSwitch;        // +0xa8 CIntelligentPointer<CThingPhysicalSwitch> (PDB +0xb4)
};

void CCreatureAction_UseSwitch::PerformAction()
{
    CThingPhysicalSwitch* pSwitch = PSwitch.GetPItem();
    if (pSwitch != 0 && !(pSwitch->m_flags91 & 1))
        pSwitch->TriggerMe((CThingCreatureBase*)PCreature.GetPItem());
}