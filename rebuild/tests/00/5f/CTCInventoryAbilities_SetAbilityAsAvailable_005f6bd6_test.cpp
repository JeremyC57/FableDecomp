// Behaviour test for CTCInventoryAbilities::SetAbilityAsAvailable (0x005F6BD6): a copy of the logic over a small model.
#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>

template <class T> const T& Max(const T& a, const T& b) { return a < b ? b : a; }
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
struct Model {
    bool WaitingToGiveTutorial, Initialised;
    long CurrentAssignedAbilityLevel;
    std::vector<EHeroAbility> ReceivedAbilityOrder;
    std::vector<std::pair<long, int> > Abilities;
    std::vector<std::pair<long, EHeroAbility> > AbilityButtons;   // sorted by button
    long numButtons;
    int cycles;
    long GetNumAssignableSpecialMoveButtons() const { return numButtons; }
    std::pair<long, EHeroAbility>* FindButton(long b) {
        for (size_t k = 0; k < AbilityButtons.size(); ++k) if (AbilityButtons[k].first == b) return &AbilityButtons[k];
        return 0;
    }
    void SetAbilityToButton(EHeroAbility a, long b, bool) { AbilityButtons.push_back(std::make_pair(b, a)); }
    void CycleAssignedAbilities(bool) { ++cycles; }
    void SetAbilityAsAvailable(EHeroAbility ability, bool available, bool reassign)
    {
        bool was_available = Abilities[ability].first >= 0;
        if (available) Abilities[ability].first = Max(0L, Abilities[ability].first);
        else Abilities[ability].first = -1;
        if (Initialised) {
            if (available && !was_available) {
                bool added = false;
                for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i)
                    if (ReceivedAbilityOrder[i] == HERO_ABILITY_NULL) { ReceivedAbilityOrder[i] = ability; added = true; break; }
                if (!added) {
                    for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
                        for (unsigned long j = i + 1; j < ReceivedAbilityOrder.size(); ++j)
                            if (ReceivedAbilityOrder[j] == ReceivedAbilityOrder[i]) { ReceivedAbilityOrder[j] = ability; added = true; break; }
                        if (added) break;
                    }
                }
                for (long button = 0; button < numButtons; ++button)
                    if (!FindButton(button)) { SetAbilityToButton(ability, button, true); break; }
                long count = 0;
                for (unsigned long k = 0; k < ReceivedAbilityOrder.size(); ++k) if (ReceivedAbilityOrder[k] != HERO_ABILITY_NULL) ++count;
                if (count > GetNumAssignableSpecialMoveButtons()) WaitingToGiveTutorial = true;
            } else if (was_available && !available) {
                for (size_t k = 0; k < AbilityButtons.size(); ++k)
                    if (AbilityButtons[k].second == ability) { AbilityButtons.erase(AbilityButtons.begin() + k); break; }
                std::vector<EHeroAbility>::iterator r = std::find(ReceivedAbilityOrder.begin(), ReceivedAbilityOrder.end(), ability);
                if (r != ReceivedAbilityOrder.end()) ReceivedAbilityOrder.erase(r);
            }
        }
        if (reassign) {
            CurrentAssignedAbilityLevel = Max(CurrentAssignedAbilityLevel - GetNumAssignableSpecialMoveButtons(), -GetNumAssignableSpecialMoveButtons());
            CycleAssignedAbilities(true);
        }
    }
};

int main()
{
    Model m;
    m.WaitingToGiveTutorial = false; m.Initialised = true; m.CurrentAssignedAbilityLevel = 0; m.numButtons = 1; m.cycles = 0;
    for (int k = 0; k < 4; ++k) m.Abilities.push_back(std::make_pair(-1L, 0));
    m.ReceivedAbilityOrder.push_back(HERO_ABILITY_NULL);
    m.SetAbilityAsAvailable((EHeroAbility)2, true, true);
    bool ok = m.Abilities[2].first == 0 && m.ReceivedAbilityOrder[0] == 2 && m.AbilityButtons.size() == 1
           && m.AbilityButtons[0].second == 2 && !m.WaitingToGiveTutorial && m.CurrentAssignedAbilityLevel == -1 && m.cycles == 1;
    m.ReceivedAbilityOrder.push_back((EHeroAbility)2);   // a duplicate slot gets overwritten
    m.SetAbilityAsAvailable((EHeroAbility)3, true, false);
    ok = ok && m.ReceivedAbilityOrder[1] == 3 && m.AbilityButtons.size() == 1 && m.WaitingToGiveTutorial && m.cycles == 1;
    m.SetAbilityAsAvailable((EHeroAbility)2, false, false);
    ok = ok && m.Abilities[2].first == -1 && m.AbilityButtons.empty() && m.ReceivedAbilityOrder.size() == 1 && m.ReceivedAbilityOrder[0] == 3;
    m.Abilities[1].first = 5;
    m.SetAbilityAsAvailable((EHeroAbility)1, true, false);   // already available: level kept, lists untouched
    ok = ok && m.Abilities[1].first == 5 && m.ReceivedAbilityOrder.size() == 1;
    if (!ok) { std::printf("005f6bd6_TEST FAIL\n"); return 1; }
    std::printf("005f6bd6_TEST PASS\n");
    return 0;
}