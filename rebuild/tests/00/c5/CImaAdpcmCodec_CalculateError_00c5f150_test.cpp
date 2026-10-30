// Behaviour test for CImaAdpcmCodec::CalculateError (0x00C5F150): per-block squared error, normalised and scaled.
#include <cstdio>
typedef unsigned char BYTE; typedef unsigned short WORD; typedef unsigned long DWORD; typedef int INT; typedef unsigned int UINT;
#ifdef _MSC_VER
typedef unsigned __int64 ULONGLONG; typedef __int64 LONGLONG;
#else
typedef unsigned long long ULONGLONG; typedef long long LONGLONG;
#endif
#pragma pack(push, 1)
struct WAVEFORMATEX { WORD wFormatTag; WORD nChannels; DWORD nSamplesPerSec; DWORD nAvgBytesPerSec; WORD nBlockAlign; WORD wBitsPerSample; WORD cbSize; };
struct IMAADPCMWAVEFORMAT { WAVEFORMATEX wfx; WORD wSamplesPerBlock; };
#pragma pack(pop)
class CImaAdpcmCodec {
public:
    IMAADPCMWAVEFORMAT m_wfxEncode; INT m_fEncoder; INT m_nStepIndexL; INT m_nStepIndexR; void* m_pfnConvert;
    ULONGLONG CalculateError(const BYTE* pbSrc, const BYTE* pbDecoded, UINT cBlocks, UINT cTotalBlocks, UINT cbBlock)
    {
        ULONGLONG nTotal = 0;
        ULONGLONG nScale = (ULONGLONG)-1 / cTotalBlocks;
        INT nSamples = m_wfxEncode.wSamplesPerBlock;
        ULONGLONG nNorm = (LONGLONG)nSamples * ((LONGLONG)1 << 32);
        for (UINT i = 0; i < cBlocks; i++) {
            const short* psSrc = (const short*)(pbSrc + i * cbBlock);
            const short* psDecoded = (const short*)(pbDecoded + i * cbBlock);
            ULONGLONG nSum = 0;
            for (UINT j = 0; j < (UINT)nSamples; j++) {
                LONGLONG nDiff = (LONGLONG)psDecoded[j] - (LONGLONG)psSrc[j];
                nSum += nDiff * nDiff;
            }
            nTotal += (ULONGLONG)(LONGLONG)((double)nSum / (double)nNorm * (double)nScale);
        }
        return nTotal;
    }
};
int main()
{
    CImaAdpcmCodec c; c.m_wfxEncode.wSamplesPerBlock = 4;
    short src[16], dec[16];
    for (int i = 0; i < 16; ++i) { src[i] = (short)(i * 100); dec[i] = src[i]; }
    if (c.CalculateError((BYTE*)src, (BYTE*)dec, 2, 2, 16) != 0) { std::printf("00c5f150_TEST FAIL\n"); return 1; }
    // block 0: one sample off by 0x8000 -> sum 2^30; with 4 samples norm = 2^34: 2^30/2^34 = 1/16; scale = ~0/16 = 2^60 - 1/16
    dec[1] = (short)(src[1] - 0x4000); dec[2] = (short)(src[2] + 0x4000);   // 2 * 2^28 = 2^29
    ULONGLONG r = c.CalculateError((BYTE*)src, (BYTE*)dec, 1, 16, 8);
    double expect = (double)(1u << 29) / 17179869184.0 * (double)(~(ULONGLONG)0 / 16);
    if (r != (ULONGLONG)(LONGLONG)expect || r == 0) { std::printf("00c5f150_TEST FAIL\n"); return 1; }
    // the second block (cbBlock apart) is ignored with cBlocks = 1, counted with cBlocks = 2; samples past wSamplesPerBlock are not read
    dec[5] = (short)(src[5] + 0x4000); dec[7] = (short)(src[7] + 0x7fff - 700);
    if (c.CalculateError((BYTE*)src, (BYTE*)dec, 1, 16, 8) != r) { std::printf("00c5f150_TEST FAIL\n"); return 1; }
    if (c.CalculateError((BYTE*)src, (BYTE*)dec, 2, 16, 8) <= r) { std::printf("00c5f150_TEST FAIL\n"); return 1; }
    std::printf("00c5f150_TEST PASS\n");
    return 0;
}