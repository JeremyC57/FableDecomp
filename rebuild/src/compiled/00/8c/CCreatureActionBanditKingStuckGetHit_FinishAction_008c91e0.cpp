// CCreatureAction_BanditKingStuckGetHit::FinishAction() -- base FinishAction; restart the creature on a CCreatureAction_BanditKingUnstuck (code 150); kill the still-alive stuck weapon thing and clear its pointer
// Leaf: FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C91E0, 217 bytes (224 with padding), virtual FinishAction.
// Callees: CCombatActionBase::FinishAction 0x00859A20, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CCombatActionBase::CCombatActionBase(owner, code, sound) 0x00858030 under the inlined CCreatureAction_BanditKingUnstuck
//   ctor (vtable 0x01281D9C, rtti-vtables.tsv; bool +0x115 = true), CThingCreatureBase::SetCurrentAction 0x006644F0,
//   ~CCombatActionBase 0x0062E900 (manifest "~CTCLightningOrb" is propagated), CThing::Kill 0x004C9B80,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x01238C6C).
// Layout: retail CCombatActionBase is 0x114 bytes (landed 0x008C5370); the stuck thing pointer is this class's +0x114.
// Shape: the 0x118-byte action temporary sits above the zeroed CSoundPair, whose 8 bytes the later clear-pointer temporary
//   reuses; the clear is the landed "assign through a stack CIntelligentPointer" shape (0x008C99C0).
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
};

class CCombatActionBase;

class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CCreaturePointer : CBaseIntelligentPointer {                       // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }   // inline over 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CCreaturePointer PCreature;                 // +0x08
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();                       // 0x0062E900
    void FinishAction();                        // 0x00859A20
};

class CCreatureAction_BanditKingUnstuck : public CCombatActionBase { public:   // retail sizeof 0x118, vtable 0x01281D9C
    bool m_114;                                 // +0x114
    bool m_bStarted;                            // +0x115
    virtual void v00();
    CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 150, CSoundPair())
        , m_bStarted(true)
    {
    }
};

class CCreatureAction_BanditKingStuckGetHit : public CCombatActionBase { public:
    CBaseIntelligentPointer PStuckThing;        // +0x114
    void FinishAction();
};

void CCreatureAction_BanditKingStuckGetHit::FinishAction()
{
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_BanditKingUnstuck(PCreature.GetPItem()));
    if (IS_THING_ALIVE(PStuckThing)) {
        ((CThing*)PStuckThing.GetPItem())->Kill(true);
        CIntelligentPointer none;
        none.SetPItem(0);
        PStuckThing.SetPItem(((CThing*)none.GetPItem()));
    }
}