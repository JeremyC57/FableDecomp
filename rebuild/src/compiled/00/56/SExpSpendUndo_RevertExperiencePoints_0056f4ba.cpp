// SExpSpendUndo::RevertExperiencePoints(CTCHeroExperience* experience) -- undoes the last recorded spend: gives the hero back the last spent general / strength / skill / will amounts, takes them off the running totals, pops the four undo lists and drops one level
// Leaf: RevertExperiencePoints
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056F4BA, 221 bytes, __fastcall, `ret 4`; Ego_r.pdb ?RevertExperiencePoints@SExpSpendUndo@@QAEXPAVCTCHeroExperience@@@Z.
// Pragma: #pragma optimize("y", off) + ("s", on) -- frame-pointer size TU (unaligned entry, `push ebp ; mov ebp,esp`, the
//   last-node pointer spilled into the dead argument slot [ebp+8]), as the landed TU-mate 0x005724CD.
// Shape: back() is STLport's `iterator tmp = end(); --tmp; return *tmp;`, which fixes the last node before each
//   GetStatExperience call (the plain `_M_node->_M_prev->_M_data` form loads it after the call).
// Callees: CTCHeroExperience::ForceSpendableExperienceTo(long) 0x006D6470, GetStatExperience(EHeroExperienceStatCategory)
//   0x006D6510, ForceTrainableStatExperienceTo(EHeroExperienceStatCategory, long) 0x006D6830; STLport list<long>::pop_back
//   0x005791B5 (not inlined in the size TU); back() is inline (`[header+4]` is the last node, its value at +8).
// Layout: as SExpSpendUndo::SaveBefore 0x0056E93D (Ego_r.pdb names, 4-byte STLport lists).
#pragma optimize("y", off)
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

void SExpSpendUndo::RevertExperiencePoints(CTCHeroExperience_Calls* experience)
{
    experience->ForceSpendableExperienceTo(Generals.back() + experience->GetExperiencePointsAvailableToSpend());
    experience->ForceTrainableStatExperienceTo(HERO_STAT_STRENGTH, Strengths.back() + experience->GetStatExperience(HERO_STAT_STRENGTH));
    experience->ForceTrainableStatExperienceTo(HERO_STAT_SKILL, Skills.back() + experience->GetStatExperience(HERO_STAT_SKILL));
    experience->ForceTrainableStatExperienceTo(HERO_STAT_WILL, Wills.back() + experience->GetStatExperience(HERO_STAT_WILL));
    General -= Generals.back();
    Strength -= Strengths.back();
    Skill -= Skills.back();
    Will -= Wills.back();
    Generals.pop_back();
    Strengths.pop_back();
    Skills.pop_back();
    Wills.pop_back();
    --Levels;
}