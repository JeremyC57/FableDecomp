#include <cstdio>
#include <cstring>
#include <math.h>

static char g_log[64]; static int g_logLen = 0;
static void Log(char c) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; }

struct CStringRecordList { void* m_head; void clear() { Log('c'); } ~CStringRecordList() { clear(); if (m_head) Log('f'); } };
struct CPodList { int n; ~CPodList() { Log('p'); } };
struct CBase { int x; ~CBase() { Log('B'); } };
struct CDerived : CBase { CStringRecordList m_records; CPodList m_entries; ~CDerived() { } };
int main()
{
    { static char head[8]; CDerived d; d.m_records.m_head = head; }
    if (std::strcmp(g_log, "pcfB") != 0) { std::printf("00443220_TEST FAIL\n"); return 1; }
    g_logLen = 0; g_log[0] = 0;
    { CDerived e; e.m_records.m_head = 0; }
    if (std::strcmp(g_log, "pcB") != 0) { std::printf("00443220_TEST FAIL\n"); return 1; }
    std::printf("00443220_TEST PASS\n");
    return 0;
}
