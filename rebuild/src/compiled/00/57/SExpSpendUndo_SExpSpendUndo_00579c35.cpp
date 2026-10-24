// SExpSpendUndo::SExpSpendUndo(const SExpSpendUndo& other) -- copy constructor of the experience-spend undo record: copies the four undo lists, then the level count, the running totals and the pending amounts
// Leaf: SExpSpendUndo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00579C35, 111 bytes, __fastcall, `ret 4`; Ego_r.pdb ??0SExpSpendUndo@@QAE@ABU0@@Z.
// Pragma: #pragma optimize("s", on) -- size TU (unaligned entry), the CTCInventoryExperience module.
// Callees: STLport list<long>::list(const list&) 0x00579A1C (four times, members in order).
// Shape: the memberwise copy (what the implicit copy constructor generates), emitted out of line as a COMDAT;
//   the ten longs are copied one by one through eax.
// Layout: as SExpSpendUndo::SaveBefore 0x0056E93D (Ego_r.pdb names, 4-byte STLport lists).
#pragma optimize("s", on)
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH = 0, HERO_STAT_SKILL = 1, HERO_STAT_WILL = 2 };

struct CTCHeroExperience_Calls {                      // engine/CTCHeroExperience.h is data-only; call view (as the landed TU-mate 0x005724CD)
    unsigned char _pad00[0x14];
    long ExperiencePointsAvailableToSpend;            // +0x14
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
    long GetStatExperience(EHeroExperienceStatCategory category) const;                 // 0x006D6510
    void ForceSpendableExperienceTo(long amount);                                       // 0x006D6470
    void ForceTrainableStatExperienceTo(EHeroExperienceStatCategory category, long amount);  // 0x006D6830
};

struct SLongListNode {                                // STLport _List_node<long>
    SLongListNode* _M_next;                           // +0x0
    SLongListNode* _M_prev;                           // +0x4
    long _M_data;                                     // +0x8
};

struct CLongListIterator {                            // STLport _List_iterator<long>
    SLongListNode* _M_node;
    CLongListIterator(SLongListNode* node) : _M_node(node) { }
    long& operator*() const { return _M_node->_M_data; }
    CLongListIterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};

struct CLongList {                                    // STLport list<long>: the header node pointer only (4 bytes)
    SLongListNode* _M_node;
    CLongList(const CLongList& other);                // 0x00579A1C
    CLongListIterator end() { return CLongListIterator(_M_node); }
    long& back() { CLongListIterator tmp = end(); --tmp; return *tmp; }
    void push_back(const long& value);                // 0x0042B687
    void pop_back();                                  // 0x005791B5
};

struct SExpSpendUndo {                                // Ego_r.pdb SExpSpendUndo (retail 0x34: 4-byte STLport lists)
    CLongList Generals;                               // +0x00
    CLongList Strengths;                              // +0x04
    CLongList Skills;                                 // +0x08
    CLongList Wills;                                  // +0x0c
    long Levels;                                      // +0x10
    long General;                                     // +0x14
    long Strength;                                    // +0x18
    long Skill;                                       // +0x1c
    long Will;                                        // +0x20
    long aGeneral;                                    // +0x24
    long aStrength;                                   // +0x28
    long aSkill;                                      // +0x2c
    long aWill;                                       // +0x30
    SExpSpendUndo(const SExpSpendUndo& other);
    void SaveBefore(CTCHeroExperience_Calls* experience);
    void SaveAfter(CTCHeroExperience_Calls* experience);
    void RevertExperiencePoints(CTCHeroExperience_Calls* experience);
};

SExpSpendUndo::SExpSpendUndo(const SExpSpendUndo& other)
    : Generals(other.Generals)
    , Strengths(other.Strengths)
    , Skills(other.Skills)
    , Wills(other.Wills)
    , Levels(other.Levels)
    , General(other.General)
    , Strength(other.Strength)
    , Skill(other.Skill)
    , Will(other.Will)
    , aGeneral(other.aGeneral)
    , aStrength(other.aStrength)
    , aSkill(other.aSkill)
    , aWill(other.aWill)
{
}