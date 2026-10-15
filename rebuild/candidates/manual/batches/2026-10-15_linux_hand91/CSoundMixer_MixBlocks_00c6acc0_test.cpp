#include <cstdio>
// Behaviour test for CSoundMixer::MixBlocks (retail 0x00C6ACC0).
static int g_calls = 0;
static const unsigned char* g_sources[8];
static long g_counts[8];
static long* g_accums[8];
static unsigned short g_volumes[8];

void MixSamplesMMX(const unsigned char* source, long quadCount, long* accumulator, unsigned short volume)
{
    if (g_calls < 8) {
        g_sources[g_calls] = source; g_counts[g_calls] = quadCount;
        g_accums[g_calls] = accumulator; g_volumes[g_calls] = volume;
    }
    ++g_calls;
}

struct CSoundMixer {
    unsigned char _pad_0x00[0x08];
    unsigned char m_bMixed;
    unsigned char _pad_0x09[0x213];
    long*         m_pAccumulator;
    void MixBlocks(const unsigned char* blocks, unsigned long blockCount, unsigned short totalVolume);
};

void CSoundMixer::MixBlocks(const unsigned char* blocks, unsigned long blockCount, unsigned short totalVolume)
{
    unsigned short volume = (unsigned short)(totalVolume / blockCount);
    if (volume == 0)
        return;
    m_bMixed = 1;
    for (unsigned long i = 0; i < blockCount; ++i) {
        MixSamplesMMX(blocks, 0x10, m_pAccumulator, volume);
        blocks += 0x100;
    }
}

static int Fail() { std::printf("00c6acc0_TEST FAIL\n"); return 1; }

int main()
{
    static unsigned char samples[0x300];
    long accum[4];
    CSoundMixer m;
    m.m_bMixed = 0; m.m_pAccumulator = accum;
    // volume below the block count rounds to zero: nothing mixed, flag untouched
    m.MixBlocks(samples, 3, 2);
    if (g_calls != 0 || m.m_bMixed != 0) return Fail();
    m.MixBlocks(samples, 3, 3000);
    if (g_calls != 3 || m.m_bMixed != 1) return Fail();
    for (int i = 0; i < 3; ++i)
        if (g_sources[i] != samples + 0x100 * i || g_counts[i] != 0x10 || g_accums[i] != accum || g_volumes[i] != 1000) return Fail();
    std::printf("00c6acc0_TEST PASS\n");
    return 0;
}
