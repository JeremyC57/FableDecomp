// Behaviour test for 0x00415070 NLocalisation::GetLanguage: probes the TEXT_<LANG>_MAIN banks in retail order and caches the hit.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* text, long length = -1) : m_text(text) { ++s_live; }
    ~CCharString() { --s_live; }
    const char* m_text;
    static int s_live;
};
int CCharString::s_live = 0;

class CBankFileManager {
public:
    const char* m_present;      // the one bank the test installs
    int m_queries;
    bool BankExists(const CCharString& bank) const
    {
        ++const_cast<CBankFileManager*>(this)->m_queries;
        return m_present != 0 && std::strcmp(bank.m_text, m_present) == 0;
    }
};

CBankFileManager g_BankFileManager;
unsigned long g_Language;

static bool LanguageBankExists(const char* bank)
{
    CCharString name(bank);
    return g_BankFileManager.BankExists(name);
}

unsigned long GetLanguage()
{
    if (g_Language != 0)
        return g_Language;
    if (LanguageBankExists("TEXT_ENGLISH_MAIN"))
        return g_Language = 9;
    if (LanguageBankExists("TEXT_FRENCH_MAIN"))
        return g_Language = 0x0C;
    if (LanguageBankExists("TEXT_JAPANESE_MAIN"))
        return g_Language = 0x11;
    if (LanguageBankExists("TEXT_GERMAN_MAIN"))
        return g_Language = 7;
    if (LanguageBankExists("TEXT_KOREAN_MAIN"))
        return g_Language = 0x12;
    if (LanguageBankExists("TEXT_CHINESE_MAIN"))
        return g_Language = 4;
    if (LanguageBankExists("TEXT_SPANISH_MAIN"))
        return g_Language = 0x0A;
    if (LanguageBankExists("TEXT_ITALIAN_MAIN"))
        return g_Language = 0x10;
    return g_Language;
}

static int Check(const char* bank, unsigned long expect, int queries)
{
    g_Language = 0;
    g_BankFileManager.m_present = bank;
    g_BankFileManager.m_queries = 0;
    if (GetLanguage() != expect || g_Language != expect || g_BankFileManager.m_queries != queries) return 0;
    if (CCharString::s_live != 0) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    ok &= Check("TEXT_ENGLISH_MAIN", 9, 1);
    ok &= Check("TEXT_GERMAN_MAIN", 7, 4);
    ok &= Check("TEXT_ITALIAN_MAIN", 0x10, 8);
    ok &= Check("TEXT_KOREAN_MAIN", 0x12, 5);
    ok &= Check(0, 0, 8);
    // cached: no further probes once detected
    g_Language = 0x0C; g_BankFileManager.m_queries = 0;
    if (GetLanguage() != 0x0C || g_BankFileManager.m_queries != 0) ok = 0;
    if (!ok) { std::printf("00415070_TEST FAIL\n"); return 1; }
    std::printf("00415070_TEST PASS\n");
    return 0;
}