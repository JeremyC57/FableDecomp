// Behaviour test for CAnimationSet::IsAppropriateEntryExisting<CIsCombatAnimAppropriateFollowingCombo>
// (0x00738F90): next-stage and finisher follow-ups of the current combo are found, others are not.
#include <cstdio>

class CAnimComponentCombatComboChain { public: long ComboStage, ComboID; };
class CAnimationEntry { public:
    CAnimComponentCombatComboChain combo;
    void GetComponent(const CAnimComponentCombatComboChain** out) const { *out = &combo; }
};
class CIsCombatAnimAppropriateFollowingCombo { public:
    const CAnimComponentCombatComboChain* CombatAnimCombo; long CurrentChainPos, MaxComboStage;
    bool operator()(const CAnimationEntry& entry) const
    {
        const CAnimComponentCombatComboChain* combo;
        entry.GetComponent(&combo);
        if (combo->ComboID == -999 || combo->ComboID != CombatAnimCombo->ComboID) return false;
        if (combo->ComboStage == CombatAnimCombo->ComboStage + 1) return true;
        if (CurrentChainPos <= MaxComboStage && combo->ComboStage == -888) return true;
        return false;
    }
};
struct CAnimationSetEntry { unsigned long first; CAnimationEntry second; };
template <class P> static bool IsAppropriateEntryExisting(const CAnimationSetEntry* first, const CAnimationSetEntry* last, const P& predicate)
{
    for (const CAnimationSetEntry* it = first; it != last; ++it)
        if (predicate(it->second)) return true;
    return false;
}

int main()
{
    CAnimComponentCombatComboChain current = { 2, 7 };
    CAnimationSetEntry e[3] = { { 0, { { 3, 8 } } }, { 1, { { 5, 7 } } }, { 2, { { -888, 7 } } } };
    CIsCombatAnimAppropriateFollowingCombo p = { &current, 1, 3 };
    if (!IsAppropriateEntryExisting(e, e + 3, p)) { std::printf("00738f90_TEST FAIL\n"); return 1; }      // finisher
    p.CurrentChainPos = 4;
    if (IsAppropriateEntryExisting(e, e + 3, p)) { std::printf("00738f90_TEST FAIL\n"); return 1; }       // chain full
    e[1].second.combo.ComboStage = 3;
    if (!IsAppropriateEntryExisting(e, e + 3, p)) { std::printf("00738f90_TEST FAIL\n"); return 1; }      // next stage
    current.ComboID = -999; e[1].second.combo.ComboID = -999; e[2].second.combo.ComboID = -999;
    if (IsAppropriateEntryExisting(e, e + 3, p)) { std::printf("00738f90_TEST FAIL\n"); return 1; }
    if (IsAppropriateEntryExisting(e, e, p)) { std::printf("00738f90_TEST FAIL\n"); return 1; }
    std::printf("00738f90_TEST PASS\n");
    return 0;
}