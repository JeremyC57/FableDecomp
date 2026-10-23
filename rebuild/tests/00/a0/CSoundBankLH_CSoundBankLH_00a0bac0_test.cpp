#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_base = 0;
namespace N_0099a2f0 { struct CBaseClass { virtual void v00() { } CBaseClass() { ++g_base; } }; } using N_0099a2f0::CBaseClass;
struct SBankCounters { long m_count; long m_limit; bool m_flag; SBankCounters() : m_count(0), m_limit(0), m_flag(false) { } };
struct CSoundBankLH : CBaseClass { long m_4, m_8, m_bankIndex, m_10; SBankCounters m_counters; virtual void v00() { }
    CSoundBankLH(long bankIndex) : CBaseClass(), m_4(0), m_8(0), m_bankIndex(bankIndex), m_10(0), m_counters() { } };
int main()
{
    unsigned char raw[sizeof(CSoundBankLH)]; std::memset(raw, 0xee, sizeof(raw));
    CSoundBankLH* b = new (raw) CSoundBankLH(42);
    bool ok = g_base == 1 && b->m_4 == 0 && b->m_8 == 0 && b->m_bankIndex == 42 && b->m_10 == 0 && b->m_counters.m_count == 0 && b->m_counters.m_limit == 0 && !b->m_counters.m_flag;
    if (!ok) { std::printf("00a0bac0_TEST FAIL\n"); return 1; }
    std::printf("00a0bac0_TEST PASS\n"); return 0;
}