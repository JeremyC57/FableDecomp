#include <cstdio>
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; }; static int g_enter = 0, g_leave = 0; static _RTL_CRITICAL_SECTION* g_cs = 0;
static void EnterCriticalSection(_RTL_CRITICAL_SECTION* c) { ++g_enter; g_cs = c; } static void LeaveCriticalSection(_RTL_CRITICAL_SECTION* c) { ++g_leave; }
struct CCountedSection { unsigned char _pad[0x14]; long m_count; unsigned char _pad18[8]; _RTL_CRITICAL_SECTION m_section; void Decrement() { EnterCriticalSection(&m_section); --m_count; LeaveCriticalSection(&m_section); } };
int main() { CCountedSection s; s.m_count = 3; s.Decrement(); if (s.m_count != 2 || g_enter != 1 || g_leave != 1 || g_cs != &s.m_section) { std::printf("00c5d700_TEST FAIL\n"); return 1; } std::printf("00c5d700_TEST PASS\n"); return 0; }