#include <cstdio>
#include <cstring>

// Model of 0x00415260 GetFontBankName: switch on GetLanguage() (Windows primary language id) to the main text bank
// name; anything unlisted (English included) gets TEXT_ENGLISH_MAIN. Pairs checked against the retail case table.
class CCharString {
public:
    const char* Text;
    CCharString(const char* text, int length) : Text(text) { (void)length; }
};
static long g_language = 0;
static long GetLanguage() { return g_language; }
enum {
    LANG_CHINESE  = 0x04,
    LANG_GERMAN   = 0x07,
    LANG_ENGLISH  = 0x09,
    LANG_SPANISH  = 0x0A,
    LANG_FRENCH   = 0x0C,
    LANG_ITALIAN  = 0x10,
    LANG_JAPANESE = 0x11,
    LANG_KOREAN   = 0x12
};
static CCharString GetFontBankName()
{
    switch (GetLanguage()) {
    case LANG_FRENCH:
        return CCharString("TEXT_FRENCH_MAIN", -1);
    case LANG_JAPANESE:
        return CCharString("TEXT_JAPANESE_MAIN", -1);
    case LANG_GERMAN:
        return CCharString("TEXT_GERMAN_MAIN", -1);
    case LANG_KOREAN:
        return CCharString("TEXT_KOREAN_MAIN", -1);
    case LANG_CHINESE:
        return CCharString("TEXT_CHINESE_MAIN", -1);
    case LANG_SPANISH:
        return CCharString("TEXT_SPANISH_MAIN", -1);
    case LANG_ITALIAN:
        return CCharString("TEXT_ITALIAN_MAIN", -1);
    default:
        return CCharString("TEXT_ENGLISH_MAIN", -1);
    }
}

int main()
{
    const long ids[] = { LANG_FRENCH, LANG_ITALIAN, LANG_GERMAN, LANG_SPANISH, LANG_JAPANESE, LANG_KOREAN, LANG_CHINESE,
                         LANG_ENGLISH, 0x13, 0 };
    const char* names[] = { "TEXT_FRENCH_MAIN", "TEXT_ITALIAN_MAIN", "TEXT_GERMAN_MAIN", "TEXT_SPANISH_MAIN",
                            "TEXT_JAPANESE_MAIN", "TEXT_KOREAN_MAIN", "TEXT_CHINESE_MAIN", "TEXT_ENGLISH_MAIN",
                            "TEXT_ENGLISH_MAIN", "TEXT_ENGLISH_MAIN" };
    for (int i = 0; i < 10; ++i) {
        g_language = ids[i];
        if (std::strcmp(GetFontBankName().Text, names[i]) != 0) {
            std::printf("00415260_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00415260_TEST PASS\n");
    return 0;
}