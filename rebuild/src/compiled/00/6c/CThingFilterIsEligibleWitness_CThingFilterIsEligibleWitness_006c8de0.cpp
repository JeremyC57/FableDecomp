// CThingFilter_IsEligibleWitness::CThingFilter_IsEligibleWitness(COpinionDeed* deed, CThing* perpetrator, bool useHearing, CThingAICreature* notRecipient, CFaction* faction, bool reverseFactionSearch, bool unknownThreatSearch) -- build the witness filter chain for a deed; perceiving is skipped when the perpetrator's opinion source is always aware
// Leaf: ??0CThingFilter_IsEligibleWitness
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C8DE0, length 170 bytes, `ret 1Ch`, returns `this`.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x50 = CTCOpinionSource).
// Identity: engine_api.tsv gives the seven-argument constructor (COpinionDeed*, CThing*, bool, CThingAICreature*,
//   CFaction*, bool, bool).
// Layout (Ego_r.pdb, 42 bytes, matches retail): Perpetrator +0, PDeed +4, HasOpinions (CThingFilter_HasTC, TC 0x4f) +8,
//   DeedTypeEnabled +0xc, IsPerceivingPerpetrator (thing, use-hearing flag) +0x10, IsNotRecipient +0x18, IsAllied +0x1c,
//   IsEnemy +0x20, UnknownThreatSearch +0x24, UseHearing +0x25, UsePerceiving +0x26, UseIsNot +0x27, UseFaction +0x28,
//   ReverseFactionSearch +0x29; retail CThing interface bitset +0x20 (TC 0x50 = word +0x28 bit 16), interface map
//   +0x44; CTCOpinionSource AlwaysAwareOf +0x75.
// Shape: every member in the initialiser list through the sub-filters' inline constructors (the deed type is the
//   null-safe `deed ? deed->Type : 0`); UsePerceiving is then narrowed in the body from the inline HasTC/GetTC lookup as
//   `UsePerceiving = UsePerceiving && !source->AlwaysAwareOf` -- the && over the known-true flag yields the full-register
//   0/1 (`xor eax,eax ; test dl,dl ; sete al`); a plain `= !x` (or == false, ternary, if/else, accessor) gives `sete dl`.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

struct CTCOpinionSource {
    unsigned char _pad00[0x75];
    bool AlwaysAwareOf;                         // +0x75
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
class CThingAICreature;
class CFaction;
struct COpinionDeed {
    long Type;                                  // +0x00 EOpinionDeedType
};

struct CThingFilter_HasTC {
    long Type;
    CThingFilter_HasTC(long type) : Type(type) { }
};
struct CThingFilter_IsDeedTypeEnabled {
    long DeedType;
    CThingFilter_IsDeedTypeEnabled(long deedType) : DeedType(deedType) { }
};
struct CThingFilter_IsPerceivingThingAndIsThingPerceived {
    CThing* Thing;
    bool UseHearing;
    CThingFilter_IsPerceivingThingAndIsThingPerceived(CThing* thing, bool useHearing) : Thing(thing), UseHearing(useHearing) { }
};
struct CThingFilter_IsNot {
    const void* Thing;
    CThingFilter_IsNot(const void* thing) : Thing(thing) { }
};
struct CThingFilter_IsAlliedWithFaction {
    CFaction* Faction;
    CThingFilter_IsAlliedWithFaction(CFaction* faction) : Faction(faction) { }
};
struct CThingFilter_IsEnemyOfFaction {
    CFaction* Faction;
    CThingFilter_IsEnemyOfFaction(CFaction* faction) : Faction(faction) { }
};

struct CThingFilter_IsEligibleWitness {
    CThing* Perpetrator;                                            // +0x00
    COpinionDeed* PDeed;                                            // +0x04
    CThingFilter_HasTC HasOpinions;                                 // +0x08
    CThingFilter_IsDeedTypeEnabled DeedTypeEnabled;                 // +0x0c
    CThingFilter_IsPerceivingThingAndIsThingPerceived IsPerceivingPerpetrator;   // +0x10
    CThingFilter_IsNot IsNotRecipient;                              // +0x18
    CThingFilter_IsAlliedWithFaction IsAllied;                      // +0x1c
    CThingFilter_IsEnemyOfFaction IsEnemy;                          // +0x20
    bool UnknownThreatSearch;                                       // +0x24
    bool UseHearing;                                                // +0x25
    bool UsePerceiving;                                             // +0x26
    bool UseIsNot;                                                  // +0x27
    bool UseFaction;                                                // +0x28
    bool ReverseFactionSearch;                                      // +0x29
    CThingFilter_IsEligibleWitness(COpinionDeed* deed, CThing* perpetrator, bool useHearing, CThingAICreature* notRecipient,
        CFaction* faction, bool reverseFactionSearch, bool unknownThreatSearch);
};

CThingFilter_IsEligibleWitness::CThingFilter_IsEligibleWitness(COpinionDeed* deed, CThing* perpetrator, bool useHearing,
    CThingAICreature* notRecipient, CFaction* faction, bool reverseFactionSearch, bool unknownThreatSearch)
    : Perpetrator(perpetrator), PDeed(deed), HasOpinions(0x4f), DeedTypeEnabled(deed ? deed->Type : 0),
      IsPerceivingPerpetrator(perpetrator, useHearing), IsNotRecipient(notRecipient), IsAllied(faction), IsEnemy(faction),
      UnknownThreatSearch(unknownThreatSearch), UseHearing(useHearing), UsePerceiving(true), UseIsNot(notRecipient != 0),
      UseFaction(faction != 0), ReverseFactionSearch(reverseFactionSearch)
{
    if (perpetrator->HasTC(0x50))
        UsePerceiving = UsePerceiving && !perpetrator->GetTC<CTCOpinionSource>(0x50)->AlwaysAwareOf;
}