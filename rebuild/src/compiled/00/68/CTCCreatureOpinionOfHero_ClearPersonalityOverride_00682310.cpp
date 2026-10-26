// CTCCreatureOpinionOfHero::ClearPersonalityOverride() -- a wife (interface 0xd7) falls back to the married or divorced wife personality; everyone else to the personality named by their opinion-of-hero def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00682310, 188 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0, CTCWife::IsMarriedToThePlayer @ 0x006CEC80,
// CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, SetPersonalityOverride(const CCharString&, bool)
// @ 0x006820E0, SetPersonalityOverride(long, bool) @ 0x00681E60 (the def-index overload; manifest
// "SetPersonalityOverride_BuildOpinionTable"). Literals "OPINION_PERSONALITY_WIFE_NORMAL" @ 0x0125BAE8,
// "OPINION_PERSONALITY_WIFE_DIVORCED" @ 0x0125BAC4.
// Layout: local model -- engine/CTCCreatureOpinionOfHero.h leaves the CTCBase ParentThing (+0x04) unnamed; PDef +0x1c (before
// any container) with COpinionOfHeroDef's default personality index at +0x30; CTCWife (Ego_r.pdb) DivorcedHero +0x24; CThing
// bitset +0x20 (0xd7 = bit 23 of the +0x38 word), map +0x44.
// Shape: HasTC + GetTC (const lookup) with the wife in a named local; each override names its string in its own block and
// returns, so the two string arms and the default arm keep separate epilogues.
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCWife { public:
    unsigned char _pad00[0x24];
    bool DivorcedHero;                                  // +0x24
public:
    bool IsMarriedToThePlayer() const;   // 0x006CEC80
};
class COpinionOfHeroDef { public:
    unsigned char _pad00[0x30];
    long DefaultPersonality;                            // +0x30 (personality def index; named descriptively)
};
class CTCCreatureOpinionOfHero { public:
    virtual void v00();
    const CThing* ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    COpinionOfHeroDef* PDef;                            // +0x1c CDefPointer<const COpinionOfHeroDef>
    void SetPersonalityOverride(long personality, bool force);              // 0x00681E60
    void SetPersonalityOverride(const CCharString& personality, bool force);   // 0x006820E0
    void ClearPersonalityOverride();
};

void CTCCreatureOpinionOfHero::ClearPersonalityOverride()
{
    if (ParentThing->HasTC(0xd7)) {
        CTCWife* wife = (CTCWife*)ParentThing->GetTC(0xd7);
        if (wife->IsMarriedToThePlayer()) {
            CCharString name("OPINION_PERSONALITY_WIFE_NORMAL", -1);
            SetPersonalityOverride(name, true);
            return;
        }
        if (wife->DivorcedHero) {
            CCharString name("OPINION_PERSONALITY_WIFE_DIVORCED", -1);
            SetPersonalityOverride(name, true);
            return;
        }
    }
    SetPersonalityOverride(PDef->DefaultPersonality, true);
}