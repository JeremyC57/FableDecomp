// CTCHeroStats::CheckForNewExpressions() -- teaches the adult hero the expressions unlocked by renown, morality and the stealth ability
// Leaf: CheckForNewExpressions
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057BE71, 968 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed SetAge 0x0057B05F).
// Callees: CThingCreatureBase::IsChild() @ 0x00661F70, CCharString @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//          CThingPlayerCreature::LearnExpression(const CCharString&, long, bool) @ 0x006AC430,
//          CTCHeroStats::GetPercentageThroughRenownLevel() @ 0x0057AAD9, CTCHeroStats::GetMoralityCategory() @ 0x0057A6B8,
//          std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound const @ 0x004365B0,
//          CTCInventoryAbilities::GetAbilityLevel(EHeroAbility) @ 0x006D6520.
// Globals: 0.5f @ 0x0122F59C. Strings: the EXPRESSION_* names 0x0124F520..0x0124F6BC.
// Shape: each renown level teaches one expression plus a second once the hero is more than half-way through the
// level; the morality category teaches the evil gestures below 0..3 and the good ones above 3..6; ability 5's level
// teaches STEAL (3) and PICKLOCK (6). Names are implicit const char* -> const CCharString& conversions (the temporary
// is built before the pushes). The abilities interface (0x68) goes through the const LowerBound with the
// `found = (it != last && !(wanted < it->type)) ? it : last` pick (the if-form flips the whole function's
// ebx/edi map: the hero takes ebx instead of edi + an [ebp-10h] home).
// Layout: retail RenownLevel +0x70 (PDB +0x78).
// Name fix (agentNF6 2026-11-10): CThingCreatureBase::IsChild 0x00661F70 is const (?IsChild@CThingCreatureBase@@QBE_NXZ, Ego_r.pdb).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public:
    bool IsChild() const;                                  // 0x00661F70
};
class CThingPlayerCreature : public CThingCreatureBase { public:
    bool LearnExpression(const CCharString& name, long index, bool notify);   // 0x006AC430
};

class CTCInventoryAbilities { public:
    long GetAbilityLevel_006d6520(long ability) const;              // 0x006D6520
};

class CTCHeroStats { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x68];
    long          RenownLevel;                       // +0x70 (PDB +0x78)

    float GetPercentageThroughRenownLevel();         // 0x0057AAD9
    long GetMoralityCategory();                      // 0x0057A6B8
    void CheckForNewExpressions();
};

void CTCHeroStats::CheckForNewExpressions()
{
    CThingPlayerCreature* hero = (CThingPlayerCreature*)ParentThing;
    if (hero->IsChild())
        return;
    long renown = RenownLevel;
    if (renown >= 1) {
        hero->LearnExpression("EXPRESSION_SNEER", -1, false);
        if (GetPercentageThroughRenownLevel() > 0.5f)
            hero->LearnExpression("EXPRESSION_TAP", -1, false);
    }
    if (renown >= 2) {
        hero->LearnExpression("EXPRESSION_VICTORY_PUMP", -1, false);
        if (GetPercentageThroughRenownLevel() > 0.5f)
            hero->LearnExpression("EXPRESSION_KISS_MY_ASS", -1, false);
    }
    if (renown >= 3) {
        hero->LearnExpression("EXPRESSION_EVIL_LAUGH", -1, false);
        if (GetPercentageThroughRenownLevel() > 0.5f)
            hero->LearnExpression("EXPRESSION_FLAMENCO", -1, false);
    }
    if (renown >= 4) {
        hero->LearnExpression("EXPRESSION_HEROIC_STANCE", -1, false);
        if (GetPercentageThroughRenownLevel() > 0.5f)
            hero->LearnExpression("EXPRESSION_CROTCH_GRAB", -1, false);
    }
    if (renown >= 5)
        hero->LearnExpression("EXPRESSION_BATTLE_CRY", -1, false);
    long morality = GetMoralityCategory();
    if (morality < 0)
        hero->LearnExpression("EXPRESSION_SHIT", -1, false);
    if (morality < 1)
        hero->LearnExpression("EXPRESSION_PELVIC_THRUST", -1, false);
    if (morality < 2)
        hero->LearnExpression("EXPRESSION_MIDDLE_FINGER", -1, false);
    if (morality < 3)
        hero->LearnExpression("EXPRESSION_AIR_GUITAR", -1, false);
    if (morality > 3)
        hero->LearnExpression("EXPRESSION_APOLOGY", -1, false);
    if (morality > 4)
        hero->LearnExpression("EXPRESSION_THANKS", -1, false);
    if (morality > 5)
        hero->LearnExpression("EXPRESSION_SATURDAY_NIGHT_FEVER", -1, false);
    if (morality > 6)
        hero->LearnExpression("EXPRESSION_BALLET", -1, false);
    const CTCInventoryAbilities* abilities;
    if (ParentThing->PeekTC(TCI_HERO_EXPERIENCE, &abilities)) {
        long stealth = abilities->GetAbilityLevel_006d6520(5);
        if (stealth >= 3)
            hero->LearnExpression("EXPRESSION_STEAL", -1, false);
        if (stealth >= 6)
            hero->LearnExpression("EXPRESSION_PICKLOCK", -1, false);
    }
}