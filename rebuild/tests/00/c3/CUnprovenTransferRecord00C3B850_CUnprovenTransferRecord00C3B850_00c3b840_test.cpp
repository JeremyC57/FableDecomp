// Behaviour test for the libpk transfer-record destructor 0x00C3B840: destroying the record destroys its CString member (at +4) exactly once.
#include <cstdio>
static int g_strdtor = 0;
struct CString { char* m_pchData; CString() : m_pchData(0) {} ~CString() { ++g_strdtor; } };
struct CRecord { unsigned char head[4]; CString Text; unsigned char tail[16]; };
int main()
{
    { CRecord r; (void)r; }
    if (g_strdtor != 1) { std::printf("00c3b840_TEST FAIL\n"); return 1; }
    CRecord* p = new CRecord; delete p;
    if (g_strdtor != 2) { std::printf("00c3b840_TEST FAIL\n"); return 1; }
    std::printf("00c3b840_TEST PASS\n");
    return 0;
}