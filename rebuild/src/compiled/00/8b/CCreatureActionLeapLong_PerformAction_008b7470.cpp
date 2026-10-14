// CCreatureAction_LeapLong::PerformAction() -- if the creature's flag word at +0x28 has bit 13 set, notify it of property 77, then mark the action finished (+0x114)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7470, length 48 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, called twice as retail does), CBaseObject::OnPropertyChanged(long) @ 0x004C9840 (landed naming).
// Shape: `test ch,20h` is the compiler's form of the 0x2000 bit test on the dword; the pointer object at +8 rides in edi.
struct CThing {
    unsigned char _pad[0x28];
    unsigned long m_flags;                  // +0x28
    void OnPropertyChanged(long property);  // 0x004C9840
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;      // +0x08
    unsigned char _body[0x114 - 0x10];
};
struct CCreatureAction_LeapLong : CCombatActionBase {
    unsigned char m_finished;               // +0x114
    virtual void PerformAction();
};

void CCreatureAction_LeapLong::PerformAction()
{
    if (PCreature.GetPItem()->m_flags & 0x2000)
        PCreature.GetPItem()->OnPropertyChanged(77);
    m_finished = 1;
}