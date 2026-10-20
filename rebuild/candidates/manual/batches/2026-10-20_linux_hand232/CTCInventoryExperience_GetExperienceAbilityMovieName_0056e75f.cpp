// CTCInventoryExperience::GetExperienceAbilityMovieName(EHeroAbility ability) -- the spell-spending movie path for an ability
// Retail 0x0056E75F, 263 bytes, ret 8 (hidden CWideString result + ability). An if-chain over the EHeroAbility
// ids in retail's test order (13, 12, 8, 6, 3, 16, 1, 15, 14, 11, 17, 9, 4, 2, 10, 5, 18, 19); every hit pushes
// its wide literal and jumps to the one shared `mov ecx,[ebp+8] ; call CWideString(const wchar_t*)` tail
// (0x0099B6B0), the fall-through pushes the Lionhead logo movie. Each `return CWideString(L"...")` constructs
// straight into the hidden result, and VC7.1 tail-merges the identical constructor calls.
// Sibling of the landed GetExperienceStatMovieName 0x0056E6B6 (CCharString form). Frame-pointer body:
// `#pragma optimize("y", off)` + `("s", on)` like the landed siblings.
#pragma optimize("y", off)
#pragma optimize("s", on)

typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t

struct CWideString {
    void* m_pStringData;
    CWideString(const wchar_t* text);              // 0x0099B6B0
};

enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };

struct CTCInventoryExperience {
    CWideString GetExperienceAbilityMovieName(EHeroAbility ability);
};

CWideString CTCInventoryExperience::GetExperienceAbilityMovieName(EHeroAbility ability)
{
    if (ability == 13) return CWideString(L"Data\\Video\\gui_spell_Assassin_Rush.wmv");
    if (ability == 12) return CWideString(L"Data\\Video\\gui_spell_Battle_Charge.wmv");
    if (ability == 8) return CWideString(L"Data\\Video\\gui_spell_Beserk.wmv");
    if (ability == 6) return CWideString(L"Data\\Video\\gui_spell_drain.wmv");
    if (ability == 3) return CWideString(L"Data\\Video\\gui_spell_enflame.wmv");
    if (ability == 16) return CWideString(L"Data\\Video\\gui_spell_fireball.wmv");
    if (ability == 1) return CWideString(L"Data\\Video\\gui_spell_force_push.wmv");
    if (ability == 15) return CWideString(L"Data\\Video\\gui_spell_ghostly_sword.wmv");
    if (ability == 14) return CWideString(L"Data\\Video\\gui_spell_heal.wmv");
    if (ability == 11) return CWideString(L"Data\\Video\\gui_spell_lightning.wmv");
    if (ability == 17) return CWideString(L"Data\\Video\\gui_spell_Multi_Arrow.wmv");
    if (ability == 9) return CWideString(L"Data\\Video\\gui_spell_MultiStrike.wmv");
    if (ability == 4) return CWideString(L"Data\\Video\\gui_spell_Shield.wmv");
    if (ability == 2) return CWideString(L"Data\\Video\\gui_spell_slow_time.wmv");
    if (ability == 10) return CWideString(L"Data\\Video\\gui_Spell_Summon.wmv");
    if (ability == 5) return CWideString(L"Data\\Video\\gui_spell_TurnCoat.wmv");
    if (ability == 18) return CWideString(L"Data\\Video\\gui_spell_divine_wrath.wmv");
    if (ability == 19) return CWideString(L"Data\\Video\\gui_spell_unholy_power.wmv");
    return CWideString(L"Data\\Video\\lionhead_logo.wmv");
}
