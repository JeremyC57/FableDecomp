#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

extern char* g_pchNil; static char s_nil[1] = { 0 }; char* g_pchNil = s_nil;
struct CAutoMapEntry {
    void* m_pKey; long m_4; char* m_pchName; short m_c, m_e; long m_10, m_14; short m_18, m_1a, m_1c, m_1e, m_20, m_22; long m_24, m_28, m_2c;
    CAutoMapEntry() : m_pKey(0), m_4(0), m_pchName(g_pchNil), m_c(0), m_e(0), m_10(0), m_14(0), m_18(0), m_1a(0), m_1c(0), m_1e(0), m_20(0), m_22(0), m_24(0), m_28(0), m_2c(0) { }
};
int main()
{
    unsigned char raw[sizeof(CAutoMapEntry)]; std::memset(raw, 0xee, sizeof(raw));
    CAutoMapEntry* e = new (raw) CAutoMapEntry();
    bool ok = sizeof(CAutoMapEntry) == 0x30 && e->m_pKey == 0 && e->m_4 == 0 && e->m_pchName == s_nil && e->m_c == 0 && e->m_e == 0
        && e->m_10 == 0 && e->m_14 == 0 && e->m_18 == 0 && e->m_1a == 0 && e->m_1c == 0 && e->m_1e == 0 && e->m_20 == 0 && e->m_22 == 0
        && e->m_24 == 0 && e->m_28 == 0 && e->m_2c == 0;
    if (!ok) { std::printf("00c3baf0_TEST FAIL\n"); return 1; }
    std::printf("00c3baf0_TEST PASS\n"); return 0;
}
