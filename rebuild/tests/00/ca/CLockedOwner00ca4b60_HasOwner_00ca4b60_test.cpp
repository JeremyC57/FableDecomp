#include <cstdio>
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; }; static int g_enter = 0, g_leave = 0;
static void EnterCriticalSection(_RTL_CRITICAL_SECTION*) { ++g_enter; } static void LeaveCriticalSection(_RTL_CRITICAL_SECTION*) { ++g_leave; }
struct CLockedOwner { unsigned char _pad[0x6c]; void* m_pOwner; unsigned char _pad70[0x24]; _RTL_CRITICAL_SECTION m_section;
    long HasOwner() { EnterCriticalSection(&m_section); long r = m_pOwner != 0; LeaveCriticalSection(&m_section); return r; } };
int main() { CLockedOwner o; int x; o.m_pOwner = 0; if (o.HasOwner()) { std::printf("00ca4b60_TEST FAIL\n"); return 1; } o.m_pOwner = &x; if (!o.HasOwner() || g_enter != 2 || g_leave != 2) { std::printf("00ca4b60_TEST FAIL\n"); return 1; } std::printf("00ca4b60_TEST PASS\n"); return 0; }