// Behaviour test for 00c0ffc0: PKCProtectionInstance constructor -- keeps and enters the section, records the entry tick
#include <cstdio>
static int g_enters = 0; static unsigned long g_now = 1234;
static unsigned long GetTickCount() { return g_now; }
struct PKCCriticalSection { unsigned char m_section[0x18]; void Enter() { ++g_enters; } };
struct PKCProtectionInstance { PKCCriticalSection* m_pSection; unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section) : m_pSection(section) { section->Enter(); m_tick = GetTickCount(); } };
static bool Check() { PKCCriticalSection cs; PKCProtectionInstance guard(&cs); return guard.m_pSection == &cs && guard.m_tick == 1234 && g_enters == 1 && sizeof(guard) == 8; }

int main()
{
    if (!Check()) { std::printf("00c0ffc0_TEST FAIL\n"); return 1; }
    std::printf("00c0ffc0_TEST PASS\n");
    return 0;
}
