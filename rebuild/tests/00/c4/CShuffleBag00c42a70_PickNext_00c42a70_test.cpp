// Behaviour test for 00c42a70: CShuffleBag::PickNext -- swaps a random entry from slots 1..count-1 into slot 0 (never repeating the last pick) and returns slot 0
#include <cstdio>

static unsigned long g_nextRandom = 0, g_lastN = 0;
static unsigned long RandomBelow(unsigned long n) { g_lastN = n; return g_nextRandom % n; }

struct CShuffleBag {
    long* m_pEntries;
    unsigned long m_capacity;
    unsigned long m_count;
    long PickNext()
    {
        if (m_count > 1) {
            unsigned long index = RandomBelow(m_count - 1) + 1;
            long picked = m_pEntries[index];
            m_pEntries[index] = m_pEntries[0];
            m_pEntries[0] = picked;
        }
        return m_pEntries[0];
    }
};

static int fail() { std::printf("00c42a70_TEST FAIL\n"); return 1; }

int main()
{
    long entries[4] = { 10, 20, 30, 40 };
    CShuffleBag bag = { entries, 4, 4 };
    g_nextRandom = 1;                                  // picks slot 2
    if (bag.PickNext() != 30 || entries[2] != 10 || g_lastN != 3) return fail();
    g_nextRandom = 0;                                  // picks slot 1
    if (bag.PickNext() != 20 || entries[1] != 30) return fail();
    bag.m_count = 1; g_lastN = 0;
    if (bag.PickNext() != 20 || g_lastN != 0) return fail();
    std::printf("00c42a70_TEST PASS\n");
    return 0;
}