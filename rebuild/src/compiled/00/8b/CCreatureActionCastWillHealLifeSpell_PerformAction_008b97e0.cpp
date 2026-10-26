// CCreatureAction_CastWillHealLifeSpell::PerformAction() -- add the owner's "CTCHealLife" component (AI cast level for an AI-state owner), hand it the action's target when that is alive, and start its healing effect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B97E0, length 237 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("CTCHealLife" @ 0x0123D19C),
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60, CTCHealLife::SetAICast(long) 0x007C3EA0, CTCHealLife::StartHealingEffect 0x007C4430, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
// dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: as the landed-in-chunk CCreatureAction_CastTimeSpell::Initialise 0x008CD730: the component name is an implicit literal temporary,
// the new component is kept in ebp, an AI-state owner (thing byte +0x10 == 1) gets the cast level, the target is a named copy of the
// action's +0x2c pointer, and the component's target pointer is assigned through an inline by-value operator= when the target is alive.
// Layout: Level at retail +0x114 (PDB +0x12c).
class CTCParamsBase;
class CBaseObject;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CThing;
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

class CThing { public:
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

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08 (the owning creature)
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointerThing PItemRemoved;      // +0x2c (CCreatureActionBase, Ego_r.pdb name; the spell's target here)
    unsigned char _pad34[0x114 - 0x34];
    virtual void Initialise();                  // 0x00857520
};

class CTCHealLife { public:                         // the component added by name
    unsigned char _pad00[0x1c];
    CIntelligentPointerThing PTarget;           // +0x1c
    void SetAICast(long level);                 // 0x007C3EA0 (stores +0x18; manifest label "CObservable::AddExclusiveObserver" is propagated)
    void StartHealingEffect();                  // 0x007C4430
};

class CCreatureAction_CastWillHealLifeSpell : public CCombatActionBase { public:
    long Level;                            // +0x114 (PDB +0x12c)
    virtual void PerformAction();
};

void CCreatureAction_CastWillHealLifeSpell::PerformAction()
{
    CTCHealLife* tc = (CTCHealLife*)PCreature.GetPItem()->AddTC("CTCHealLife", true, 0);
    if (PCreature.GetPItem()->m_state == 1)
        tc->SetAICast(Level);
    CIntelligentPointerThing target = PItemRemoved;
    if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1))
        tc->PTarget = target;
    tc->StartHealingEffect();
}