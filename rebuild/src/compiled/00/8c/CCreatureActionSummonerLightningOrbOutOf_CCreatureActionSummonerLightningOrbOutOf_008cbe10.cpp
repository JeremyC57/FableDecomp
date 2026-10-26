// CCreatureAction_SummonerLightningOrbOutOf::CCreatureAction_SummonerLightningOrbOutOf(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), own vtable, then target the orb when it is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CBE10, length 135 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (the temporary, then PTarget), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10. Own vtable 0x01283234 (rtti-vtables.tsv); the temporary's vtable 0x01238C6C.
// Shape: `ret 8`; the sound pair is zeroed with immediates in the 8-byte frame, which the stack CIntelligentPointer<CThing> built from the orb
// re-uses afterwards; the orb test is the IS_THING_ALIVE idiom on the raw argument (null, then dead bit +0x91 bit 0) held in edi; the pointer is
// assigned to PTarget (+0xa8, a CCombatActionBase member) through the temporary (`PTarget.SetPItem(temp.GetPItem())`), whose implicit destructor
// stores no vtable before ~CBaseIntelligentPointer (the landed 0x008C99C0 shape).
// No engine header for this class; modelled locally (CCombatActionBase is 0x114 bytes in retail).
class CThingCreatureBase;
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
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
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];         // +0x04
    CBaseIntelligentPointer PTarget;        // +0xa8 CIntelligentPointer<CThing>
    unsigned char _padb0[0x114 - 0xb0];     // +0xb0
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};
class CCreatureAction_SummonerLightningOrbOutOf : public CCombatActionBase { public:   // vtable 0x01283234
    virtual void v00();
    CCreatureAction_SummonerLightningOrbOutOf(CThingCreatureBase* owner, CThing* orb);
};

CCreatureAction_SummonerLightningOrbOutOf::CCreatureAction_SummonerLightningOrbOutOf(CThingCreatureBase* owner, CThing* orb)
    : CCombatActionBase(owner, 150, CSoundPair())
{
    if (orb != 0 && !(orb->m_flags91 & 1)) {
        CIntelligentPointer target(orb);
        PTarget.SetPItem(target.GetPItem());
    }
}