#include <cstdio>
#include <cstring>
#include <math.h>

static char g_log[64]; static int g_logLen = 0;
static void Log(char c) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; }

struct SRecord { int id; };
static int g_destroyed = 0;
static void DestroyRange(SRecord* first, SRecord* last) { for (; first != last; ++first) ++g_destroyed; Log('r'); }
struct CRecordVector { SRecord* m_first; SRecord* m_last; SRecord* m_end; void Free() { if (m_first) Log('F'); } ~CRecordVector() { DestroyRange(m_first, m_last); Free(); } };
struct CPodVector { void* m_first; void* m_last; void* m_end; ~CPodVector() { if (m_first) Log('v'); } };
struct CBase { int x; ~CBase() { Log('B'); } };
struct CDerived : CBase { CPodVector m_values; CRecordVector m_records; ~CDerived() { } };
int main()
{
    static SRecord recs[3]; static long pod[2];
    { CDerived d; d.m_records.m_first = recs; d.m_records.m_last = recs + 3; d.m_records.m_end = recs + 3; d.m_values.m_first = pod; d.m_values.m_last = d.m_values.m_end = pod + 2; }
    if (std::strcmp(g_log, "rFvB") != 0 || g_destroyed != 3) { std::printf("0047747c_TEST FAIL\n"); return 1; }
    g_logLen = 0; g_log[0] = 0;
    { CDerived e; e.m_records.m_first = e.m_records.m_last = e.m_records.m_end = 0; e.m_values.m_first = e.m_values.m_last = e.m_values.m_end = 0; }
    if (std::strcmp(g_log, "rB") != 0 || g_destroyed != 3) { std::printf("0047747c_TEST FAIL\n"); return 1; }
    std::printf("0047747c_TEST PASS\n");
    return 0;
}