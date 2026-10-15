// CCreatureAction_CastPhysicalShieldSpell::Initialise() -- base Initialise, then unless the creature already has a physical-shield component (interface type 147), add "CTCPhysicalShieldSpell" to it and fill in its level and time limit (heroes only) and its shield health
// Grade: DIFFER(ours v retail) -- register swap only: retail keeps `this` in edi and the PCreature address in esi, ours the other way round (every instruction otherwise identical, 125 bytes both)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B9DD0, length 125 bytes (+ int3 padding).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at
// +8, three times), CCharString::CCharString(const char*, long) @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
// @ 0x004C9D60 (landed naming of 0x008C08B0 / 0x008C4250), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the presence test is the inlined bitset probe (type 147 = bit 19 of the dword at +0x30, `test ..,80000h`); the name is a scoped
// named local re-addressed with `lea` (the landed 0x008C4250 form; the implicit-temporary form gives the same bytes); the new component rides in ebx across the
// string destructor and is NOT null-checked; the hero test is `== true` (`cmp byte ptr [eax+10h],1`) on the creature's byte at +0x10.
// Layout: engine/CCreatureAction_CastPhysicalShieldSpell.h has LevelToCast/TimeLimit/ShieldHealth at the PDB's +0x130/+0x134/+0x138;
// retail CCombatActionBase is 0x114 bytes, so `this` is modelled locally at +0x118 / +0x11c / +0x120.
struct CTCParamsBase;
struct CCharString {                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CTCPhysicalShieldSpell {             // the component AddTC returns: level +0x10, time limit +0x1c, health +0x20
    unsigned char _pad0[0x10];
    long  m_level;                          // +0x10
    unsigned char _pad14[8];
    long  m_timeLimit;                      // +0x1c
    float m_shieldHealth;                   // +0x20
};
struct CThing {
    unsigned char _pad0[0x10];
    bool m_isHero;                          // +0x10
    unsigned char _pad11[0xf];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CTCPhysicalShieldSpell* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
};

struct CCreatureAction_CastPhysicalShieldSpell : CCombatActionBase {
    bool  ImmediateCast;                    // +0x114 (PDB +0x12c)
    bool  OneAnimCast;                      // +0x115 (PDB +0x12d)
    unsigned char _pad116[2];
    long  LevelToCast;                      // +0x118 (PDB +0x130)
    long  TimeLimit;                        // +0x11c (PDB +0x134)
    float ShieldHealth;                     // +0x120 (PDB +0x138)
    virtual void Initialise();
};

void CCreatureAction_CastPhysicalShieldSpell::Initialise()
{
    CCombatActionBase::Initialise();
    if (!(PCreature.GetPItem()->m_presentBits[147 >> 5] & (1UL << (147 & 31)))) {   // the inlined HasTC probe
        CTCPhysicalShieldSpell* pShield;
        {
            CCharString name("CTCPhysicalShieldSpell", -1);
            pShield = PCreature.GetPItem()->AddTC(name, true, 0);
        }
        if (PCreature.GetPItem()->m_isHero == true) {
            pShield->m_level = LevelToCast;
            pShield->m_timeLimit = TimeLimit;
        }
        pShield->m_shieldHealth = ShieldHealth;
    }
}
