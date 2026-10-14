#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void* g_record = 0; static long g_value = 0;
struct CThing { virtual long Slot70() { std::strcat(g_seq, "A"); return 7; } virtual void Slot597(void* record, long value) { std::strcat(g_seq, "B"); g_record = record; g_value = value; } };
struct CKillBird { unsigned char _pad[4]; CThing* m_pThing; unsigned char m_record[4]; void Init() { m_pThing->Slot597(m_record, m_pThing->Slot70()); } };

int main()
{
    CThing t; CKillBird b; b.m_pThing = &t; b.Init(); if (g_record != b.m_record || g_value != 7 || std::strcmp(g_seq, "AB")) { std::printf("00d430f0_TEST FAIL\n"); return 1; }
    std::printf("00d430f0_TEST PASS\n");
    return 0;
}