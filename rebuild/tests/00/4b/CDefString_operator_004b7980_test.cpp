// Behaviour test for 0x004b7980 CDefString::operator==(const CCharString&): the table string compared with the argument's
// characters (a null string reads as "").
#include <cstdio>
#include <cstring>
struct CCharStringData { const char* m_pChars; unsigned int m_length; };
struct CCharString {
    CCharStringData* m_pStringData;
    const char* GetChars() const { return m_pStringData == 0 ? "" : m_pStringData->m_pChars; }
};
static const char* g_table[3] = { "", "SWORD", "AXE" };
struct CDefString {
    int TablePos;
    bool operator==(const CCharString& text) const { return std::strcmp(g_table[TablePos], text.GetChars()) == 0; }
};
int main()
{
    CCharStringData sword = { "SWORD", 5 }, axe = { "AXE", 3 };
    CCharString s; s.m_pStringData = &sword;
    CDefString d; d.TablePos = 1;
    bool same = (std::strcmp("==", "==") == 0);
    if ((d == s) != same) { std::printf("004b7980_TEST FAIL\n"); return 1; }
    s.m_pStringData = &axe;
    if ((d == s) == same) { std::printf("004b7980_TEST FAIL\n"); return 1; }
    s.m_pStringData = 0; d.TablePos = 0;
    if ((d == s) != same) { std::printf("004b7980_TEST FAIL\n"); return 1; }
    std::printf("004b7980_TEST PASS\n");
    return 0;
}