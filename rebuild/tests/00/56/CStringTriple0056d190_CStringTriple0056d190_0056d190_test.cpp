// Behaviour test for 0x0056D190: the record's alt text, text, then name are destroyed.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
class CWideString { public: int id; ~CWideString() { g_log[g_n++] = 10 + id; } };
struct CStringTriple { unsigned char _pad00[0x10]; CCharString m_name; CWideString m_text; CWideString m_altText; ~CStringTriple() {} };
int main()
{
    { CStringTriple t; t.m_name.id = 1; t.m_text.id = 2; t.m_altText.id = 3; }
    if (g_n != 3 || g_log[0] != 13 || g_log[1] != 12 || g_log[2] != 1) { std::printf("0056d190_TEST FAIL\n"); return 1; }
    std::printf("0056d190_TEST PASS\n");
    return 0;
}