// Behaviour test for the CTCWife::WifeAppearanceDialogueGroupStemNames dynamic initializer @ 0x0121E260: sixteen dialogue-group stems
// built from the retail literals (0x0125F608 .. 0x0125F37C, in table order), indexed by EWifeAppearanceCategories (0 .. 15); the
// reader 0x00985F40 appends the attitude suffix to the stem of the category.
// NF193 (q4155): the earlier test used invented stems (SLIMY_ / FILTHY_ ...); these are the strings the row and retail construct.
#include <cstdio>
#include <cstring>
#include <string>
static const char* const kRetailStems[16] = {
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_GENERAL_",   "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_HEAD_",
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BODY_",      "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_HANDS_",
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_LEGS_",      "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_FEET_",
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_HAIR_",      "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BEARD_",
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_MOUSTACHE_", "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_TATTOO_",
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_FATNESS_",   "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_THINNER_",
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_MORESCARS_", "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BALDNESS_",
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_NAKED_",     "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BARETORSO_" };
// The initializer: one string per element, constructed in place from its literal (CCharString(const char*, long = -1)).
static std::string g_StemNames[16];
static void DynamicInitializer() { for (int i = 0; i < 16; ++i) g_StemNames[i] = std::string(kRetailStems[i]); }
int main()
{
    DynamicInitializer();
    bool ok = sizeof(g_StemNames) / sizeof(g_StemNames[0]) == 16;
    if (g_StemNames[0] != "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_GENERAL_") ok = false;
    if (g_StemNames[15] != "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BARETORSO_") ok = false;
    for (int i = 0; i < 16; ++i)
        if (std::strncmp(g_StemNames[i].c_str(), "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_", 34) != 0
            || g_StemNames[i][g_StemNames[i].size() - 1] != '_') ok = false;
    if (!ok) { std::printf("0121e260_TEST FAIL\n"); return 1; }
    std::printf("0121e260_TEST PASS\n");
    return 0;
}
