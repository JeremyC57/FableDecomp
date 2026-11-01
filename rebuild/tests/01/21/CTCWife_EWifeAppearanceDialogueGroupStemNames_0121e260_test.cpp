// Behaviour test for the CTCWife::WifeAppearanceDialogueGroupStemNames dynamic initializer @ 0x0121E260:
// sixteen CCharString stems built from literals, indexed by EWifeAppearanceCategories.
#include <cstdio>
#include <cstring>
#include <string>
namespace model {
enum EWifeAppearanceCategories { CLOTHING_SLIMY, CLOTHING_FILTHY, CLOTHING_MOSTLY_FILTHY, CLOTHING_DIRTY, CLOTHING_GRUBBY,
    CLOTHING_CLEAN, CLOTHING_SMART, CLOTHING_FANCY, CLOTHING_GLAMOROUS, CLOTHING_DISGUSTING, CLOTHING_NASTY, CLOTHING_BAD,
    CLOTHING_OK, CLOTHING_GOOD, CLOTHING_GREAT, CLOTHING_PERFECT, NUM_WIFE_APPEARANCE_CATEGORIES };
struct CCharString { std::string s; CCharString(const char* p, long length = -1) : s(length < 0 ? std::string(p) : std::string(p, length)) {} };
struct CTCWife { static CCharString WifeAppearanceDialogueGroupStemNames[NUM_WIFE_APPEARANCE_CATEGORIES]; };
CCharString CTCWife::WifeAppearanceDialogueGroupStemNames[NUM_WIFE_APPEARANCE_CATEGORIES] = {
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_SLIMY_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_FILTHY_"),
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_MOSTLY_FILTHY_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_DIRTY_"),
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_GRUBBY_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_CLEAN_"),
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_SMART_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_FANCY_"),
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_GLAMOROUS_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_DISGUSTING_"),
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_NASTY_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BAD_"),
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_OK_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_GOOD_"),
    CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_GREAT_"), CCharString("TEXT_AI_WIFE_OPINION_ACT_CLOTHING_PERFECT_") };
}
int main()
{
    using namespace model;
    bool ok = sizeof(CTCWife::WifeAppearanceDialogueGroupStemNames) / sizeof(CCharString) == 16;
    if (CTCWife::WifeAppearanceDialogueGroupStemNames[CLOTHING_SLIMY].s != "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_SLIMY_") ok = false;
    if (CTCWife::WifeAppearanceDialogueGroupStemNames[CLOTHING_PERFECT].s != "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_PERFECT_") ok = false;
    for (int i = 0; i < NUM_WIFE_APPEARANCE_CATEGORIES; ++i)
        if (std::strncmp(CTCWife::WifeAppearanceDialogueGroupStemNames[i].s.c_str(), "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_", 34) != 0) ok = false;
    if (!ok) { std::printf("0121e260_TEST FAIL\n"); return 1; }
    std::printf("0121e260_TEST PASS\n");
    return 0;
}