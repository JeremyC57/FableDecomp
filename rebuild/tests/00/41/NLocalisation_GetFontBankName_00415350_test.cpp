#include <cstdio>
#include <cstring>
// Behaviour test for NLocalisation::GetFontBankName (retail 0x00415350): the language id from GetLanguage selects
// the bank name; ids that are not one of the seven localised languages fall back to English.
struct CCharString
{
    const char* m_text;
    CCharString(const char* text, long) : m_text(text) { }
};
static unsigned long g_language = 9;
static unsigned long GetLanguage() { return g_language; }
static CCharString GetFontBankName()
{
    switch (GetLanguage())
    {
    case 12: return CCharString("FONT_FRENCH_MAIN", -1);
    case 17: return CCharString("FONT_JAPANESE_MAIN", -1);
    case 7:  return CCharString("FONT_GERMAN_MAIN", -1);
    case 18: return CCharString("FONT_KOREAN_MAIN", -1);
    case 4:  return CCharString("FONT_CHINESE_MAIN", -1);
    case 10: return CCharString("FONT_SPANISH_MAIN", -1);
    case 16: return CCharString("FONT_ITALIAN_MAIN", -1);
    default: return CCharString("FONT_ENGLISH_MAIN", -1);
    }
}
static int Check(unsigned long language, const char* expected)
{
    g_language = language;
    CCharString s = GetFontBankName();
    return std::strcmp(s.m_text, expected) == 0;
}
int main()
{
    int ok = Check(9, "FONT_ENGLISH_MAIN") && Check(12, "FONT_FRENCH_MAIN") && Check(17, "FONT_JAPANESE_MAIN")
          && Check(7, "FONT_GERMAN_MAIN") && Check(18, "FONT_KOREAN_MAIN") && Check(4, "FONT_CHINESE_MAIN")
          && Check(10, "FONT_SPANISH_MAIN") && Check(16, "FONT_ITALIAN_MAIN") && Check(0, "FONT_ENGLISH_MAIN")
          && Check(5, "FONT_ENGLISH_MAIN") && Check(19, "FONT_ENGLISH_MAIN") && Check(13, "FONT_ENGLISH_MAIN");
    if (!ok) { std::printf("00415350_TEST FAIL\n"); return 1; }
    std::printf("00415350_TEST PASS\n");
    return 0;
}