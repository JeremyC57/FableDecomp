// CTCShopKeeper::RepressOpinions(bool repress) -- on a change, store the flag; for a creature keeper flip its deed-77 mask entry and switch its opinion deed / reaction masks to the shopkeeper-on-the-job masks, or back to its def's defaults
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007996B0, 214 bytes (`ret 4`).
// Callees: the interface LowerBound @ 0x0040F020, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CTCCreatureOpinionOfHero::SetOpinionDeedMask(const CCharString&) @ 0x006822E0 / SetOpinionReactionMask(const CCharString&)
// @ 0x00681E30 / SetOpinionDeedMask(long) @ 0x006821E0 / SetOpinionReactionMask(long) @ 0x00681CD0. Literals
// "OPINION_DEED_MASK_SHOPKEEPER_ON_THE_JOB" @ 0x0126C284, "OPINION_REACTION_MASK_SHOPKEEPER_ON_THE_JOB" @ 0x0126C258.
// Layout: CTCShopKeeper RepressedOpinions +0x42 (Ego_r.pdb +0x46 less the 4 bytes of the 12-byte vector); CThing ThingClass
// +0x10 (1 = AI creature), interface bitset +0x20 (0x4f = bit 15 of the third word), map +0x44; CTCCreatureOpinionOfHero
// (interface 0x4f) PDef +0x1c (COpinionOfHeroDef ReactionMask +0x34, DeedMask +0x38) and DeedMask (a retail 12-byte
// vector of bool bytes) +0x3c -- the local model of the landed SetOpinionDeedMask 0x006822E0 (engine header has PDB offsets).
// Shape: `repress != RepressedOpinions` compares the argument byte first; HasTC + one-ternary GetTC into a named local; the mask
// entry takes `!repress` from the argument, the branch re-reads the stored flag; each string is a named local in its own block.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                           // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class COpinionOfHeroDef { public:
    unsigned char _pad00[0x34];
    long ReactionMask;                                  // +0x34
    long DeedMask;                                      // +0x38
};
struct CBoolVector {                                    // STLport vector<bool> (byte elements, 12 bytes)
    bool* _M_start; bool* _M_finish; bool* _M_end_of_storage;
    bool& operator[](long i) { return *(_M_start + i); }
};
class CTCCreatureOpinionOfHero { public:
    unsigned char _pad00[0x1c];
    COpinionOfHeroDef* PDef;                            // +0x1c CDefPointer<const COpinionOfHeroDef>
    unsigned char _pad20[0x3c - 0x20];
    CBoolVector DeedMask;                               // +0x3c
    void SetOpinionReactionMask(long defIndex);         // 0x00681CD0
    void SetOpinionReactionMask(const CCharString& defName);   // 0x00681E30
    void SetOpinionDeedMask(long defIndex);             // 0x006821E0
    void SetOpinionDeedMask(const CCharString& defName);       // 0x006822E0
};
class CTCShopKeeper { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x42 - 0x08];
    bool RepressedOpinions;                             // +0x42
    void RepressOpinions(bool repress);
};

void CTCShopKeeper::RepressOpinions(bool repress)
{
    if (repress != RepressedOpinions) {
        CThing* thing = ParentThing;
        RepressedOpinions = repress;
        if (thing->HasTC(0x4f)) {
            CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)thing->GetTC(0x4f);
            if (thing->ThingClass == 1) {
                opinion->DeedMask[0x4d] = !repress;
                if (RepressedOpinions) {
                    {
                        CCharString name("OPINION_DEED_MASK_SHOPKEEPER_ON_THE_JOB", -1);
                        opinion->SetOpinionDeedMask(name);
                    }
                    {
                        CCharString name("OPINION_REACTION_MASK_SHOPKEEPER_ON_THE_JOB", -1);
                        opinion->SetOpinionReactionMask(name);
                    }
                } else {
                    opinion->SetOpinionDeedMask(opinion->PDef->DeedMask);
                    opinion->SetOpinionReactionMask(opinion->PDef->ReactionMask);
                }
            }
        }
    }
}