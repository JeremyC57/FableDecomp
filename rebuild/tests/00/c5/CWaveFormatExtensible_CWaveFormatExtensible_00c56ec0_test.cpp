#include <cstdio>
#include <cstring>
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
static const GUID KSDATAFORMAT_SUBTYPE_PCM = { 0x00000001, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };
struct CWaveFormatExtensible {
    unsigned short wFormatTag; unsigned short nChannels; unsigned int nSamplesPerSec; unsigned int nAvgBytesPerSec;
    unsigned short nBlockAlign; unsigned short wBitsPerSample; unsigned short cbSize; unsigned short wValidBitsPerSample;
    unsigned int dwChannelMask; GUID SubFormat;
    CWaveFormatExtensible(unsigned int rate, unsigned short channels, unsigned short bits)
    {
        wFormatTag = 0xFFFE; nChannels = channels; nSamplesPerSec = rate; wBitsPerSample = bits; cbSize = 22;
        nBlockAlign = channels * bits / 8;
        nAvgBytesPerSec = nBlockAlign * rate;
        wValidBitsPerSample = bits;
        if (channels == 2) dwChannelMask = 3;
        else if (channels == 4) dwChannelMask = 0x33;
        else if (channels == 6) dwChannelMask = 0x3F;
        SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c56ec0_TEST FAIL\n"); return 1; } } while (0)
int main()
{
    CHECK(sizeof(CWaveFormatExtensible) == 0x28);
    CWaveFormatExtensible a(44100, 2, 16);
    CHECK(a.wFormatTag == 0xFFFE && a.nChannels == 2 && a.nSamplesPerSec == 44100 && a.nBlockAlign == 4 && a.nAvgBytesPerSec == 176400);
    CHECK(a.wBitsPerSample == 16 && a.cbSize == 22 && a.wValidBitsPerSample == 16 && a.dwChannelMask == 3);
    CHECK(std::memcmp(&a.SubFormat, &KSDATAFORMAT_SUBTYPE_PCM, 16) == 0);
    CWaveFormatExtensible b(22050, 6, 16);
    CHECK(b.dwChannelMask == 0x3F && b.nBlockAlign == 12 && b.nAvgBytesPerSec == 264600);
    CWaveFormatExtensible c(48000, 4, 8);
    CHECK(c.dwChannelMask == 0x33 && c.nBlockAlign == 4);
    std::printf("00c56ec0_TEST PASS\n");
    return 0;
}