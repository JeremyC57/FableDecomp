#include <cstdio>
#include <cstring>
// Behaviour test for NLocalisation::GetSoundDefName (retail 0x00415550): the language id from GetLanguage selects
// the bank name; ids that are not one of the seven localised languages fall back to English.
class CCharString
{ public:
    const char* m_text;
    CCharString(const char* text, long) : m_text(text) { }
};
static unsigned long g_language = 9;
static unsigned long GetLanguage() { return g_language; }
static CCharString GetSoundDefName()
{
    switch (GetLanguage())
    {
    case 12: return CCharString("FRENCH_SOUND_SETUP", -1);
    case 17: return CCharString("JAPANESE_SOUND_SETUP", -1);
    case 7:  return CCharString("GERMAN_SOUND_SETUP", -1);
    case 18: return CCharString("KOREAN_SOUND_SETUP", -1);
    case 4:  return CCharString("CHINESE_SOUND_SETUP", -1);
    case 10: return CCharString("SPANISH_SOUND_SETUP", -1);
    case 16: return CCharString("ITALIAN_SOUND_SETUP", -1);
    default: return CCharString("ENGLISH_SOUND_SETUP", -1);
    }
}
static int Check(unsigned long language, const char* expected)
{
    g_language = language;
    CCharString s = GetSoundDefName();
    return std::strcmp(s.m_text, expected) == 0;
}
int main()
{
    int ok = Check(9, "ENGLISH_SOUND_SETUP") && Check(12, "FRENCH_SOUND_SETUP") && Check(17, "JAPANESE_SOUND_SETUP")
          && Check(7, "GERMAN_SOUND_SETUP") && Check(18, "KOREAN_SOUND_SETUP") && Check(4, "CHINESE_SOUND_SETUP")
          && Check(10, "SPANISH_SOUND_SETUP") && Check(16, "ITALIAN_SOUND_SETUP") && Check(0, "ENGLISH_SOUND_SETUP")
          && Check(5, "ENGLISH_SOUND_SETUP") && Check(19, "ENGLISH_SOUND_SETUP") && Check(13, "ENGLISH_SOUND_SETUP");
    if (!ok) { std::printf("00415550_TEST FAIL\n"); return 1; }
    std::printf("00415550_TEST PASS\n");
    return 0;
}