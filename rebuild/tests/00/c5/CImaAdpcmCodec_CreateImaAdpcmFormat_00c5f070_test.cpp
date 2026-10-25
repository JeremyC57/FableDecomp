// behaviour test for 0x00C5F070 CImaAdpcmCodec::CreateImaAdpcmFormat: Xbox ADPCM format fields
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

static void CreateImaAdpcmFormat(WORD nChannels, DWORD nSamplesPerSec, WORD nSamplesPerBlock, IMAADPCMWAVEFORMAT* pwfx)
{
    pwfx->wfx.wFormatTag = 0x69;
    pwfx->wfx.nChannels = nChannels;
    pwfx->wfx.nSamplesPerSec = nSamplesPerSec;
    pwfx->wfx.nBlockAlign = CalculateEncodeAlignment(nChannels, nSamplesPerBlock);
    pwfx->wfx.nAvgBytesPerSec = pwfx->wfx.nBlockAlign * nSamplesPerSec / nSamplesPerBlock;
    pwfx->wfx.wBitsPerSample = 4;
    pwfx->wfx.cbSize = sizeof(*pwfx) - sizeof(pwfx->wfx);
    pwfx->wSamplesPerBlock = nSamplesPerBlock;
}

int main()
{
    IMAADPCMWAVEFORMAT f;
    if (sizeof(f) != 20 || sizeof(f.wfx) != 18) { std::printf("00c5f070_TEST FAIL\n"); return 1; }
    CreateImaAdpcmFormat(1, 44100, 64, &f);
    if (f.wfx.wFormatTag != 0x69 || f.wfx.nChannels != 1 || f.wfx.nSamplesPerSec != 44100) { std::printf("00c5f070_TEST FAIL\n"); return 1; }
    if (f.wfx.nBlockAlign != 36 || f.wfx.nAvgBytesPerSec != 36u * 44100u / 64u) { std::printf("00c5f070_TEST FAIL\n"); return 1; }
    if (f.wfx.wBitsPerSample != 4 || f.wfx.cbSize != 2 || f.wSamplesPerBlock != 64) { std::printf("00c5f070_TEST FAIL\n"); return 1; }
    CreateImaAdpcmFormat(2, 22050, 64, &f);
    if (f.wfx.nBlockAlign != 72 || f.wfx.nAvgBytesPerSec != 72u * 22050u / 64u) { std::printf("00c5f070_TEST FAIL\n"); return 1; }
    std::printf("00c5f070_TEST PASS\n");
    return 0;
}