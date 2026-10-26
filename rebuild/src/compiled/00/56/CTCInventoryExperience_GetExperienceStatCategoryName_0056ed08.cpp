// CTCInventoryExperience::GetExperienceStatCategoryName(long category) -- the localised name of an experience stat category (0 strength, 1 skill, 2 will), else ""
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056ED08, length 108 bytes (`ret 8`: hidden CWideString result + category; `this` unused).
// Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CDataBank::GetTextBySymbol
// @ 0x009C95E0 (engine globals @ 0x013B86A0 +0x14), CWideString(const wchar_t*) @ 0x0099B6B0 on L"" @ 0x0122D70C.
// Strings: "TEXT_GUI_MENU_SKILL" @ 0x0124E670, "TEXT_GUI_MENU_STRENGTH" @ 0x0124E658, "TEXT_GUI_MENU_WILL" @ 0x0124E644.
// Shape: the text bank is a named local read first; each arm returns GetTextBySymbol of an implicit CCharString
// conversion temporary, homed in the dead `category` slot ([ebp+0Ch]); the three arms tail-merge into one call.
#pragma optimize("y", off)
#pragma optimize("s", on)
typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
};
class CWideString { public:
    void* m_pStringData;
    CWideString(const wchar_t* text);                             // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                               // 0x0099B510
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);       // 0x009C95E0
};
struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                                    // +0x14
};
extern CEngineGlobals* g_pEngineGlobals;                          // 0x013B86A0

enum EExperienceStatCategory { STAT_CATEGORY_STRENGTH = 0, STAT_CATEGORY_SKILL = 1, STAT_CATEGORY_WILL = 2 };
class CTCInventoryExperience { public:
    CWideString GetExperienceStatCategoryName(long category);
};
CWideString CTCInventoryExperience::GetExperienceStatCategoryName(long category)
{
    CDataBank* textBank = g_pEngineGlobals->m_pTextBank;
    if (category == STAT_CATEGORY_SKILL)
        return textBank->GetTextBySymbol("TEXT_GUI_MENU_SKILL");
    if (category == STAT_CATEGORY_STRENGTH)
        return textBank->GetTextBySymbol("TEXT_GUI_MENU_STRENGTH");
    if (category == STAT_CATEGORY_WILL)
        return textBank->GetTextBySymbol("TEXT_GUI_MENU_WILL");
    return CWideString(L"");
}