#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_strings = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
struct CNamedRecord { CCharString m_name; long m_4, m_8, m_c, m_10, m_14, m_18, m_1c; bool m_20, m_21;
    CNamedRecord() : m_name(), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0), m_20(false), m_21(false) { } };
int main()
{
    unsigned char raw[sizeof(CNamedRecord)]; std::memset(raw, 0xee, sizeof(raw));
    CNamedRecord* r = new (raw) CNamedRecord();
    bool ok = g_strings == 1 && r->m_c == 0 && r->m_10 == 0 && r->m_14 == 0 && r->m_18 == 0 && r->m_1c == 0 && !r->m_20 && !r->m_21
        && r->m_4 == (long)0xeeeeeeee && r->m_8 == (long)0xeeeeeeee;
    if (!ok) { std::printf("00ab3520_TEST FAIL\n"); return 1; }
    std::printf("00ab3520_TEST PASS\n"); return 0;
}