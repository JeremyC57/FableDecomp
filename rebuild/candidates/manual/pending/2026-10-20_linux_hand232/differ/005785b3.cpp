// Grade: DIFFER(262v258) -- retail biased ebp frame `lea ebp,[esp-70h]` (lane never emits it) and this in edi / map in ebx (ours esi / edi); body order otherwise identical
// CTCInventoryExperience::BuyAbility(EHeroAbility ability, long cost) -- buys one level of an ability: snapshots the count-down values, finds (or creates) the ability's undo record, records the experience before and after raising the level and spending the cost, and flags the purchase
// Retail 0x005785B3, 258 bytes, `ret 8`; biased ebp frame (`lea ebp,[esp-70h]`, 0xAC bytes of locals).
// Callees: SaveExperienceValuesForCountDown 0x00570161 (landed), map<EHeroAbility, SExpSpendUndo>::find 0x0057914C
// (returns the node; manifest "BinaryTree_FindLowerBound_Ability"), SExpSpendUndo() 0x005797A4 / copy ctor
// 0x00579C35 / ~SExpSpendUndo 0x005797FA, pair<const EHeroAbility, SExpSpendUndo>(const pair<EHeroAbility,
// SExpSpendUndo>&) 0x00579E1B, map::insert(const value_type&) 0x0057A1AA (pair<iterator,bool> by hidden pointer),
// GetTCHeroExperience 0x0056FF3D, GetTCInventoryAbilities 0x0056FFC5 (manifest "GetTCHeroExperience" twin),
// SExpSpendUndo::SaveBefore/SaveAfter(CTCHeroExperience*) 0x0056E93D / 0x0056F597, CTCInventoryAbilities::
// GetAbilityLevel 0x005EDE93, SetAbilityLevel 0x005EE01A (manifest "CEngineLandscapeRenderer::
// OnRegistedZSortingPatchRelocation" is propagated), GetAbilityStatType 0x005EDEEB, CTCHeroExperience::SpendExperience
// (EHeroExperienceStatCategory, long) 0x006D6590.
// Shape: a missing record is inserted from a block-scoped default SExpSpendUndo through an inline make_pair (key store
// + out-of-line copy ctor) and the out-of-line converting value_type ctor; both pair temporaries die after the insert
// (value_type first), the find is repeated, and the default record dies at the block end. The record is used through
// its node; Levels (+0x10 of the record) is bumped and the purchase flag (+0x21C) set.
// Layout (retail): UndoableAbilityBuys map +0x14C (PDB +0x16C), its header pointer at +0; map node = 16-byte tree
// links + key +0x10 + SExpSpendUndo +0x14; SExpSpendUndo = four 4-byte STLport lists + Levels +0x10 (52 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)

enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
enum EHeroExperienceStatCategory { EHeroExperienceStatCategory_Force32 = 0x7fffffff };
struct CTCHeroExperience {
    void SpendExperience(EHeroExperienceStatCategory category, long amount);   // 0x006D6590
};
struct CTCInventoryAbilities {
    long GetAbilityLevel(EHeroAbility ability);                                // 0x005EDE93
    void SetAbilityLevel(EHeroAbility ability, long level);                    // 0x005EE01A
    EHeroExperienceStatCategory GetAbilityStatType(EHeroAbility ability);      // 0x005EDEEB
};
struct CLongList { void* _M_node; };               // STLport list<long> (4 bytes)
struct SExpSpendUndo {
    CLongList Generals;                            // +0x00
    CLongList Strengths;                           // +0x04
    CLongList Skills;                              // +0x08
    CLongList Wills;                               // +0x0c
    long Levels;                                   // +0x10
    long General, Strength, Skill, Will;           // +0x14
    long aGeneral, aStrength, aSkill, aWill;       // +0x24
    SExpSpendUndo();                               // 0x005797A4
    SExpSpendUndo(const SExpSpendUndo& other);     // 0x00579C35
    ~SExpSpendUndo();                              // 0x005797FA
    void SaveBefore(CTCHeroExperience* experience);   // 0x0056E93D
    void SaveAfter(CTCHeroExperience* experience);    // 0x0056F597
};
struct SAbilityUndoPair {                          // pair<EHeroAbility, SExpSpendUndo> (make_pair result)
    EHeroAbility first;
    SExpSpendUndo second;
    SAbilityUndoPair(const EHeroAbility& a, const SExpSpendUndo& b) : first(a), second(b) {}
};
struct SAbilityUndoValue {                         // pair<const EHeroAbility, SExpSpendUndo> (value_type)
    const EHeroAbility first;
    SExpSpendUndo second;
    SAbilityUndoValue(const SAbilityUndoPair& p);  // 0x00579E1B
};
inline SAbilityUndoPair make_pair(const EHeroAbility& a, const SExpSpendUndo& b) { return SAbilityUndoPair(a, b); }
struct SAbilityUndoNode {                          // _Rb_tree_node<value_type>
    void* _M_color_parent_left_right[4];
    EHeroAbility key;                              // +0x10
    SExpSpendUndo value;                           // +0x14
};
struct SInsertResult {                             // pair<iterator, bool>
    SAbilityUndoNode* first;
    bool second;
    SInsertResult() {}
};
struct CAbilityUndoMap {                           // map<EHeroAbility, SExpSpendUndo>
    SAbilityUndoNode* _M_header;
    long _M_node_count;
    long _M_compare;
    SAbilityUndoNode* end() { return _M_header; }
    SAbilityUndoNode* find(const EHeroAbility& key);           // 0x0057914C
    SInsertResult insert(const SAbilityUndoValue& value);      // 0x0057A1AA
};

struct CTCInventoryExperience {
    unsigned char _pad_000[0x14c];
    CAbilityUndoMap UndoableAbilityBuys;           // +0x14c
    unsigned char _pad_158[0x21c - 0x158];
    bool AbilityBought;                            // +0x21c (descriptive)
    void SaveExperienceValuesForCountDown();       // 0x00570161
    CTCHeroExperience* GetTCHeroExperience();      // 0x0056FF3D
    CTCInventoryAbilities* GetTCInventoryAbilities();   // 0x0056FFC5
    void BuyAbility(EHeroAbility ability, long cost);
};

void CTCInventoryExperience::BuyAbility(EHeroAbility ability, long cost)
{
    SaveExperienceValuesForCountDown();
    SAbilityUndoNode* it = UndoableAbilityBuys.find(ability);
    if (it == UndoableAbilityBuys.end()) {
        SExpSpendUndo undo;
        UndoableAbilityBuys.insert(make_pair(ability, undo));
        it = UndoableAbilityBuys.find(ability);
    }
    SExpSpendUndo& undo = it->value;
    undo.SaveBefore(GetTCHeroExperience());
    GetTCInventoryAbilities()->SetAbilityLevel(ability, GetTCInventoryAbilities()->GetAbilityLevel(ability) + 1);
    GetTCHeroExperience()->SpendExperience(GetTCInventoryAbilities()->GetAbilityStatType(ability), cost);
    undo.SaveAfter(GetTCHeroExperience());
    ++undo.Levels;
    AbilityBought = true;
}
