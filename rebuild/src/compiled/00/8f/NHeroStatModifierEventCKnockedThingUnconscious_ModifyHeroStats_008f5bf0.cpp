// CKilledThing_008f5bf0::ModifyHeroStats(CTCHeroStats* stats) -- set the hero's morality (0x0057C2E6) to its current value (+0x28) plus the def's change (+4 -> +0x28), then add the def's renown (+0x38, 0x0057D8BE)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5BF0, length 48 bytes.
// Callees: CTCHeroStats::SetMorality @ 0x0057C2E6, CTCHeroStats::AddRenown @ 0x0057D8BE. Two byte-identical bodies (0x008F5BF0 / 0x008F5C20) of two killed-thing classes; named by address. Shape: the def's change loaded before the argument, `add ecx,edx`, `ret 4`.
struct CTCHeroStats {
    unsigned char _pad[0x28];
    long          Morality;                        // +0x28
    void SetMorality(long morality);               // 0x0057C2E6
    void AddRenown(long renown);                   // 0x0057D8BE
};
struct SKilledThingDef_008f5bf0 {
    unsigned char _pad[0x28];
    long          MoralityChange;                  // +0x28
    unsigned char _pad2c[0xc];
    long          RenownChange;                    // +0x38
};
struct CKilledThing_008f5bf0 {
    unsigned char           _pad[4];
    SKilledThingDef_008f5bf0*  m_pDef;                // +0x04
    void ModifyHeroStats(CTCHeroStats* stats);
};

void CKilledThing_008f5bf0::ModifyHeroStats(CTCHeroStats* stats)
{
    stats->SetMorality(stats->Morality + m_pDef->MoralityChange);
    stats->AddRenown(m_pDef->RenownChange);
}