// CTCSummonSpell::ResumeForCutscene() -- when cancelled for a cut-scene, clear the mark and bring the live minion back out of limbo
// Leaf: ResumeForCutscene
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079A6A0, length 60 bytes (caller 0x0072BF38).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThing::SetInLimbo(bool) @ 0x004C8CF0.
// Layout: Ego_r.pdb CTCSummonSpell (0x38 bytes, as the landed ??_G 0x0079BDF0): Minion (CIntelligentPointer<CThingAICreature>)
// at the UNALIGNED +0x11, CutSceneCancelled +0x34; the thing flag byte +0x91 bit 0 marks a killed thing.
// Name confirmed by ghidra_out/egor_signatures.tsv (?ResumeForCutscene@CTCSummonSpell@@QAEXXZ).
// Shape: the flag is stored before the minion tests; the minion is re-fetched for every use.
class CBaseObject;

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_thingFlags;                             // +0x91, bit 0: killed
    void SetInLimbo(bool inLimbo);                          // 0x004C8CF0
};

#pragma pack(push, 1)
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCSummonSpell { public:
    void*                   __vftable;                      // +0x00
    CThing*                 ParentThing;                    // +0x04
    bool                    ToBeRemoved;                    // +0x08
    bool                    IsInGlobalUpdateTCs;            // +0x09
    unsigned char           _pad0a[2];
    void*                   PDef;                           // +0x0c CDefPointer<CSpecialAbilitiesSummonSpellDef const>
    bool                    JustReleased;                   // +0x10
    CIntelligentPointer<CThing> Minion;                         // +0x11 CIntelligentPointer<CThingAICreature> (unaligned)
    long                    FadeInTimer;                    // +0x19
    unsigned char           _pad1d[0x34 - 0x1d];
    bool                    CutSceneCancelled;              // +0x34
    void ResumeForCutscene();
};
#pragma pack(pop)

void CTCSummonSpell::ResumeForCutscene()
{
    if (CutSceneCancelled) {
        CutSceneCancelled = false;
        if (Minion.GetPItem() && !(Minion.GetPItem()->m_thingFlags & 1))
            Minion.GetPItem()->SetInLimbo(false);
    }
}