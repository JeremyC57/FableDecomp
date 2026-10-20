// Behaviour test for 00569330: body copied from src/00569330.cpp (CNetString_Retail::Substr), callees stubbed.
#include <cstdio>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SForwardIteratorTag { SForwardIteratorTag() { } };
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
static int g_throw = 0, g_init = 0; static const char* g_first = 0; static const char* g_last = 0;
struct CNetString_Retail {
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    void ThrowOutOfRange() const { ++g_throw; }
    void RangeInitialize(const char* first, const char* last, const SForwardIteratorTag& tag) { ++g_init; g_first = first; g_last = last; (void)tag; }
    CNetString_Retail() : m_start(0), m_finish(0), m_endOfStorage(0) { }
    CNetString_Retail(const char* first, const char* last) : m_start(0), m_finish(0), m_endOfStorage(0)
    {
        RangeInitialize(first, last, SForwardIteratorTag());
    }
    unsigned int size() const { return m_finish - m_start; }
    CNetString_Retail Substr(unsigned int pos, unsigned int n) const;
};
CNetString_Retail CNetString_Retail::Substr(unsigned int pos, unsigned int n) const
{
    if (pos > size())
        ThrowOutOfRange();
    unsigned int len = Min(n, size() - pos);
    return CNetString_Retail(m_start + pos, m_start + pos + len);
}
int main()
{
    char buf[16] = "hello world";
    CNetString_Retail s; s.m_start = buf; s.m_finish = buf + 11; s.m_endOfStorage = buf + 16;
    CNetString_Retail r = s.Substr(6, 3);               // in range, n shorter than the tail
    CHECK(g_throw == 0 && g_init == 1 && g_first == buf + 6 && g_last == buf + 9 && r.m_start == 0);
    r = s.Substr(6, 100);                               // n clipped to size()-pos
    CHECK(g_init == 2 && g_first == buf + 6 && g_last == buf + 11);
    r = s.Substr(11, 5);                                // pos == size(): empty range, no throw
    CHECK(g_throw == 0 && g_init == 3 && g_first == buf + 11 && g_last == buf + 11);
    r = s.Substr(12, 0);                                // pos > size(): out-of-range path taken
    CHECK(g_throw == 1 && g_init == 4);
    if (fails == 0) printf("00569330_TEST PASS\n"); else printf("00569330_TEST FAIL (%d)\n", fails);
    return fails;
}