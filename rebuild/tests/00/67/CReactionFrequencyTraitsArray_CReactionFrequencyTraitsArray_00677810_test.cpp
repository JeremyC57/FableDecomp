// Behaviour test for CReactionFrequencyTraitsArray::~CReactionFrequencyTraitsArray @ 0x00677810: every slot of both rows is deleted.
#include <cstdio>

static int g_deleted = 0;
class CReactionFrequencyTraits { public:
    int m_id;
    void operator delete(void* p) { if (p) ++g_deleted; ::operator delete(p); }
};

class CReactionFrequencyTraitsArray { public:
    CReactionFrequencyTraits* LowHigh[2][79];
    ~CReactionFrequencyTraitsArray()
    {
        for (unsigned long i = 0; i < 79; ++i) {
            delete LowHigh[0][i];
            delete LowHigh[1][i];
        }
    }
};

int main()
{
    {
        CReactionFrequencyTraitsArray a;
        for (int i = 0; i < 79; ++i) {
            a.LowHigh[0][i] = new CReactionFrequencyTraits;
            a.LowHigh[1][i] = (i % 3 == 0) ? 0 : new CReactionFrequencyTraits;
        }
    }
    if (g_deleted != 79 + 52) { std::printf("00677810_TEST FAIL\n"); return 1; }
    std::printf("00677810_TEST PASS\n");
    return 0;
}