// Behaviour test for 0x00656BA2 CQuestInfoElementCounter(count, total, height): the base gets the height,
// count and total stored, the two displayed values zero.
#include <cstdio>
#include <cstring>
#include <new>
static float g_height;
static int g_baseCtor;
struct CQuestInfoElement {
    virtual void v00() { }
    unsigned char _body[0x18];
    CQuestInfoElement(float height) { g_height = height; ++g_baseCtor; }
};
static double g_storage[32];
struct CQuestInfoElementCounter : CQuestInfoElement {
    long m_count, m_total, m_displayedCount, m_displayedTotal;
    CQuestInfoElementCounter(long count, long total, float height)
        : CQuestInfoElement(height), m_count(count), m_total(total), m_displayedCount(0), m_displayedTotal(0) { }
};

int main()
{
    std::memset(g_storage, 0xAB, sizeof g_storage);
    CQuestInfoElementCounter* p = new (g_storage) CQuestInfoElementCounter(4, 9, 12.5f);
    if (g_baseCtor != 1 || g_height != 12.5f || p->m_count != 4 || p->m_total != 9 || p->m_displayedCount != 0 || p->m_displayedTotal != 0) { std::printf("00656ba2_TEST FAIL\n"); return 1; }
    std::printf("00656ba2_TEST PASS\n");
    return 0;
}
