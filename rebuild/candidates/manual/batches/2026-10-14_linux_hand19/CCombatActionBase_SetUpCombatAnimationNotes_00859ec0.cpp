// CCombatActionBase::SetUpCombatAnimationNotes() -- build the strike events and interrupts; with strike events present and the flag at +0xde clear, ActionTime becomes -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00859EC0, length 44 bytes (+ int3 padding).
// Callees: CCombatActionBase::SetupCombatStrikeEvents @ 0x00859A70, CCombatActionBase::SetupCombatInterrupts
// @ 0x00858A20 (both non-virtual on this).
// StrikeEvents is the retail std::list at +0xbc (head pointer first, size at +0xc0); its emptiness is tested as
// begin() != end() (`mov eax,[esi+0BCh] ; cmp [eax],eax`), not through the size. ActionTime is +0x14
// (Ego_r.pdb, CCreatureActionBase); the bool at +0xde is one of the HasXxx flags of the PDB build (descriptive
// name here). Retail CCombatActionBase is 0x114 bytes, so the layout is modelled locally.
struct CStrikeEventNode {
    CStrikeEventNode* m_pNext;                  // +0x00
    CStrikeEventNode* m_pPrev;                  // +0x04
};

struct CStrikeEventList {                       // retail std::list<CCountedPointer<CStrikeEvent>>
    CStrikeEventNode* m_pHead;                  // +0x00
    long m_size;                                // +0x04
    CStrikeEventNode* begin() const { return m_pHead->m_pNext; }
    CStrikeEventNode* end() const { return m_pHead; }
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[0x14 - 4];             // +0x04
    long ActionTime;                            // +0x14
    unsigned char _pad18[0xbc - 0x18];          // +0x18
    CStrikeEventList StrikeEvents;              // +0xbc
    unsigned char _padc4[0xde - 0xc4];          // +0xc4
    bool m_keepActionTime;                      // +0xde
    unsigned char _paddf[0x114 - 0xdf];         // +0xdf
    void SetupCombatStrikeEvents();             // 0x00859A70
    void SetupCombatInterrupts();               // 0x00858A20
    void SetUpCombatAnimationNotes();
};

void CCombatActionBase::SetUpCombatAnimationNotes()
{
    SetupCombatStrikeEvents();
    SetupCombatInterrupts();
    if (StrikeEvents.begin() != StrikeEvents.end() && !m_keepActionTime)
        ActionTime = -1;
}
