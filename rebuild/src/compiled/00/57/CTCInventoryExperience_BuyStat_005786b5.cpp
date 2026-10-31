// CTCInventoryExperience::BuyStat(EHeroTrainableStatType stat, long amount) -- buys one level of a trainable stat for `amount` experience: snapshots the experience values for the count-down, finds (or inserts) the stat's undo record, records the hero's experience before and after raising the stat level and spending, counts the bought level and asks for the screen to be rebuilt
// Grade: RELOCATION_MATCH
// Lever: `#pragma optimize("s", on)` without `("y", off)` gives retail's biased frame (`lea ebp,[esp-70h]` ... `add ebp,70h ; leave`); adding y-off gives the plain `mov ebp,esp` frame (GOTCHAS chunk 302/471/521).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005786B5, length 226 bytes (`ret 8`). Size TU: optimize("s",on) alone (default FPO setting);
// the frame is wider than 0x80 bytes so VC biases ebp (`lea ebp,[esp-70h]` ... `add ebp,70h ; leave`) to keep every local in a byte displacement.
// Callees: CTCInventoryExperience::SaveExperienceValuesForCountDown() @ 0x00570161, map<EHeroTrainableStatType,SExpSpendUndo>::find
// @ 0x00579114 (catalog "BinaryTree_FindLowerBound"), SExpSpendUndo::SExpSpendUndo() @ 0x005797A4 (catalog "CHeroExperienceDef::
// CHeroExperienceDef", propagated), SExpSpendUndo(const SExpSpendUndo&) @ 0x00579C35, ~SExpSpendUndo @ 0x005797FA (catalog "_Dest_val<CMacro>",
// propagated), the value_type converting pair ctor @ 0x00579E4E (catalog "pair<unsigned long,CAnimationEntry>", propagated), the map's
// insert (insert_unique, pair<iterator,bool> through a hidden pointer) @ 0x0057A124 (catalog "BuyStat_FindOrInsert"), CTCInventoryExperience::
// GetTCHeroExperience() @ 0x0056FF3D, SExpSpendUndo::SaveBefore(CTCHeroExperience*) @ 0x0056E93D and SaveAfter @ 0x0056F597, CTCHeroExperience::
// ModifyTrainableStatLevel(EHeroTrainableStatType, long) @ 0x006D6810 and SpendExperience(EHeroTrainableStatType, long) @ 0x006D6F60.
// Layout: retail CTCInventoryExperience UndoableStatBuys at +0x140 (PDB +0x15C; header pointer first), DestroyEverything +0x21C (PDB +0x270);
// retail SExpSpendUndo is 52 bytes (four 4-byte list heads, then Levels +0x10 and the eight experience values); the map node holds the
// key at +0x10 and the record at +0x14.
// Shape: the record is a block-scoped default-constructed local ([ebp-4], destroyed after the re-find at the end of its block); make_pair
// builds pair<EHeroTrainableStatType,SExpSpendUndo> straight into its result ([ebp+30h]: key copy then record copy) and the value_type
// conversion copies it again ([ebp-3Ch]) for insert, whose pair<iterator,bool> result ([ebp+68h]) is discarded; the two pair temporaries
// die in reverse order before the re-find. The map lives in ebx as a reference, the record reference in ebx after it, the node in esi.
#pragma optimize("s", on)
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    template <class U1, class U2> pair(const pair<U1, U2>& other);   // 0x00579E4E for <const EHeroTrainableStatType, SExpSpendUndo>
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
}
class CTCHeroExperience { public:
    void ModifyTrainableStatLevel(EHeroTrainableStatType stat, long delta);   // 0x006D6810
    void SpendExperience(EHeroTrainableStatType stat, long amount);           // 0x006D6F60
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
struct SUndoNode {                                   // _Rb_tree_node<pair<const EHeroTrainableStatType, SExpSpendUndo>>
    long       _M_color;
    SUndoNode* _M_parent;
    SUndoNode* _M_left;
    SUndoNode* _M_right;
    EHeroTrainableStatType first;                    // +0x10
    SExpSpendUndo          second;                   // +0x14
};
struct CUndoMap {                                    // std::map<EHeroTrainableStatType, SExpSpendUndo> (12 bytes)
    typedef std::pair<const EHeroTrainableStatType, SExpSpendUndo> value_type;
    typedef SUndoNode* iterator;
    struct SInsertResult { iterator first; bool second; };   // pair<iterator, bool>
    SUndoNode*    _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    iterator end() { return _M_header; }
    iterator find(const EHeroTrainableStatType& key);        // 0x00579114
    SInsertResult insert(const value_type& value);           // 0x0057A124 (insert_unique)
};
class CTCInventoryExperience { public:
    unsigned char _pad000[0x140];
    CUndoMap      UndoableStatBuys;                  // +0x140
    unsigned char _pad14c[0x21c - 0x14c];
    bool          DestroyEverything;                 // +0x21c
    void SaveExperienceValuesForCountDown();         // 0x00570161
    CTCHeroExperience* GetTCHeroExperience();        // 0x0056FF3D
    void BuyStat(EHeroTrainableStatType stat, long amount);
};
void CTCInventoryExperience::BuyStat(EHeroTrainableStatType stat, long amount)
{
    SaveExperienceValuesForCountDown();
    CUndoMap& undos = UndoableStatBuys;
    CUndoMap::iterator it = undos.find(stat);
    if (it == undos.end()) {
        SExpSpendUndo undo;
        undos.insert(std::make_pair(stat, undo));
        it = undos.find(stat);
    }
    SExpSpendUndo& undo = it->second;
    undo.SaveBefore(GetTCHeroExperience());
    GetTCHeroExperience()->ModifyTrainableStatLevel(stat, 1);
    GetTCHeroExperience()->SpendExperience(stat, amount);
    undo.SaveAfter(GetTCHeroExperience());
    ++it->second.Levels;
    DestroyEverything = true;
}