#include <cstdio>
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; }; static int g_enter = 0, g_leave = 0;
static void EnterCriticalSection(_RTL_CRITICAL_SECTION*) { ++g_enter; } static void LeaveCriticalSection(_RTL_CRITICAL_SECTION*) { ++g_leave; }
struct CLockedOwner { unsigned char _pad[0x94]; _RTL_CRITICAL_SECTION m_section; unsigned char _padac[4]; long m_context;
    void SetContext(long c) { EnterCriticalSection(&m_section); m_context = c; LeaveCriticalSection(&m_section); } };
int main() { CLockedOwner o; o.m_context = 0; o.SetContext(77); if (o.m_context != 77 || g_enter != 1 || g_leave != 1) { std::printf("00ca4ea0_TEST FAIL\n"); return 1; } std::printf("00ca4ea0_TEST PASS\n"); return 0; }
