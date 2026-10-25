#include <cstdio>
#include <cctype>

// Host model of CDefString::operator==(const CCharString&): the def-table string at TablePos compared with the CCharString's
// text by _stricmp (case-insensitive); an unassigned CCharString reads as "".
static int stricmp_(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int ca = std::tolower((unsigned char)*a), cb = std::tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
}
struct CCharStringData { const char* m_pChars; unsigned int m_length; };
struct CCharString {
    CCharStringData* m_pStringData;
    const char* GetChars() const { return m_pStringData == 0 ? "" : m_pStringData->m_pChars; }
};
struct CDefStringTable {
    const char* Strings[4];
    const char* GetStringAsCharArray(long id) { return Strings[id]; }
};
static CDefStringTable g_DefStringTable = { { "", "EXPRESSION_LAUGH", "Expression_Point", "WAVE" } };
struct CDefString {
    long TablePos;
    bool operator==(const CCharString& text) const { return stricmp_(g_DefStringTable.GetStringAsCharArray(TablePos), text.GetChars()) == 0; }
};

int main()
{
    CCharStringData laugh = { "expression_laugh", 16 };
    CCharStringData point = { "EXPRESSION_POINT", 16 };
    CCharString s; s.m_pStringData = &laugh;
    CDefString d1 = { 1 }, d2 = { 2 }, d0 = { 0 };
    if (!(d1 == s)) { std::printf("006aeb10_TEST FAIL\n"); return 1; }        // case is ignored
    if (d2 == s) { std::printf("006aeb10_TEST FAIL\n"); return 1; }
    s.m_pStringData = &point;
    if (!(d2 == s) || d1 == s) { std::printf("006aeb10_TEST FAIL\n"); return 1; }
    s.m_pStringData = 0;                                                      // unassigned == ""
    if (!(d0 == s) || d1 == s) { std::printf("006aeb10_TEST FAIL\n"); return 1; }
    std::printf("006aeb10_TEST PASS\n");
    return 0;
}