// Behaviour test for the 0x00C3E260 default-descriptor filler: the three module defaults then four zero words.
#include <cstdio>
struct SDescriptor { unsigned long m_00, m_04, m_08, m_0c, m_10, m_14, m_18; };
static unsigned long g_a = 0xFFFFFFFFul, g_b = 0x12345678ul, g_c = 0x80ul;
static void InitDefaultDescriptor(SDescriptor* d)
{
    d->m_00 = g_a; d->m_04 = g_b; d->m_08 = g_c;
    d->m_0c = 0; d->m_10 = 0; d->m_14 = 0; d->m_18 = 0;
}
int main()
{
    SDescriptor d; d.m_00 = d.m_04 = d.m_08 = d.m_0c = d.m_10 = d.m_14 = d.m_18 = 7;
    InitDefaultDescriptor(&d);
    bool ok = d.m_00 == 0xFFFFFFFFul && d.m_04 == 0x12345678ul && d.m_08 == 0x80ul && d.m_0c == 0 && d.m_10 == 0 && d.m_14 == 0 && d.m_18 == 0;
    if (!ok) { std::printf("00c3e260_TEST FAIL\n"); return 1; }
    std::printf("00c3e260_TEST PASS\n");
    return 0;
}