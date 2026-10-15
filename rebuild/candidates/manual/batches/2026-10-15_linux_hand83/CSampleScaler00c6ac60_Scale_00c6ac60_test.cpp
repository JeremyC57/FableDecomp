// Behaviour test for 00c6ac60 (CSampleScaler_00c6ac60::Scale): a non-zero volume marks the block scaled and scales 16 samples from the source into dest; zero does nothing.
#include <cstdio>

static void ScaleSamplesByPackedVolume(void* dest, long count, const void* source, unsigned short volume)
{
    short* d = (short*)dest; const short* s = (const short*)source;
    for (long i = 0; i < count; ++i)
        d[i] = (short)((s[i] * (long)volume) >> 8);
}

struct CSampleScaler {
    unsigned char m_bScaled;
    const void*   m_pSource;
    void Scale(void* dest, unsigned short volume)
    {
        if (volume) {
            m_bScaled = 1;
            ScaleSamplesByPackedVolume(dest, 0x10, m_pSource, volume);
        }
    }
};

int main()
{
    short src[16], dst[16];
    for (int i = 0; i < 16; ++i) { src[i] = (short)(i * 4); dst[i] = -1; }
    CSampleScaler sc; sc.m_bScaled = 0; sc.m_pSource = src;
    sc.Scale(dst, 0);
    if (sc.m_bScaled != 0 || dst[0] != -1) { std::printf("00c6ac60_TEST FAIL\n"); return 1; }
    sc.Scale(dst, 0x80);
    for (int i = 0; i < 16; ++i)
        if (dst[i] != i * 2) { std::printf("00c6ac60_TEST FAIL\n"); return 1; }
    if (sc.m_bScaled != 1) { std::printf("00c6ac60_TEST FAIL\n"); return 1; }
    std::printf("00c6ac60_TEST PASS\n");
    return 0;
}
