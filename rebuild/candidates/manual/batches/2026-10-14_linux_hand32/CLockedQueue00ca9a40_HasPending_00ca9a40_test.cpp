#include <cstdio>
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; }; static int g_enter = 0, g_leave = 0;
static void EnterCriticalSection(_RTL_CRITICAL_SECTION*) { ++g_enter; } static void LeaveCriticalSection(_RTL_CRITICAL_SECTION*) { ++g_leave; }
struct CLockedQueue { unsigned char _pad[0xa4]; _RTL_CRITICAL_SECTION m_section; unsigned char _padbc[0x10]; unsigned long m_pendingCount;
    bool HasPending() { EnterCriticalSection(&m_section); bool r = m_pendingCount > 0; LeaveCriticalSection(&m_section); return r; } };
int main() { CLockedQueue q; q.m_pendingCount = 0; if (q.HasPending()) { std::printf("00ca9a40_TEST FAIL\n"); return 1; } q.m_pendingCount = 3; if (!q.HasPending() || g_enter != 2) { std::printf("00ca9a40_TEST FAIL\n"); return 1; } std::printf("00ca9a40_TEST PASS\n"); return 0; }
