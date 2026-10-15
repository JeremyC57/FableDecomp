// CTCCoopSpirit::OnHit() -- shows the coop spirit's hit particle: the def's OnHitParticle name looked up in the def string table and handed to AddAppearanceParticle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006707A0, length 50 bytes (+ int3 padding).
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 on the global table @ 0x013CA828 (CCharString by
// value into the 4-byte frame slot), CTCCoopSpirit::AddAppearanceParticle(const CCharString&) @ 0x00670560,
// CCharString::~CCharString @ 0x0099EAE0.
// Layout = Ego_r.pdb: CTCCoopSpirit::PCoopSpiritDef +0x28 (CDefPointer<CCoopSpiritDef const>, one word);
// CCoopSpiritDef::OnHitParticle +0x28, OnStrikeParticle +0x2c (CDefString table positions).
// Shape: the string is a named local in the `push ecx` slot (re-addressed with `lea` for the call and the
// destructor), the def's table position pushed before the hidden result pointer.
// The manifest gives this body and its twin 0x006707E0 the same label; this one reads OnHitParticle.
struct CCharString {
    void* PStringData;                                      // +0 the shared string block
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefStringTable {
    CCharString GetString(long id);                         // 0x009D49B0
};

extern CDefStringTable g_DefStringTable;                    // 0x013CA828

struct CCoopSpiritDef {
    unsigned char _pad[0x28];
    long          OnHitParticle;                            // +0x28 CDefString
    long          OnStrikeParticle;                         // +0x2c CDefString
    long          DefaultParticle;                          // +0x30 CDefString
    long          AttackTargetParticle;                     // +0x34 CDefString
    float         MeleeTargetRange;                         // +0x38
};

struct CTCCoopSpirit {
    void*                 __vftable;                        // +0x00
    unsigned char         _pad04[0x24];                     // +0x04 CTCBase .. Master
    const CCoopSpiritDef* PCoopSpiritDef;                   // +0x28 CDefPointer<CCoopSpiritDef const>
    void AddAppearanceParticle(const CCharString& particleName);   // 0x00670560
    void OnHit();
};

void CTCCoopSpirit::OnHit()
{
    CCharString particle = g_DefStringTable.GetString(PCoopSpiritDef->OnHitParticle);
    AddAppearanceParticle(particle);
}
