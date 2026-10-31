// CTCInventoryExperience::BuyAbility(EHeroAbility ability, long cost) -- buys one level of an ability: snapshots the count-down values, finds (or inserts) the ability's undo record, records the experience before and after raising the level and spending the cost, counts the bought level and asks for the screen to be rebuilt
// Leaf: ?BuyAbility@CTCInventoryExperience@@QAEXW4EHeroAbility@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005785B3, 258 bytes, `ret 8`; biased ebp frame (`lea ebp,[esp-70h]`, 0xAC bytes of
//   locals): optimize("s", on) alone, as the landed twin CTCInventoryExperience::BuyStat 0x005786B5.
// Callees: CTCInventoryExperience::SaveExperienceValuesForCountDown 0x00570161 (landed), map<EHeroAbility, SExpSpendUndo>::find
//   0x0057914C (returns the node), SExpSpendUndo() 0x005797A4 / SExpSpendUndo(const SExpSpendUndo&) 0x00579C35 (landed) /
//   ~SExpSpendUndo 0x005797FA (landed), the value_type converting pair ctor 0x00579E1B, the map's insert (insert_unique,
//   pair<iterator,bool> through a hidden pointer) 0x0057A1AA, CTCInventoryExperience::GetTCHeroExperience 0x0056FF3D /
//   GetTCInventoryAbilities 0x0056FFC5 (private, PDB AAE), SExpSpendUndo::SaveBefore 0x0056E93D / SaveAfter 0x0056F597
//   (landed), CTCInventoryAbilities::GetAbilityLevel 0x005EDE93 (const), SetAbilityLevelTo 0x005EE01A (the PDB setter with
//   this signature; the retail body is the plain store `Abilities[ability].Level = level`), GetAbilityStatType 0x005EDEEB
//   (const), CTCHeroExperience::SpendExperience(EHeroExperienceStatCategory, long) 0x006D6590.
// Layout: engine/CTCInventoryExperience.h (retail STLport offsets; CTCHeroExperience / CTCInventoryAbilities from their
//   headers too, only methods added): UndoableAbilityBuys +0x14C (an opaque 12-byte map in the
//   header, viewed through the twin's map model), DestroyEverything +0x21C. Map node = 16-byte tree links + key +0x10 +
//   SExpSpendUndo +0x14; retail SExpSpendUndo = four 4-byte STLport lists + Levels +0x10 + eight longs (52 bytes).
// Shape: the twin BuyStat's: a missing record is inserted from a block-scoped default SExpSpendUndo through make_pair and the
//   out-of-line converting value_type ctor, then the find is repeated. Lever (chunk 609): every record access goes through
//   the node (`it->second`), so the node stays live in esi to the end (`inc [esi+24h]`), &it->second is CSE'd into ebx for
//   SaveBefore/SaveAfter, and VC7.1 colours this edi / map ebx / node esi as retail. A `SExpSpendUndo& undo = it->second`
//   reference (the parked form) frees the node early and permutes all three registers.
#pragma optimize("s", on)
#define CTCInventoryExperience CTCInventoryExperienceLayout
#include "engine/CTCInventoryExperience.h"
#undef CTCInventoryExperience
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience
#define CTCInventoryAbilities CTCInventoryAbilitiesLayout
#include "engine/CTCInventoryAbilities.h"
#undef CTCInventoryAbilities

enum EHeroAbility { HERO_ABILITY_NULL };
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    template <class U1, class U2> pair(const pair<U1, U2>& other);   // 0x00579E1B for <const EHeroAbility, SExpSpendUndo>
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
}
class CTCHeroExperience : public CTCHeroExperienceLayout {
public:
    void SpendExperience(EHeroExperienceStatCategory category, long amount);   // 0x006D6590
};
class CTCInventoryAbilities : public CTCInventoryAbilitiesLayout {
public:
    long GetAbilityLevel(EHeroAbility ability) const;                          // 0x005EDE93
    void SetAbilityLevelTo(EHeroAbility ability, long level);                  // 0x005EE01A
    EHeroExperienceStatCategory GetAbilityStatType(EHeroAbility ability) const;   // 0x005EDEEB
};
struct CLongList {                                   // retail std::list<long>: the header node only
    void* _M_node;
};
struct SExpSpendUndo {                               // retail 52 bytes
    CLongList Generals;                              // +0x00
    CLongList Strengths;                             // +0x04
    CLongList Skills;                                // +0x08
    CLongList Wills;                                 // +0x0c
    long Levels;                                     // +0x10
    long General, Strength, Skill, Will;             // +0x14
    long aGeneral, aStrength, aSkill, aWill;         // +0x24
    SExpSpendUndo();                                 // 0x005797A4
    SExpSpendUndo(const SExpSpendUndo& other);       // 0x00579C35
    ~SExpSpendUndo();                                // 0x005797FA
    void SaveBefore(CTCHeroExperience* experience);  // 0x0056E93D
    void SaveAfter(CTCHeroExperience* experience);   // 0x0056F597
};
struct SAbilityUndoNode {                            // _Rb_tree_node<pair<const EHeroAbility, SExpSpendUndo>>
    long              _M_color;
    SAbilityUndoNode* _M_parent;
    SAbilityUndoNode* _M_left;
    SAbilityUndoNode* _M_right;
    EHeroAbility      first;                         // +0x10
    SExpSpendUndo     second;                        // +0x14
};
struct CAbilityUndoMap {                             // std::map<EHeroAbility, SExpSpendUndo> (12 bytes)
    typedef std::pair<const EHeroAbility, SExpSpendUndo> value_type;
    typedef SAbilityUndoNode* iterator;
    struct SInsertResult { iterator first; bool second; };   // pair<iterator, bool>
    SAbilityUndoNode* _M_header;
    unsigned long     _M_node_count;
    long              _M_key_compare;
    iterator end() { return _M_header; }
    iterator find(const EHeroAbility& key);                  // 0x0057914C
    SInsertResult insert(const value_type& value);           // 0x0057A1AA (insert_unique)
};

class CTCInventoryExperience : public CTCInventoryExperienceLayout {
public:
    void SaveExperienceValuesForCountDown();         // 0x00570161
    void BuyAbility(EHeroAbility ability, long cost);
private:
    CTCHeroExperience* GetTCHeroExperience();        // 0x0056FF3D
    CTCInventoryAbilities* GetTCInventoryAbilities();   // 0x0056FFC5
};

void CTCInventoryExperience::BuyAbility(EHeroAbility ability, long cost)
{
    SaveExperienceValuesForCountDown();
    CAbilityUndoMap& undos = *(CAbilityUndoMap*)UndoableAbilityBuys;
    CAbilityUndoMap::iterator it = undos.find(ability);
    if (it == undos.end()) {
        SExpSpendUndo undo;
        undos.insert(std::make_pair(ability, undo));
        it = undos.find(ability);
    }
    it->second.SaveBefore(GetTCHeroExperience());
    GetTCInventoryAbilities()->SetAbilityLevelTo(ability, GetTCInventoryAbilities()->GetAbilityLevel(ability) + 1);
    GetTCHeroExperience()->SpendExperience(GetTCInventoryAbilities()->GetAbilityStatType(ability), cost);
    it->second.SaveAfter(GetTCHeroExperience());
    ++it->second.Levels;
    DestroyEverything = true;
}