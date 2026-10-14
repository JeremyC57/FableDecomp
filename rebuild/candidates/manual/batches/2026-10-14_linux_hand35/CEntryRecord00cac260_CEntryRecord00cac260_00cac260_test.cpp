#include <cstdio>
#include <string.h>
static long g_args[6];
struct CEntryBase_00cac070 { unsigned char _body[0x9c]; CEntryBase_00cac070(long a, long b, long c, long d, long e, long f) { g_args[0] = a; g_args[1] = b; g_args[2] = c; g_args[3] = d; g_args[4] = e; g_args[5] = f; } };
struct CEntryRecord_00cac260 : CEntryBase_00cac070 { long m_count; bool m_flagA; bool m_flagB; unsigned char _pada2[6]; long m_table[12];
    CEntryRecord_00cac260(long a, long b, long c, long d, long e) : CEntryBase_00cac070(a, b, c, d, e, 0), m_count(0), m_flagA(false), m_flagB(false) { memset(m_table, 0, sizeof(m_table)); } };
int main() {
    CEntryRecord_00cac260 e(1, 2, 3, 4, 5); if (g_args[0] != 1 || g_args[4] != 5 || g_args[5] != 0 || e.m_count || e.m_flagA || e.m_flagB || e.m_table[0] || e.m_table[11]) { std::printf("00cac260_TEST FAIL\n"); return 1; }
    std::printf("00cac260_TEST PASS\n"); return 0;
}
