// Behaviour test for the 0x00564B00 record constructor: the leading word is zeroed and the CWideString member is
// default-constructed (null string, constructor called once).
#include <cstdio>
static int g_wideCtors = 0;
struct CWideString { unsigned short* m_pString; CWideString() : m_pString(0) { ++g_wideCtors; } };
struct CRecord { long m_first; CWideString m_second; CRecord() : m_first(0), m_second() { } };
int main()
{
    CRecord r;
    if (r.m_first != 0 || r.m_second.m_pString != 0 || g_wideCtors != 1) { std::printf("00564b00_TEST FAIL\n"); return 1; }
    std::printf("00564b00_TEST PASS\n");
    return 0;
}
