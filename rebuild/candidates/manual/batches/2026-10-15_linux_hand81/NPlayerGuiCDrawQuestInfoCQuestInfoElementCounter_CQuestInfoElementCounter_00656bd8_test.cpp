// Behaviour test for 0x00656BD8 CQuestInfoElementCounter(total, displayedTotal, height): the base gets the
// height, the count pair zero, the two totals stored.
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
    long m_count; unsigned long m_total; long m_displayedCount; unsigned long m_displayedTotal;
    CQuestInfoElementCounter(unsigned long total, unsigned long displayedTotal, float height)
        : CQuestInfoElement(height), m_count(0), m_total(total), m_displayedCount(0), m_displayedTotal(displayedTotal) { }
};

int main()
{
    std::memset(g_storage, 0xAB, sizeof g_storage);
    CQuestInfoElementCounter* p = new (g_storage) CQuestInfoElementCounter(6, 2, 8.0f);
    if (g_baseCtor != 1 || g_height != 8.0f || p->m_count != 0 || p->m_total != 6 || p->m_displayedCount != 0 || p->m_displayedTotal != 2) { std::printf("00656bd8_TEST FAIL\n"); return 1; }
    std::printf("00656bd8_TEST PASS\n");
    return 0;
}
