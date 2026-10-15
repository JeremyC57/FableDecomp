// CTCSummonSpell::CancelForCutscene() -- once, mark the spell as cut-scene cancelled and put the live minion into limbo
// Leaf: CancelForCutscene
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079A660, length 60 bytes (caller 0x0072BD4D).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThing::SetInLimbo(bool) @ 0x004C8CF0.
// Layout: Ego_r.pdb CTCSummonSpell (0x38 bytes, as the landed ??_G 0x0079BDF0): Minion (CIntelligentPointer<CThingAICreature>)
// at the UNALIGNED +0x11, CutSceneCancelled +0x34; the thing flag byte +0x91 bit 0 marks a killed thing.
// Name confirmed by ghidra_out/egor_signatures.tsv (?CancelForCutscene@CTCSummonSpell@@QAEXXZ).
// Shape: the flag is stored before the minion tests; the minion is re-fetched for every use.
struct CBaseObject;

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_thingFlags;                             // +0x91, bit 0: killed
    void SetInLimbo(bool inLimbo);                          // 0x004C8CF0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void*        m_vtable;
    CBaseObject* m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};

struct CTCSummonSpell {
    void*                   __vftable;                      // +0x00
    CThing*                 ParentThing;                    // +0x04
    bool                    ToBeRemoved;                    // +0x08
    bool                    IsInGlobalUpdateTCs;            // +0x09
    unsigned char           _pad0a[2];
    void*                   PDef;                           // +0x0c CDefPointer<CSpecialAbilitiesSummonSpellDef const>
    bool                    JustReleased;                   // +0x10
    CBaseIntelligentPointer Minion;                         // +0x11 CIntelligentPointer<CThingAICreature> (unaligned)
    long                    FadeInTimer;                    // +0x19
    unsigned char           _pad1d[0x34 - 0x1d];
    bool                    CutSceneCancelled;              // +0x34
    void CancelForCutscene();
};
#pragma pack(pop)

void CTCSummonSpell::CancelForCutscene()
{
    if (!CutSceneCancelled) {
        CutSceneCancelled = true;
        if (Minion.GetPItem() && !(Minion.GetPItem()->m_thingFlags & 1))
            Minion.GetPItem()->SetInLimbo(true);
    }
}
