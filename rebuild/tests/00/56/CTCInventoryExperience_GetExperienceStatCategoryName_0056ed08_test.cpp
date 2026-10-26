// Behaviour test for 0x0056ED08 CTCInventoryExperience::GetExperienceStatCategoryName
#include <cstdio>
#include <cstring>
class CWideString { public:
    char buf[64];
    CWideString(const char* t) { std::strcpy(buf, t); }
};
struct CDataBank {
    CWideString GetTextBySymbol(const char* s)
    {
        if (std::strcmp(s, "TEXT_GUI_MENU_SKILL") == 0) return CWideString("Skill");
        if (std::strcmp(s, "TEXT_GUI_MENU_STRENGTH") == 0) return CWideString("Strength");
        if (std::strcmp(s, "TEXT_GUI_MENU_WILL") == 0) return CWideString("Will");
        return CWideString("?");
    }
};
static CDataBank g_bank;
static CWideString GetExperienceStatCategoryName(long category)
{
    CDataBank* textBank = &g_bank;
    if (category == 1) return textBank->GetTextBySymbol("TEXT_GUI_MENU_SKILL");
    if (category == 0) return textBank->GetTextBySymbol("TEXT_GUI_MENU_STRENGTH");
    if (category == 2) return textBank->GetTextBySymbol("TEXT_GUI_MENU_WILL");
    return CWideString("");
}
int main()
{
    bool ok = std::strcmp(GetExperienceStatCategoryName(0).buf, "Strength") == 0
           && std::strcmp(GetExperienceStatCategoryName(1).buf, "Skill") == 0
           && std::strcmp(GetExperienceStatCategoryName(2).buf, "Will") == 0
           && GetExperienceStatCategoryName(3).buf[0] == 0;
    if (!ok) { std::printf("0056ed08_TEST FAIL\n"); return 1; }
    std::printf("0056ed08_TEST PASS\n");
    return 0;
}