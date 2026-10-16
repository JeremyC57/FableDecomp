// CCreatureAction_CastTimeSpell::Initialise() -- base Initialise; if the owner has no bullet-time component yet, add "CTCBulletTime" and hand it the action's target when that is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CD730, length 229 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0 ("CTCBulletTime" @ 0x0123C408), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60,
// CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: presence bit 0x92 (dword +0x30, bit 18) on a fresh owner fetch; the component name is an implicit literal temporary
// (built before `push 0 ; push 1`); the target is a named copy of the action's +0x2c pointer; the component's +0x3c pointer is
// assigned through an inline by-value operator= (the parameter copy lands in the dead string slot).
struct CTCParamsBase;
struct CBaseObject;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CThing;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointerThing& operator=(CIntelligentPointerThing other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    unsigned char _pad44[0x91 - 0x44];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    CBaseObject* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};

struct CCombatActionBase {
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08 (the owning creature)
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointerThing PItemRemoved;      // +0x2c (CCreatureActionBase, Ego_r.pdb name; the spell's target here)
    unsigned char _pad34[0x114 - 0x34];
    virtual void Initialise();                  // 0x00857520
};

struct CTCBulletTime {                          // the interface-0x92 component
    unsigned char _pad00[0x3c];
    CIntelligentPointerThing PTarget;           // +0x3c
};

struct CCreatureAction_CastTimeSpell : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_CastTimeSpell::Initialise()
{
    CCombatActionBase::Initialise();
    if (!PCreature.GetPItem()->HasTC(0x92)) {
        CTCBulletTime* tc = (CTCBulletTime*)PCreature.GetPItem()->AddTC("CTCBulletTime", true, 0);
        CIntelligentPointerThing target = PItemRemoved;
        if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1))
            tc->PTarget = target;
    }
}