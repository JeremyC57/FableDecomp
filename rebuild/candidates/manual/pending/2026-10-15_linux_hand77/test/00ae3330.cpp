#include <cstdio>
// Behaviour test for 00ae3330: ReserveBoth reserves the same count in both vectors, first then second.
static int g_order[4]; static unsigned long g_counts[4]; static int g_n = 0;
struct CVector16 {
    int m_id;
    unsigned long m_capacity;
    void Reserve(unsigned long count) { if (m_capacity < count) m_capacity = count; g_order[g_n] = m_id; g_counts[g_n] = count; ++g_n; }
};
struct CVectorPairOwner_00ae3330 {
    CVector16 m_first;
    CVector16 m_second;
    void ReserveBoth(unsigned long count);
};
void CVectorPairOwner_00ae3330::ReserveBoth(unsigned long count)
{
    m_first.Reserve(count);
    m_second.Reserve(count);
}
int main()
{
    CVectorPairOwner_00ae3330 o;
    o.m_first.m_id = 1; o.m_first.m_capacity = 0;
    o.m_second.m_id = 2; o.m_second.m_capacity = 50;
    o.ReserveBoth(20);
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_counts[0] != 20 || g_counts[1] != 20 || o.m_first.m_capacity != 20 || o.m_second.m_capacity != 50) {
        std::printf("00ae3330_TEST FAIL\n"); return 1;
    }
    std::printf("00ae3330_TEST PASS\n");
    return 0;
}
