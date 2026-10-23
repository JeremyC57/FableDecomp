// CTCHeroStats::CheckForNewExpressions() -- teaches the adult hero the expressions unlocked by renown, morality and the stealth ability
// Leaf: CheckForNewExpressions
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
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

struct CPresenceBits {
    bool test(long bit) const;                       // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            const CInterfaceMap::SEntry* found = (it != map->last && !(wanted < it->type)) ? it : map->last;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};

struct CThingCreatureBase : CThing {
    bool IsChild();                                  // 0x00661F70
};
struct CThingPlayerCreature : CThingCreatureBase {
    bool LearnExpression(const CCharString& name, long index, bool notify);   // 0x006AC430
};

struct CTCInventoryAbilities {
    long GetAbilityLevel(long ability);              // 0x006D6520
};

struct CTCHeroStats {
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
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x68)) {
        long stealth = abilities->GetAbilityLevel(5);
        if (stealth >= 3)
            hero->LearnExpression("EXPRESSION_STEAL", -1, false);
        if (stealth >= 6)
            hero->LearnExpression("EXPRESSION_PICKLOCK", -1, false);
    }
}