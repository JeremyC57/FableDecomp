// CThingObject::ResetDieOffTimer() -- reloads DieOffTimer from the def and sets DiesOff when it is positive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00702CE0, length 30 bytes (called from 0x0087F7B3).
// No callees. Layout: PDef +0xd0 (CDefPointer<const CThingObjectDef>, its def pointer first),
// DieOffTimer +0xd4, DiesOff +0xd8; CThingObjectDef::DieOffTimer at +0x94 (PDB +0x98).
// Shape: the def value is tested (`setg dl`) before either store; both stores from registers.
class CThingObjectDef { public:
    unsigned char _pad00[0x94];
    long          DieOffTimer;                     // +0x94
};
struct CDefPointerThingObjectDef {
    const CThingObjectDef* m_pDef;
    const CThingObjectDef* operator->() const { return m_pDef; }
};
class CThingObject { public:
    virtual void v00();
    unsigned char             _pad04[0xcc];
    CDefPointerThingObjectDef PDef;                // +0xd0
    long                      DieOffTimer;         // +0xd4
    bool                      DiesOff;             // +0xd8
    void ResetDieOffTimer();
};

void CThingObject::ResetDieOffTimer()
{
    DieOffTimer = PDef->DieOffTimer;
    DiesOff = DieOffTimer > 0;
}