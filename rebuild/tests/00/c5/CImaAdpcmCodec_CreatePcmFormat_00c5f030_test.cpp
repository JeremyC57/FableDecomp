// behaviour test for 0x00C5F030 CImaAdpcmCodec::CreatePcmFormat: 16-bit PCM format fields
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

static void CreatePcmFormat(WORD nChannels, DWORD nSamplesPerSec, WAVEFORMATEX* pwfx)
{
    pwfx->wFormatTag = 1;
    pwfx->nChannels = nChannels;
    pwfx->nSamplesPerSec = nSamplesPerSec;
    pwfx->nBlockAlign = nChannels * 16 / 8;
    pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;
    pwfx->wBitsPerSample = 16;
}

int main()
{
    WAVEFORMATEX w;
    std::memset(&w, 0xAB, sizeof(w));
    CreatePcmFormat(2, 44100, &w);
    if (w.wFormatTag != 1 || w.nChannels != 2 || w.nSamplesPerSec != 44100 || w.nBlockAlign != 4) { std::printf("00c5f030_TEST FAIL\n"); return 1; }
    if (w.nAvgBytesPerSec != 176400 || w.wBitsPerSample != 16) { std::printf("00c5f030_TEST FAIL\n"); return 1; }
    if (w.cbSize != 0xABAB) { std::printf("00c5f030_TEST FAIL\n"); return 1; }                       // cbSize is left alone
    CreatePcmFormat(1, 22050, &w);
    if (w.nBlockAlign != 2 || w.nAvgBytesPerSec != 44100) { std::printf("00c5f030_TEST FAIL\n"); return 1; }
    std::printf("00c5f030_TEST PASS\n");
    return 0;
}