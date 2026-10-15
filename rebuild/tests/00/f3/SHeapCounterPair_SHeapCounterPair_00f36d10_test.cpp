#include <new>
#include <cstdio>
// Behaviour test for SHeapCounterPair::SHeapCounterPair (0x00F36D10): both words start at zero.
struct SHeapCounterPair { long m_first; long m_second; SHeapCounterPair() { m_first = 0; m_second = 0; } };
int main()
{
    unsigned char raw[sizeof(SHeapCounterPair)]; for (unsigned i = 0; i < sizeof raw; ++i) raw[i] = 0xff;
    SHeapCounterPair* p = new (raw) SHeapCounterPair;
    int ok = p->m_first == 0 && p->m_second == 0;
    std::printf(ok ? "00f36d10_TEST PASS\n" : "00f36d10_TEST FAIL\n"); return ok ? 0 : 1;
}