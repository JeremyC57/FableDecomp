// behaviour test for 0x00C5F9E0 CImaAdpcmCodec::Initialize: validation, format copy, converter selection
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

typedef int (*LPFNCONVERT)(int);
static int DecodeM16(int) { return 1; }
static int DecodeS16(int) { return 2; }
static int EncodeM16(int) { return 3; }
static int EncodeS16(int) { return 4; }

static BOOL IsValidImaAdpcmFormat(const IMAADPCMWAVEFORMAT* pwfx)
{
    if (0x69 != pwfx->wfx.wFormatTag) return 0;
    if (sizeof(*pwfx) - sizeof(pwfx->wfx) != pwfx->wfx.cbSize) return 0;
    if ((pwfx->wfx.nChannels < 1) || (pwfx->wfx.nChannels > 2)) return 0;
    if (4 != pwfx->wfx.wBitsPerSample) return 0;
    if (CalculateEncodeAlignment(pwfx->wfx.nChannels, pwfx->wSamplesPerBlock) != pwfx->wfx.nBlockAlign) return 0;
    return 1;
}

class CImaAdpcmCodec {
public:
    IMAADPCMWAVEFORMAT m_wfxEncode;
    BOOL               m_fEncoder;
    INT                m_nStepIndexL;
    INT                m_nStepIndexR;
    LPFNCONVERT        m_pfnConvert;
    BOOL Initialize(const IMAADPCMWAVEFORMAT* pwfxEncode, BOOL fEncoder)
    {
        static const LPFNCONVERT apfnConverter[2][2] = { { DecodeM16, DecodeS16 }, { EncodeM16, EncodeS16 } };
        if (!IsValidImaAdpcmFormat(pwfxEncode))
            return 0;
        m_wfxEncode = *pwfxEncode;
        m_fEncoder = fEncoder;
        m_pfnConvert = apfnConverter[!!fEncoder][m_wfxEncode.wfx.nChannels - 1];
        m_nStepIndexL = m_nStepIndexR = 0;
        return 1;
    }
};

int main()
{
    IMAADPCMWAVEFORMAT f;
    std::memset(&f, 0, sizeof(f));
    f.wfx.wFormatTag = 0x69; f.wfx.nChannels = 2; f.wfx.nSamplesPerSec = 44100; f.wfx.nBlockAlign = 72;
    f.wfx.wBitsPerSample = 4; f.wfx.cbSize = 2; f.wSamplesPerBlock = 64;
    CImaAdpcmCodec c;
    c.m_nStepIndexL = 5; c.m_nStepIndexR = 6; c.m_pfnConvert = 0; c.m_fEncoder = 9;
    if (c.Initialize(&f, 7) != 1) { std::printf("00c5f9e0_TEST FAIL\n"); return 1; }
    if (c.m_fEncoder != 7 || c.m_nStepIndexL != 0 || c.m_nStepIndexR != 0) { std::printf("00c5f9e0_TEST FAIL\n"); return 1; }
    if (c.m_pfnConvert(0) != 4 || std::memcmp(&c.m_wfxEncode, &f, sizeof(f)) != 0) { std::printf("00c5f9e0_TEST FAIL\n"); return 1; }
    f.wfx.nChannels = 1; f.wfx.nBlockAlign = 36;
    if (c.Initialize(&f, 0) != 1 || c.m_pfnConvert(0) != 1 || c.m_fEncoder != 0) { std::printf("00c5f9e0_TEST FAIL\n"); return 1; }
    f.wfx.nBlockAlign = 40;
    c.m_nStepIndexL = 3;
    if (c.Initialize(&f, 1) != 0 || c.m_nStepIndexL != 3 || c.m_pfnConvert(0) != 1) { std::printf("00c5f9e0_TEST FAIL\n"); return 1; }   // invalid: untouched
    std::printf("00c5f9e0_TEST PASS\n");
    return 0;
}