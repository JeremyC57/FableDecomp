// Behaviour test for CArenaWaveDef::operator= (0x00F25F60).
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

struct CDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x24];
    CDefClassBase& operator=(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x24; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CArenaCreatureDefVector {
    void* m_first; void* m_last; void* m_end;
    CArenaCreatureDefVector& operator=(const CArenaCreatureDefVector& o)
    {
        m_first = o.m_first; m_last = o.m_last; m_end = o.m_end;
        g_order[g_calls++] = 2;
        return *this;
    }
};

struct CArenaWaveDef : CDefClassBase {
    long                    NumWaveCreatures;
    CArenaCreatureDefVector Creatures;
    bool                    ShortWave;
    CArenaWaveDef& operator=(const CArenaWaveDef& other);
};

CArenaWaveDef& CArenaWaveDef::operator=(const CArenaWaveDef& other)
{
    CDefClassBase::operator=(other);
    NumWaveCreatures = other.NumWaveCreatures;
    Creatures = other.Creatures;
    ShortWave = other.ShortWave;
    return *this;
}

int main()
{
    static int creatures[3];
    CArenaWaveDef a, b;
    for (int i = 0; i < 0x24; ++i) { a._body[i] = 0; b._body[i] = 6; }
    a.NumWaveCreatures = 0; b.NumWaveCreatures = 3;
    a.Creatures.m_first = a.Creatures.m_last = a.Creatures.m_end = 0;
    b.Creatures.m_first = creatures; b.Creatures.m_last = creatures + 3; b.Creatures.m_end = creatures + 3;
    a.ShortWave = false; b.ShortWave = true;
    CArenaWaveDef& r = (a = b);
    if (&r != &a || g_calls != 2 || g_order[0] != 1 || g_order[1] != 2 || a._body[2] != 6 || a.NumWaveCreatures != 3
        || a.Creatures.m_end != creatures + 3 || !a.ShortWave) {
        std::printf("00f25f60_TEST FAIL\n");
        return 1;
    }
    std::printf("00f25f60_TEST PASS\n");
    return 0;
}