// behaviour test for 0x00C5F350 CImaAdpcmCodec::DecodeSample: 4-bit dequantisation with 16-bit saturation
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

static INT DecodeSample(INT nInputSample, INT nPredictedSample, INT nStepSize)
{
    LONG lDifference = nStepSize >> 3;
    if (nInputSample & 4) lDifference += nStepSize;
    if (nInputSample & 2) lDifference += nStepSize >> 1;
    if (nInputSample & 1) lDifference += nStepSize >> 2;
    if (nInputSample & 8) lDifference = -lDifference;
    LONG lNewSample = nPredictedSample + lDifference;
    if ((LONG)(short)lNewSample != lNewSample)
    {
        if (lNewSample < -32768) lNewSample = -32768;
        else lNewSample = 32767;
    }
    return (INT)lNewSample;
}

int main()
{
    if (DecodeSample(0, 0, 64) != 8) { std::printf("00c5f350_TEST FAIL\n"); return 1; }                 // step/8
    if (DecodeSample(7, 0, 64) != 8 + 64 + 32 + 16) { std::printf("00c5f350_TEST FAIL\n"); return 1; }
    if (DecodeSample(15, 0, 64) != -(8 + 64 + 32 + 16)) { std::printf("00c5f350_TEST FAIL\n"); return 1; }
    if (DecodeSample(6, 1000, 64) != 1000 + 8 + 64 + 32) { std::printf("00c5f350_TEST FAIL\n"); return 1; }
    if (DecodeSample(7, 32700, 1000) != 32767) { std::printf("00c5f350_TEST FAIL\n"); return 1; }      // saturate high
    if (DecodeSample(15, -32700, 1000) != -32768) { std::printf("00c5f350_TEST FAIL\n"); return 1; }   // saturate low
    std::printf("00c5f350_TEST PASS\n");
    return 0;
}