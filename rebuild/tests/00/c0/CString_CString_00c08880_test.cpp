// behaviour test for 00c08880 (standalone copy of the model)
#include <cstdio>

static char g_nil[16]; static char* g_pchNil = g_nil;
class CString { public: CString() { m_pchData = g_pchNil; } char* m_pchData; };

int main()
{
    CString s;
    if (!(s.m_pchData == g_nil)) { std::printf("00c08880_TEST FAIL\n"); return 1; }
    std::printf("00c08880_TEST PASS\n");
    return 0;
}