// CCreatureAction_WhisperSommersaultForward::PerformAction() -- if the creature's flag bit 13 at +0x28 is set, notify its property 77 changed; then mark the action finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0B40, length 48 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, fetched twice), CBaseObject::OnPropertyChanged(long) @ 0x004C9840 (property 0x4d = 77).
// Shape: the intelligent pointer's address is kept in edi for both GetPItem calls; `test ch,20h` is bit 13 of the
// creature's flag word at +0x28; the Finished byte at +0x114 (retail CCombatActionBase is 0x114 bytes) is stored
// after the branch, between the two pops. PerformAction has no arguments (`ret`).
struct CBaseObject {
    unsigned char _pad[0x28];
    unsigned long m_flags28;                   // +0x28: bit 13 tested here
    void OnPropertyChanged(long propertyId);   // 0x004C9840
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    CBaseObject* GetPItem() const;             // 0x00A01B50
};

struct CCreatureAction_WhisperSommersaultForward {   // CCombatActionBase (0x114 bytes) + Finished
    virtual void v00();
    unsigned char           _pad04[4];         // +0x04
    CBaseIntelligentPointer PCreature;         // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    unsigned char           Finished;          // +0x114
    void PerformAction();
};

void CCreatureAction_WhisperSommersaultForward::PerformAction()
{
    if (PCreature.GetPItem()->m_flags28 & 0x2000)
        PCreature.GetPItem()->OnPropertyChanged(77);
    Finished = 1;
}