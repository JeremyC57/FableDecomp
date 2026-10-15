// Behaviour test for the CArenaWaveDef copy constructor (0x00F25C60).
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

struct CDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x24];
    CDefClassBase() {}
    CDefClassBase(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x24; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
    }
};

struct CArenaCreatureDefVector {
    void* m_first; void* m_last; void* m_end;
    CArenaCreatureDefVector() : m_first(0), m_last(0), m_end(0) {}
    CArenaCreatureDefVector(const CArenaCreatureDefVector& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end)
    {
        g_order[g_calls++] = 2;
    }
};

struct CArenaWaveDef : CDefClassBase {
    long                    NumWaveCreatures;
    CArenaCreatureDefVector Creatures;
    bool                    ShortWave;
    CArenaWaveDef() : NumWaveCreatures(0), ShortWave(false) {}
    CArenaWaveDef(const CArenaWaveDef& other);
};

CArenaWaveDef::CArenaWaveDef(const CArenaWaveDef& other)
    : CDefClassBase(other)
    , NumWaveCreatures(other.NumWaveCreatures)
    , Creatures(other.Creatures)
    , ShortWave(other.ShortWave)
{
}

int main()
{
    static int creatures[5];
    CArenaWaveDef src;
    for (int i = 0; i < 0x24; ++i) src._body[i] = 4;
    src.NumWaveCreatures = 5;
    src.Creatures.m_first = creatures; src.Creatures.m_last = creatures + 5; src.Creatures.m_end = creatures + 5;
    src.ShortWave = true;
    CArenaWaveDef copy(src);
    if (g_calls != 2 || g_order[0] != 1 || g_order[1] != 2 || copy._body[0x23] != 4 || copy.NumWaveCreatures != 5
        || copy.Creatures.m_first != creatures || copy.Creatures.m_last != creatures + 5 || !copy.ShortWave) {
        std::printf("00f25c60_TEST FAIL\n");
        return 1;
    }
    std::printf("00f25c60_TEST PASS\n");
    return 0;
}