// behaviour test for 0x00C5F2D0 CImaAdpcmCodec::EncodeSample: 4-bit quantisation and prediction update
#include <cstdio>
#include <cstring>
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef int            BOOL;
typedef int            INT;
typedef int            LONG;

#pragma pack(push, 1)
struct WAVEFORMATEX {
    WORD  wFormatTag;
    WORD  nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD  nBlockAlign;
    WORD  wBitsPerSample;
    WORD  cbSize;
};
struct IMAADPCMWAVEFORMAT {
    WAVEFORMATEX wfx;
    WORD         wSamplesPerBlock;
};
#pragma pack(pop)

static WORD CalculateEncodeAlignment(WORD nChannels, WORD nSamplesPerBlock)
{
    const WORD nEncodedSampleBits = nChannels * 4;
    const WORD nHeaderBytes = nChannels * 4;
    int n = nSamplesPerBlock - 1;
    n *= nEncodedSampleBits; n += 7; n /= 8; n += 7; n /= 8; n *= 8; n += nHeaderBytes;
    return (WORD)n;
}

static INT EncodeSample(INT nInputSample, INT* pnPredictedSample, INT nStepSize)
{
    INT nPredictedSample = *pnPredictedSample;
    LONG lDifference = nInputSample - nPredictedSample;
    INT nEncodedSample = 0;
    if (lDifference < 0) { nEncodedSample = 8; lDifference = -lDifference; }
    if (lDifference >= nStepSize) { nEncodedSample |= 4; lDifference -= nStepSize; }
    nStepSize >>= 1;
    if (lDifference >= nStepSize) { nEncodedSample |= 2; lDifference -= nStepSize; }
    nStepSize >>= 1;
    if (lDifference >= nStepSize) { nEncodedSample |= 1; lDifference -= nStepSize; }
    if (nEncodedSample & 8) nPredictedSample = nInputSample + lDifference - (nStepSize >> 1);
    else nPredictedSample = nInputSample - lDifference + (nStepSize >> 1);
    if (nPredictedSample > 32767) nPredictedSample = 32767;
    else if (nPredictedSample < -32768) nPredictedSample = -32768;
    *pnPredictedSample = nPredictedSample;
    return nEncodedSample;
}

int main()
{
    INT p = 0;
    // diff 100 with step 64: 100 >= 64 -> 4 (36 left), 36 >= 32 -> 2 (4 left), 4 < 16; new = 100 - 4 + 8 = 104
    if (EncodeSample(100, &p, 64) != 6 || p != 104) { std::printf("00c5f2d0_TEST FAIL\n"); return 1; }
    p = 0;
    // diff -100: sign 8, same magnitude code; new = -100 + 4 - 8 = -104
    if (EncodeSample(-100, &p, 64) != 14 || p != -104) { std::printf("00c5f2d0_TEST FAIL\n"); return 1; }
    p = 32761;
    if (EncodeSample(32767, &p, 8) != 3 || p != 32767) { std::printf("00c5f2d0_TEST FAIL\n"); return 1; }      // 32767 - 0 + 1 = 32768 clamps to 32767
    p = -32762;
    if (EncodeSample(-32768, &p, 8) != 11 || p != -32768) { std::printf("00c5f2d0_TEST FAIL\n"); return 1; }   // -32768 + 0 - 1 clamps to -32768
    p = 50;
    if (EncodeSample(50, &p, 64) != 0 || p != 58) { std::printf("00c5f2d0_TEST FAIL\n"); return 1; }
    std::printf("00c5f2d0_TEST PASS\n");
    return 0;
}