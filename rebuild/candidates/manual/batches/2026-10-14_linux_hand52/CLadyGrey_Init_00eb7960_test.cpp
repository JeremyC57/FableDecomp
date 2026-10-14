#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void* g_recordA = 0; static void* g_recordB = 0;
struct CThing { virtual void Slot477(void* record, long a) { char b[32]; std::sprintf(b, "B(%ld)", a); std::strcat(g_seq, b); g_recordB = record; }
    virtual void Slot517(void* record, long a, long c) { char b[32]; std::sprintf(b, "A(%ld,%ld)", a, c); std::strcat(g_seq, b); g_recordA = record; } };
struct CLadyGrey { unsigned char _pad[4]; CThing* m_pThing; unsigned char m_record[0x14]; bool m_flag;
    void Init() { m_flag = false; m_pThing->Slot517(m_record, 0, 1); m_pThing->Slot477(m_record, 1); } };

int main()
{
    CThing t; CLadyGrey g; g.m_pThing = &t; g.m_flag = true; g.Init(); if (g.m_flag || std::strcmp(g_seq, "A(0,1)B(1)") || g_recordA != g.m_record || g_recordB != g.m_record) { std::printf("00eb7960_TEST FAIL\n"); return 1; }
    std::printf("00eb7960_TEST PASS\n");
    return 0;
}
