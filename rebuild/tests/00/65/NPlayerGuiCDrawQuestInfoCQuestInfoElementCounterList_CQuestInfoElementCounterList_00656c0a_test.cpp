#include <cstdio>
static float g_baseHeight = 0;
namespace NPlayerGui {
struct CQuestInfoElement { virtual void v00() { } unsigned char _body[0x1c - 4]; CQuestInfoElement(float h) { g_baseHeight = h; } };
struct CQuestInfoElementCounterList : CQuestInfoElement { long m_firstCounter, m_cursor, m_counterCount;
    CQuestInfoElementCounterList(long f, long c, float h) : CQuestInfoElement(h), m_firstCounter(f), m_cursor(0), m_counterCount(c) { } }; }
int main()
{
    NPlayerGui::CQuestInfoElementCounterList e(4, 9, 2.5f);
    if (g_baseHeight != 2.5f || e.m_firstCounter != 4 || e.m_cursor != 0 || e.m_counterCount != 9 || (char*)&e.m_counterCount - (char*)&e != 0x24) { std::printf("00656c0a_TEST FAIL\n"); return 1; }
    std::printf("00656c0a_TEST PASS\n"); return 0;
}