#include <cstdio>
#include <cstring>
// Behaviour test for NLocalisation::GetStreamingFontBankName (retail 0x00415440): the language id from GetLanguage selects
// the bank name; ids that are not one of the seven localised languages fall back to English.
struct CCharString
{
    const char* m_text;
    CCharString(const char* text, long) : m_text(text) { }
};
static unsigned long g_language = 9;
static unsigned long GetLanguage() { return g_language; }
static CCharString GetStreamingFontBankName()
{
    switch (GetLanguage())
    {
    case 12: return CCharString("STREAMING_FONT_FRENCH_PC", -1);
    case 17: return CCharString("STREAMING_FONT_JAPANESE_PC", -1);
    case 7:  return CCharString("STREAMING_FONT_GERMAN_PC", -1);
    case 18: return CCharString("STREAMING_FONT_KOREAN_PC", -1);
    case 4:  return CCharString("STREAMING_FONT_CHINESE_PC", -1);
    case 10: return CCharString("STREAMING_FONT_SPANISH_PC", -1);
    case 16: return CCharString("STREAMING_FONT_ITALIAN_PC", -1);
    default: return CCharString("STREAMING_FONT_ENGLISH_PC", -1);
    }
}
static int Check(unsigned long language, const char* expected)
{
    g_language = language;
    CCharString s = GetStreamingFontBankName();
    return std::strcmp(s.m_text, expected) == 0;
}
int main()
{
    int ok = Check(9, "STREAMING_FONT_ENGLISH_PC") && Check(12, "STREAMING_FONT_FRENCH_PC") && Check(17, "STREAMING_FONT_JAPANESE_PC")
          && Check(7, "STREAMING_FONT_GERMAN_PC") && Check(18, "STREAMING_FONT_KOREAN_PC") && Check(4, "STREAMING_FONT_CHINESE_PC")
          && Check(10, "STREAMING_FONT_SPANISH_PC") && Check(16, "STREAMING_FONT_ITALIAN_PC") && Check(0, "STREAMING_FONT_ENGLISH_PC")
          && Check(5, "STREAMING_FONT_ENGLISH_PC") && Check(19, "STREAMING_FONT_ENGLISH_PC") && Check(13, "STREAMING_FONT_ENGLISH_PC");
    if (!ok) { std::printf("00415440_TEST FAIL\n"); return 1; }
    std::printf("00415440_TEST PASS\n");
    return 0;
}