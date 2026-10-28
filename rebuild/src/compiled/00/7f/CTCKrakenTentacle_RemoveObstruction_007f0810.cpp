// CTCKrakenTentacle::RemoveObstruction() -- kill the obstruction thing if it is still alive, clear PObstruction and reset QueuedStrikeTick
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F0810, length 120 bytes. Callers 0x007F089A, 0x007F13A7, 0x007F1584 (CTCKrakenTentacle)
// and 0x008CF5CA (CCreatureAction_TentacleStrikeInto::Initialise).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x4), CThing::Kill(bool) @ 0x004C9B80, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 (x2) / dtor @ 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: PDB CTCKrakenTentacle (0x50) matches retail here: PObstruction +0x18, QueuedStrikeTick +0x2C.
// Shape: IS_THING_ALIVE inlined around Kill(true) only; the clear through a default-constructed stack CIntelligentPointer
// (landed 0x008C99C0 form) and the tick reset are unconditional; the temporary sits in its own `{}` block so the tick store
// follows its destructor call (at function scope VC7.1 hoists the store above the dtor).
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool immediately);            // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCKrakenTentacle { public:
    virtual void v00();
    CThing* ParentThing;                    // +0x04 (CTCBase)
    unsigned char _base08[4];
    void* PTentacleDef;                     // +0x0c CDefPointer<CKrakenTentacleDef>
    CBaseIntelligentPointer PKraken;        // +0x10
    CBaseIntelligentPointer PObstruction;   // +0x18
    CBaseIntelligentPointer PRippleEffect;  // +0x20
    bool CanStrike;                         // +0x28
    bool Tracking;                          // +0x29
    bool IsStriking;                        // +0x2a
    long QueuedStrikeTick;                  // +0x2c
    void RemoveObstruction();
};

void CTCKrakenTentacle::RemoveObstruction()
{
    if (IS_THING_ALIVE(PObstruction))
        PObstruction.GetPItem()->Kill(true);
    {
        CIntelligentPointer none;
        none.SetPItem(0);
        PObstruction.SetPItem(none.GetPItem());
    }
    QueuedStrikeTick = 0;
}