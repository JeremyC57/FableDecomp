// behaviour test for 0x00C5F000 CImaAdpcmCodec::CalculateEncodeAlignment: Xbox ADPCM block alignment
#include <cstdio>
typedef unsigned short WORD;

static WORD CalculateEncodeAlignment(WORD nChannels, WORD nSamplesPerBlock)
{
    const WORD nEncodedSampleBits = nChannels * 4;
    const WORD nHeaderBytes = nChannels * 4;
    int nBlockAlign;
    nBlockAlign = nSamplesPerBlock - 1;
    nBlockAlign *= nEncodedSampleBits;
    nBlockAlign += 7;
    nBlockAlign /= 8;
    nBlockAlign += 7;
    nBlockAlign /= 8;
    nBlockAlign *= 8;
    nBlockAlign += nHeaderBytes;
    return (WORD)nBlockAlign;
}

int main()
{
    // the canonical Xbox ADPCM blocks: 64 samples -> 36 bytes mono, 72 bytes stereo
    if (CalculateEncodeAlignment(1, 64) != 36) { std::printf("00c5f000_TEST FAIL\n"); return 1; }
    if (CalculateEncodeAlignment(2, 64) != 72) { std::printf("00c5f000_TEST FAIL\n"); return 1; }
    // one sample: no encoded data, header only
    if (CalculateEncodeAlignment(2, 1) != 8) { std::printf("00c5f000_TEST FAIL\n"); return 1; }
    // 65 samples mono: 64*4 bits = 32 bytes -> 32, + 4 header
    if (CalculateEncodeAlignment(1, 65) != 36) { std::printf("00c5f000_TEST FAIL\n"); return 1; }
    // 66 samples mono: 65*4 bits = 33 bytes -> rounded to 40, + 4
    if (CalculateEncodeAlignment(1, 66) != 44) { std::printf("00c5f000_TEST FAIL\n"); return 1; }
    std::printf("00c5f000_TEST PASS\n");
    return 0;
}