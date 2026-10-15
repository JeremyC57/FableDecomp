#include <cstdio>
#include <cstring>
#include <math.h>

static char g_log[64]; static int g_logLen = 0;
static void Log(char c) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; }

struct CPolymorphicList { void* m_head; void clear() { Log('c'); } ~CPolymorphicList() { clear(); if (m_head) Log('f'); } };
struct COwner { long m_0; CPolymorphicList m_list; };
static void Destroy(COwner* owner) { delete owner; }
int main()
{
    static char head[8];
    COwner* o = new COwner; o->m_0 = 1; o->m_list.m_head = head;
    Destroy(o);
    if (std::strcmp(g_log, "cf") != 0) { std::printf("004ad650_TEST FAIL\n"); return 1; }
    g_logLen = 0; g_log[0] = 0;
    COwner* p = new COwner; p->m_0 = 2; p->m_list.m_head = 0;
    Destroy(p);
    if (std::strcmp(g_log, "c") != 0) { std::printf("004ad650_TEST FAIL\n"); return 1; }
    std::printf("004ad650_TEST PASS\n");
    return 0;
}
