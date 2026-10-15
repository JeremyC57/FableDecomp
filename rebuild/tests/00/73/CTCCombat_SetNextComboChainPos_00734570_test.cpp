#include <cstdio>
struct CComboDef { long Stage; long ChainID; };
struct CDefClassBase {
    const CComboDef* m_combo; bool m_hasFollowing;
    void GetComboDef(const CComboDef** out) { *out = m_combo; }
};
struct CMeleeCombatAbilityDef { CDefClassBase* DefClass; };
struct CTCCombat {
    long ComboChainPos; long ComboChainID;
    bool HasAppropriateFollowingCombo(CDefClassBase* defClass) { return defClass->m_hasFollowing; }
    void SetNextComboChainPos(const CMeleeCombatAbilityDef* ability)
    {
        CDefClassBase* defClass = ability->DefClass;
        const CComboDef* combo;
        defClass->GetComboDef(&combo);
        if (combo->Stage != -777) {
            if (ComboChainPos == -777)
                ComboChainPos = 1;
            else
                ComboChainPos = ComboChainPos + 1;
            ComboChainID = combo->ChainID;
            if (HasAppropriateFollowingCombo(defClass))
                return;
        }
        ComboChainPos = -777;
        ComboChainID = -999;
    }
};
int main()
{
    CComboDef combo = { 2, 42 }, none = { -777, 5 };
    CDefClassBase dc = { &combo, true };
    CMeleeCombatAbilityDef ab = { &dc };
    CTCCombat t; t.ComboChainPos = -777; t.ComboChainID = -999;
    t.SetNextComboChainPos(&ab);
    if (t.ComboChainPos != 1 || t.ComboChainID != 42) { std::printf("00734570_TEST FAIL\n"); return 1; }
    t.SetNextComboChainPos(&ab);
    if (t.ComboChainPos != 2 || t.ComboChainID != 42) { std::printf("00734570_TEST FAIL\n"); return 1; }
    dc.m_hasFollowing = false;
    t.SetNextComboChainPos(&ab);
    if (t.ComboChainPos != -777 || t.ComboChainID != -999) { std::printf("00734570_TEST FAIL\n"); return 1; }
    dc.m_hasFollowing = true; dc.m_combo = &none; t.ComboChainPos = 3; t.ComboChainID = 1;
    t.SetNextComboChainPos(&ab);
    if (t.ComboChainPos != -777 || t.ComboChainID != -999) { std::printf("00734570_TEST FAIL\n"); return 1; }
    std::printf("00734570_TEST PASS\n");
    return 0;
}