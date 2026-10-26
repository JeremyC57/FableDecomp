// Behaviour test for 006c8de0: src/006c8de0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CThingFilter_IsEligibleWitness::CThingFilter_IsEligibleWitness(COpinionDeed* deed, CThing* perpetrator, bool useHearing, CThingAICreature* notRecipient, CFaction* faction, bool reverseFactionSearch, bool unknownThreatSearch)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

class CTCOpinionSource { public:
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
class COpinionDeed { public:
    long Type;                                  // +0x00 EOpinionDeedType
};

class CThingFilter_HasTC { public:
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
class CThingFilter_IsNot { public:
    const void* Thing;
    CThingFilter_IsNot(const void* thing) : Thing(thing) { }
};
class CThingFilter_IsAlliedWithFaction { public:
    CFaction* Faction;
    CThingFilter_IsAlliedWithFaction(CFaction* faction) : Faction(faction) { }
};
struct CThingFilter_IsEnemyOfFaction {
    CFaction* Faction;
    CThingFilter_IsEnemyOfFaction(CFaction* faction) : Faction(faction) { }
};

class CThingFilter_IsEligibleWitness { public:
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

// ---- stubs and driver ----
static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
int main()
{
    static double tb[(sizeof(CThing) + 7) / 8]; CThing& perp = *(CThing*)tb; memset(tb, 0, sizeof(tb));
    static double sb[(sizeof(CTCOpinionSource) + 7) / 8]; CTCOpinionSource& src = *(CTCOpinionSource*)sb; memset(sb, 0, sizeof(sb));
    g_entries[0].type = 0x4f; g_entries[0].component = 0;
    g_entries[1].type = 0x50; g_entries[1].component = (CTCBase*)&src;
    perp.Components.first = g_entries; perp.Components.last = g_entries + 2;
    COpinionDeed deed; deed.Type = 17;
    CThingAICreature* rec = (CThingAICreature*)0x1234; CFaction* fac = (CFaction*)0x5678;
    // no opinion source: UsePerceiving stays true
    CThingFilter_IsEligibleWitness f1(&deed, &perp, true, rec, fac, false, true);
    CHECK(f1.Perpetrator == &perp && f1.PDeed == &deed && f1.HasOpinions.Type == 0x4f && f1.DeedTypeEnabled.DeedType == 17);
    CHECK(f1.IsPerceivingPerpetrator.Thing == &perp && f1.IsPerceivingPerpetrator.UseHearing == true);
    CHECK(f1.IsNotRecipient.Thing == rec && f1.IsAllied.Faction == fac && f1.IsEnemy.Faction == fac);
    CHECK(f1.UnknownThreatSearch == true && f1.UseHearing == true && f1.UsePerceiving == true);
    CHECK(f1.UseIsNot == true && f1.UseFaction == true && f1.ReverseFactionSearch == false);
    // opinion source present, not always aware: still perceiving
    perp.UsingComponents[0x50 >> 5] |= 1UL << (0x50 & 31);
    CThingFilter_IsEligibleWitness f2(0, &perp, false, 0, 0, true, false);
    CHECK(f2.DeedTypeEnabled.DeedType == 0 && f2.UseIsNot == false && f2.UseFaction == false && f2.ReverseFactionSearch == true);
    CHECK(f2.UsePerceiving == true && f2.UseHearing == false && f2.UnknownThreatSearch == false);
    // always aware: perceiving switched off
    src.AlwaysAwareOf = true;
    CThingFilter_IsEligibleWitness f3(&deed, &perp, false, 0, fac, false, false);
    CHECK(f3.UsePerceiving == false);
    CHECK(*(unsigned char*)&f3.UsePerceiving == 0);
    src.AlwaysAwareOf = false;
    CThingFilter_IsEligibleWitness f4(&deed, &perp, false, 0, fac, false, false);
    CHECK(*(unsigned char*)&f4.UsePerceiving == 1);
    if (fails == 0) printf("006c8de0_TEST PASS\n"); else printf("006c8de0_TEST FAIL\n");
    return fails;
}
