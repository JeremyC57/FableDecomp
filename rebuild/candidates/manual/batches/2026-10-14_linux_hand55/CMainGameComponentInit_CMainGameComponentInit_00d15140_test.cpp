#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_strings = 0; struct CCharString { void* m_pStringData; CCharString() : m_pStringData(0) { ++g_strings; } };
struct CMainGameComponentInit { CCharString m_name; long m_4; long m_8; CCharString m_c; CCharString m_10; CCharString m_14; CMainGameComponentInit() { } };

int main()
{
    CMainGameComponentInit i; if (g_strings != 4 || i.m_name.m_pStringData || i.m_c.m_pStringData || i.m_10.m_pStringData || i.m_14.m_pStringData) { std::printf("00d15140_TEST FAIL\n"); return 1; }
    std::printf("00d15140_TEST PASS\n");
    return 0;
}
