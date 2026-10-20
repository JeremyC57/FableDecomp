// Behaviour test for CTCInventoryExperience::GetExperienceAbilityMovieName 0x0056E75F (model copy, narrow strings)
#include <cstdio>
#include <cstring>

struct CWideString {
    const char* m_text;
    CWideString(const char* text) : m_text(text) {}
};

struct CTCInventoryExperience {
    CWideString GetExperienceAbilityMovieName(long ability);
};

CWideString CTCInventoryExperience::GetExperienceAbilityMovieName(long ability)
{
    if (ability == 13) return CWideString("Data\\Video\\gui_spell_Assassin_Rush.wmv");
    if (ability == 12) return CWideString("Data\\Video\\gui_spell_Battle_Charge.wmv");
    if (ability == 8) return CWideString("Data\\Video\\gui_spell_Beserk.wmv");
    if (ability == 6) return CWideString("Data\\Video\\gui_spell_drain.wmv");
    if (ability == 3) return CWideString("Data\\Video\\gui_spell_enflame.wmv");
    if (ability == 16) return CWideString("Data\\Video\\gui_spell_fireball.wmv");
    if (ability == 1) return CWideString("Data\\Video\\gui_spell_force_push.wmv");
    if (ability == 15) return CWideString("Data\\Video\\gui_spell_ghostly_sword.wmv");
    if (ability == 14) return CWideString("Data\\Video\\gui_spell_heal.wmv");
    if (ability == 11) return CWideString("Data\\Video\\gui_spell_lightning.wmv");
    if (ability == 17) return CWideString("Data\\Video\\gui_spell_Multi_Arrow.wmv");
    if (ability == 9) return CWideString("Data\\Video\\gui_spell_MultiStrike.wmv");
    if (ability == 4) return CWideString("Data\\Video\\gui_spell_Shield.wmv");
    if (ability == 2) return CWideString("Data\\Video\\gui_spell_slow_time.wmv");
    if (ability == 10) return CWideString("Data\\Video\\gui_Spell_Summon.wmv");
    if (ability == 5) return CWideString("Data\\Video\\gui_spell_TurnCoat.wmv");
    if (ability == 18) return CWideString("Data\\Video\\gui_spell_divine_wrath.wmv");
    if (ability == 19) return CWideString("Data\\Video\\gui_spell_unholy_power.wmv");
    return CWideString("Data\\Video\\lionhead_logo.wmv");
}

int main()
{
    CTCInventoryExperience e;
    if (std::strcmp(e.GetExperienceAbilityMovieName(13).m_text, "Data\\Video\\gui_spell_Assassin_Rush.wmv") != 0 ||
        std::strcmp(e.GetExperienceAbilityMovieName(16).m_text, "Data\\Video\\gui_spell_fireball.wmv") != 0 ||
        std::strcmp(e.GetExperienceAbilityMovieName(19).m_text, "Data\\Video\\gui_spell_unholy_power.wmv") != 0 ||
        std::strcmp(e.GetExperienceAbilityMovieName(0).m_text, "Data\\Video\\lionhead_logo.wmv") != 0 ||
        std::strcmp(e.GetExperienceAbilityMovieName(7).m_text, "Data\\Video\\lionhead_logo.wmv") != 0) {
        std::printf("0056e75f_TEST FAIL\n"); return 1;
    }
    std::printf("0056e75f_TEST PASS\n");
    return 0;
}