// SExpSpendUndo::SaveAfter(CTCHeroExperience* experience) -- after a spend: turns the saved amounts into the amounts spent (before minus the hero's current values), pushes each onto its undo list and adds it to the running totals
// Leaf: SaveAfter
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056F597, 126 bytes, __fastcall, `ret 4`; Ego_r.pdb ?SaveAfter@SExpSpendUndo@@QAEXPAVCTCHeroExperience@@@Z.
// Pragma: #pragma optimize("s", on) -- size TU (unaligned entry), the CTCInventoryExperience module.
// Callees: CTCHeroExperience::GetStatExperience(EHeroExperienceStatCategory) 0x006D6510 (categories 0..2); STLport
//   list<long>::push_back(const long&) 0x0042B687 (not inlined in the size TU; landed as "LinkedList_PushFront", but it
//   inserts before the header, i.e. at the end) on Generals, Strengths, Skills, Wills.
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

struct CLongList {                                    // STLport list<long>: the header node pointer only (4 bytes)
    SLongListNode* _M_node;
    CLongList(const CLongList& other);                // 0x00579A1C
    long& back() { return _M_node->_M_prev->_M_data; }
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

void SExpSpendUndo::SaveAfter(CTCHeroExperience_Calls* experience)
{
    aGeneral -= experience->GetExperiencePointsAvailableToSpend();
    aStrength -= experience->GetStatExperience(HERO_STAT_STRENGTH);
    aSkill -= experience->GetStatExperience(HERO_STAT_SKILL);
    aWill -= experience->GetStatExperience(HERO_STAT_WILL);
    Generals.push_back(aGeneral);
    Strengths.push_back(aStrength);
    Skills.push_back(aSkill);
    Wills.push_back(aWill);
    General += aGeneral;
    Strength += aStrength;
    Skill += aSkill;
    Will += aWill;
}