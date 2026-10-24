// SExpSpendUndo::SaveBefore(CTCHeroExperience* experience) -- snapshots the hero's spendable experience and the strength / skill / will stat experience into the undo record's amounts before a spend
// Leaf: SaveBefore
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E93D, 55 bytes, __fastcall, `ret 4`; Ego_r.pdb ?SaveBefore@SExpSpendUndo@@QAEXPAVCTCHeroExperience@@@Z.
// Pragma: #pragma optimize("s", on) -- size TU (unaligned entry), the CTCInventoryExperience module (landed TU-mates 0x005721FA, 0x005724CD).
// Callees: CTCHeroExperience::GetStatExperience(EHeroExperienceStatCategory) 0x006D6510 (thrice, categories 0..2).
// Layout: Ego_r.pdb SExpSpendUndo with 4-byte STLport lists (Generals +0, Strengths +4, Skills +8, Wills +0xc, Levels +0x10,
//   General..Will +0x14..+0x20, aGeneral..aWill +0x24..+0x30, confirmed by the copy constructor 0x00579C35); the hero's
//   spendable experience is the inline GetExperiencePointsAvailableToSpend (+0x14, engine/CTCHeroExperience.h).
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

void SExpSpendUndo::SaveBefore(CTCHeroExperience_Calls* experience)
{
    aGeneral = experience->GetExperiencePointsAvailableToSpend();
    aStrength = experience->GetStatExperience(HERO_STAT_STRENGTH);
    aSkill = experience->GetStatExperience(HERO_STAT_SKILL);
    aWill = experience->GetStatExperience(HERO_STAT_WILL);
}