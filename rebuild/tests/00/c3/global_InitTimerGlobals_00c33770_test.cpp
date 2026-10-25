// Behaviour test for 00c33770: InitTimerGlobals -- count 1, both links at the anchor
#include <cstdio>
struct CAnchor { long m_value; };
static CAnchor g_anchor;
struct CStaticRecord { long m_count; CAnchor* m_pFirst; CAnchor* m_pLast; };
static CStaticRecord g_record = { 7, 0, 0 };
static void InitTimerGlobals()
{
    g_record.m_count = 1;
    g_record.m_pFirst = &g_anchor;
    g_record.m_pLast = &g_anchor;
}
int main()
{
    InitTimerGlobals();
    if (g_record.m_count != 1 || g_record.m_pFirst != &g_anchor || g_record.m_pLast != &g_anchor) { std::printf("00c33770_TEST FAIL\n"); return 1; }
    std::printf("00c33770_TEST PASS\n");
    return 0;
}