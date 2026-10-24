// Behaviour test for 0x0056F4BA SExpSpendUndo::RevertExperiencePoints: gives back the last spent amounts, takes them off the totals, pops the lists, drops a level (the four SExpSpendUndo methods share one model; this test exercises the named one)
#include <cstdio>
#include <cstdlib>
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH = 0, HERO_STAT_SKILL = 1, HERO_STAT_WILL = 2 };
struct CTCHeroExperience_Calls {
    long ExperiencePointsAvailableToSpend;
    long StatExperience[3];
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
    long GetStatExperience(EHeroExperienceStatCategory category) const { return StatExperience[category]; }
    void ForceSpendableExperienceTo(long amount) { ExperiencePointsAvailableToSpend = amount; }
    void ForceTrainableStatExperienceTo(EHeroExperienceStatCategory category, long amount) { StatExperience[category] = amount; }
};
struct SLongListNode {
    SLongListNode* _M_next;
    SLongListNode* _M_prev;
    long _M_data;
};
struct CLongListIterator {
    SLongListNode* _M_node;
    CLongListIterator(SLongListNode* node) : _M_node(node) { }
    long& operator*() const { return _M_node->_M_data; }
    CLongListIterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};
struct CLongList {
    SLongListNode* _M_node;
    CLongList() { _M_node = (SLongListNode*)std::malloc(sizeof(SLongListNode)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    CLongList(const CLongList& other)
    {
        _M_node = (SLongListNode*)std::malloc(sizeof(SLongListNode)); _M_node->_M_next = _M_node->_M_prev = _M_node;
        SLongListNode* n;
        for (n = other._M_node->_M_next; n != other._M_node; n = n->_M_next)
            push_back(n->_M_data);
    }
    ~CLongList() { while (_M_node->_M_next != _M_node) pop_back(); std::free(_M_node); }
    CLongListIterator end() { return CLongListIterator(_M_node); }
    long& back() { CLongListIterator tmp = end(); --tmp; return *tmp; }
    long size() const { long count = 0; SLongListNode* n; for (n = _M_node->_M_next; n != _M_node; n = n->_M_next) ++count; return count; }
    void push_back(const long& value)
    {
        SLongListNode* tmp = (SLongListNode*)std::malloc(sizeof(SLongListNode));
        tmp->_M_data = value; tmp->_M_next = _M_node; tmp->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = tmp; _M_node->_M_prev = tmp;
    }
    void pop_back()
    {
        SLongListNode* last = _M_node->_M_prev;
        last->_M_prev->_M_next = _M_node; _M_node->_M_prev = last->_M_prev;
        std::free(last);
    }
};
struct SExpSpendUndo {
    CLongList Generals;
    CLongList Strengths;
    CLongList Skills;
    CLongList Wills;
    long Levels;
    long General;
    long Strength;
    long Skill;
    long Will;
    long aGeneral;
    long aStrength;
    long aSkill;
    long aWill;
    SExpSpendUndo() : Levels(0), General(0), Strength(0), Skill(0), Will(0), aGeneral(0), aStrength(0), aSkill(0), aWill(0) { }
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
void SExpSpendUndo::SaveBefore(CTCHeroExperience_Calls* experience)
{
    aGeneral = experience->GetExperiencePointsAvailableToSpend();
    aStrength = experience->GetStatExperience(HERO_STAT_STRENGTH);
    aSkill = experience->GetStatExperience(HERO_STAT_SKILL);
    aWill = experience->GetStatExperience(HERO_STAT_WILL);
}
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
static void SetHero(CTCHeroExperience_Calls& hero, long spendable, long strength, long skill, long will)
{
    hero.ExperiencePointsAvailableToSpend = spendable;
    hero.StatExperience[0] = strength; hero.StatExperience[1] = skill; hero.StatExperience[2] = will;
}
int main()
{
    CTCHeroExperience_Calls hero; SetHero(hero, 500, 10, 20, 30);
    SExpSpendUndo undo;
    undo.Levels = 2;
    undo.SaveBefore(&hero);
    SetHero(hero, 380, 60, 70, 50);
    undo.SaveAfter(&hero);
    undo.SaveBefore(&hero);
    SetHero(hero, 300, 80, 70, 55);
    undo.SaveAfter(&hero);
    undo.RevertExperiencePoints(&hero);
    if (hero.ExperiencePointsAvailableToSpend != 380 || hero.StatExperience[0] != 60 || hero.StatExperience[1] != 70 || hero.StatExperience[2] != 50)
        { std::printf("0056f4ba_TEST FAIL\n"); return 1; }
    if (undo.Generals.size() != 1 || undo.Wills.size() != 1 || undo.General != 120 || undo.Will != -20 || undo.Levels != 1)
        { std::printf("0056f4ba_TEST FAIL\n"); return 1; }
    undo.RevertExperiencePoints(&hero);
    if (hero.ExperiencePointsAvailableToSpend != 500 || hero.StatExperience[0] != 10 || hero.StatExperience[2] != 30 || undo.Generals.size() != 0 ||
        undo.General != 0 || undo.Strength != 0 || undo.Levels != 0) { std::printf("0056f4ba_TEST FAIL\n"); return 1; }
    std::printf("0056f4ba_TEST PASS\n");
    return 0;
}