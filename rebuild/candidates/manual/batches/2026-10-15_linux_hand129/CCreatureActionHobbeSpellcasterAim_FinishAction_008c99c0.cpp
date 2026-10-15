// CCreatureAction_HobbeSpellcasterAim::FinishAction() -- base FinishAction, then if the aimed spell thing is still alive kill it and clear the pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C99C0, length 119 bytes (+ int3 padding).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the spell
// pointer at +0x114, three times, then on the temporary), CThing::Kill @ 0x004C9B80 (one argument, 1),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (temporary, then the member),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: IS_THING_ALIVE inlined; `Kill(true)` pushes its argument before the chained GetPItem; the clear is the landed
// "assign through a stack CIntelligentPointer" shape (0x0084B1C0): default-construct (inlined CIntelligentPointer ctor stores the
// vtable 0x01238C6C after the base ctor), SetPItem(0), read it back into the member, destroy -- the 8-byte frame is that temporary.
// Retail CCombatActionBase is 0x114 bytes, so the spell pointer is this class's first member.
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool immediately);            // 0x004C9B80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    ~CBaseIntelligentPointer();             // 0x00A01C10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[0x114 - 4];
};
struct CCreatureAction_HobbeSpellcasterAim : CCombatActionBase {
    CBaseIntelligentPointer PSpell;         // +0x114 the spell thing being aimed
    virtual void FinishAction();
};

void CCreatureAction_HobbeSpellcasterAim::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PSpell)) {
        PSpell.GetPItem()->Kill(true);
        CIntelligentPointer none;
        none.SetPItem(0);
        PSpell.SetPItem(none.GetPItem());
    }
}
