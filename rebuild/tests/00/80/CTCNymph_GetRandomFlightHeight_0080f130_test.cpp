// Behaviour test for CTCNymph::GetRandomFlightHeight @ 0x0080F130: seed advanced once, height = min + (max-min)*0.001*(r%1000).
#include <cstdio>
static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0) return 0;
    return seed % range;
}
class CNymphDef { public: float MaxHeight, MinHeight; };
class CTCNymph { public:
    const CNymphDef* PDef;
    unsigned long* m_pSeed;
    float GetRandomFlightHeight() const
    {
        unsigned long r = WorldRandomRange(*m_pSeed, 1000);
        float step = (PDef->MaxHeight - PDef->MinHeight) * 0.001f;
        return step * (float)r + PDef->MinHeight;
    }
};
int main()
{
    CNymphDef d = { 12.0f, 2.0f };
    unsigned long seed = 12345;
    CTCNymph n; n.PDef = &d; n.m_pSeed = &seed;
    unsigned long expectSeed = GFROR13(12345UL * 0x24A1 + 0x24DF);
    float h = n.GetRandomFlightHeight();
    float expect = 2.0f + 10.0f * 0.001f * (float)(expectSeed % 1000);
    float diff = h - expect; if (diff < 0) diff = -diff;
    if (seed != expectSeed || diff > 0.001f || h < 2.0f || h > 12.0f) { std::printf("0080f130_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 50; ++i) { float x = n.GetRandomFlightHeight(); if (x < 2.0f || x > 12.0f) { std::printf("0080f130_TEST FAIL\n"); return 1; } }
    std::printf("0080f130_TEST PASS\n");
    return 0;
}