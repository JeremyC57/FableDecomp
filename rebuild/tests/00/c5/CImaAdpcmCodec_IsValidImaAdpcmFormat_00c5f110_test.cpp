// behaviour test for 0x00C5F110 CImaAdpcmCodec::IsValidImaAdpcmFormat: format validation
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

static BOOL IsValidImaAdpcmFormat(const IMAADPCMWAVEFORMAT* pwfx)
{
    if (0x69 != pwfx->wfx.wFormatTag) return 0;
    if (sizeof(*pwfx) - sizeof(pwfx->wfx) != pwfx->wfx.cbSize) return 0;
    if ((pwfx->wfx.nChannels < 1) || (pwfx->wfx.nChannels > 2)) return 0;
    if (4 != pwfx->wfx.wBitsPerSample) return 0;
    if (CalculateEncodeAlignment(pwfx->wfx.nChannels, pwfx->wSamplesPerBlock) != pwfx->wfx.nBlockAlign) return 0;
    return 1;
}

int main()
{
    IMAADPCMWAVEFORMAT f;
    std::memset(&f, 0, sizeof(f));
    f.wfx.wFormatTag = 0x69; f.wfx.nChannels = 2; f.wfx.nSamplesPerSec = 44100; f.wfx.nBlockAlign = 72;
    f.wfx.wBitsPerSample = 4; f.wfx.cbSize = 2; f.wSamplesPerBlock = 64;
    if (IsValidImaAdpcmFormat(&f) != 1) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    IMAADPCMWAVEFORMAT g = f; g.wfx.wFormatTag = 1;     if (IsValidImaAdpcmFormat(&g) != 0) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    g = f; g.wfx.cbSize = 0;                            if (IsValidImaAdpcmFormat(&g) != 0) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    g = f; g.wfx.nChannels = 0;                         if (IsValidImaAdpcmFormat(&g) != 0) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    g = f; g.wfx.nChannels = 3;                         if (IsValidImaAdpcmFormat(&g) != 0) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    g = f; g.wfx.wBitsPerSample = 16;                   if (IsValidImaAdpcmFormat(&g) != 0) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    g = f; g.wfx.nBlockAlign = 36;                      if (IsValidImaAdpcmFormat(&g) != 0) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    g = f; g.wfx.nChannels = 1;                         if (IsValidImaAdpcmFormat(&g) != 0) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    g.wfx.nBlockAlign = 36;                             if (IsValidImaAdpcmFormat(&g) != 1) { std::printf("00c5f110_TEST FAIL\n"); return 1; }
    std::printf("00c5f110_TEST PASS\n");
    return 0;
}