#include <cstdio>
#include <cstring>

// Behaviour test for 0x00C5F4E0 CImaAdpcmCodec::EncodeS16: per block a left and a right header dword, then groups of up to
// eight samples as one dword of left codes (sample i in bits 4i) followed by one dword of right codes. Each channel must
// produce exactly the code stream the mono encoder (EncodeM16, landed 0x00C5F3B0) produces for that channel alone.
typedef unsigned char BYTE; typedef unsigned short WORD; typedef unsigned int DWORD; typedef int BOOL; typedef int INT;
typedef int LONG;   // 32-bit under both g++ (LP64) and VC7.1
 typedef short SHORT; typedef unsigned int UINT; typedef BYTE* LPBYTE; typedef INT* LPINT;
typedef SHORT* LPSHORT; typedef LONG* LPLONG; typedef DWORD* LPDWORD;
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define NUMELMS(a) (sizeof(a) / sizeof((a)[0]))
#define MAKELONG(a, b) ((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))

struct CImaAdpcmCodec {
    static const SHORT m_asNextStep[16];
    static const SHORT m_asStep[89];
    static INT EncodeSample(INT nInputSample, LPINT pnPredictedSample, INT nStepSize)
    {
        INT nPredictedSample = *pnPredictedSample; LONG lDifference = nInputSample - nPredictedSample; INT nEncodedSample = 0;
        if (lDifference < 0) { nEncodedSample = 8; lDifference = -lDifference; }
        if (lDifference >= nStepSize) { nEncodedSample |= 4; lDifference -= nStepSize; }
        nStepSize >>= 1;
        if (lDifference >= nStepSize) { nEncodedSample |= 2; lDifference -= nStepSize; }
        nStepSize >>= 1;
        if (lDifference >= nStepSize) { nEncodedSample |= 1; lDifference -= nStepSize; }
        if (nEncodedSample & 8) nPredictedSample = nInputSample + lDifference - (nStepSize >> 1);
        else nPredictedSample = nInputSample - lDifference + (nStepSize >> 1);
        if (nPredictedSample > 32767) nPredictedSample = 32767; else if (nPredictedSample < -32768) nPredictedSample = -32768;
        *pnPredictedSample = nPredictedSample;
        return nEncodedSample;
    }
    static INT NextStepIndex(INT nEncodedSample, INT nStepIndex)
    {
        nStepIndex += m_asNextStep[nEncodedSample];
        if (nStepIndex < 0) nStepIndex = 0;
        else if (nStepIndex >= NUMELMS(m_asStep)) nStepIndex = NUMELMS(m_asStep) - 1;
        return nStepIndex;
    }
    static BOOL EncodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT)
    {
        LPBYTE pbBlock; UINT cSamples; INT nSample; INT nEncSample1; INT nEncSample2; INT nPredSample; INT nStepIndex;
        nStepIndex = *pnStepIndexL;
        while (cBlocks--) {
            pbBlock = pbDst;
            cSamples = cSamplesPerBlock - 1;
            nPredSample = *(LPSHORT)pbSrc;
            pbSrc += sizeof(SHORT);
            *(LPLONG)pbBlock = MAKELONG(nPredSample, nStepIndex);
            pbBlock += sizeof(LONG);
            while (cSamples) {
                nSample = *(LPSHORT)pbSrc; pbSrc += sizeof(SHORT); cSamples--;
                nEncSample1 = EncodeSample(nSample, &nPredSample, m_asStep[nStepIndex]);
                nStepIndex = NextStepIndex(nEncSample1, nStepIndex);
                if (cSamples) {
                    nSample = *(LPSHORT)pbSrc; pbSrc += sizeof(SHORT); cSamples--;
                    nEncSample2 = EncodeSample(nSample, &nPredSample, m_asStep[nStepIndex]);
                    nStepIndex = NextStepIndex(nEncSample2, nStepIndex);
                } else {
                    nEncSample2 = 0;
                }
                *pbBlock++ = (BYTE)(nEncSample1 | (nEncSample2 << 4));
            }
            pbDst += nBlockAlignment;
        }
        *pnStepIndexL = nStepIndex;
        return 1;
    }
    static BOOL EncodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR)
    {
        LPBYTE pbBlock; UINT cSamples; UINT cSubSamples; INT nSample; INT nEncSample; INT nPredSampleL; INT nPredSampleR;
        INT nStepIndexL; INT nStepIndexR; DWORD dwLeft; DWORD dwRight; UINT i;
        nStepIndexL = *pnStepIndexL;
        nStepIndexR = *pnStepIndexR;
        while (cBlocks--) {
            pbBlock = pbDst;
            cSamples = cSamplesPerBlock - 1;
            nPredSampleL = *(LPSHORT)pbSrc; pbSrc += sizeof(SHORT);
            *(LPLONG)pbBlock = MAKELONG(nPredSampleL, nStepIndexL); pbBlock += sizeof(LONG);
            nPredSampleR = *(LPSHORT)pbSrc; pbSrc += sizeof(SHORT);
            *(LPLONG)pbBlock = MAKELONG(nPredSampleR, nStepIndexR); pbBlock += sizeof(LONG);
            while (cSamples) {
                dwLeft = 0; dwRight = 0;
                cSubSamples = min(cSamples, 8);
                for (i = 0; i < cSubSamples; i++) {
                    nSample = *(LPSHORT)pbSrc; pbSrc += sizeof(SHORT);
                    nEncSample = EncodeSample(nSample, &nPredSampleL, m_asStep[nStepIndexL]);
                    nStepIndexL = NextStepIndex(nEncSample, nStepIndexL);
                    dwLeft |= (DWORD)nEncSample << (4 * i);
                    nSample = *(LPSHORT)pbSrc; pbSrc += sizeof(SHORT);
                    nEncSample = EncodeSample(nSample, &nPredSampleR, m_asStep[nStepIndexR]);
                    nStepIndexR = NextStepIndex(nEncSample, nStepIndexR);
                    dwRight |= (DWORD)nEncSample << (4 * i);
                }
                *(LPDWORD)pbBlock = dwLeft; pbBlock += sizeof(DWORD);
                *(LPDWORD)pbBlock = dwRight; pbBlock += sizeof(DWORD);
                cSamples -= cSubSamples;
            }
            pbDst += nBlockAlignment;
        }
        *pnStepIndexL = nStepIndexL;
        *pnStepIndexR = nStepIndexR;
        return 1;
    }
};
const SHORT CImaAdpcmCodec::m_asNextStep[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8 };
const SHORT CImaAdpcmCodec::m_asStep[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060,
    1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
    7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 };

static int check(int nSamples, int startL, int startR)
{
    /* one stereo block of nSamples per channel against two mono encodes of the separated channels */
    SHORT st[2 * 40]; SHORT l[40]; SHORT r[40];
    for (int i = 0; i < nSamples; ++i) {
        l[i] = (SHORT)((i * 2711) % 20000 - 9000 + (i & 1) * 400);
        r[i] = (SHORT)(((i * 977) % 3000) * ((i & 2) ? -1 : 1));
        st[2 * i] = l[i]; st[2 * i + 1] = r[i];
    }
    BYTE out[64]; std::memset(out, 0xEE, sizeof out);
    BYTE ml[32]; BYTE mr[32]; std::memset(ml, 0, sizeof ml); std::memset(mr, 0, sizeof mr);
    INT sL = startL, sR = startR, mL = startL, mR = startR, unused = 0;
    if (CImaAdpcmCodec::EncodeS16((LPBYTE)st, out, 1, 64, nSamples, &sL, &sR) != 1) return 0;
    CImaAdpcmCodec::EncodeM16((LPBYTE)l, ml, 1, 32, nSamples, &mL, &unused);
    CImaAdpcmCodec::EncodeM16((LPBYTE)r, mr, 1, 32, nSamples, &mR, &unused);
    if (sL != mL || sR != mR) return 0;
    if (std::memcmp(out, ml, 4) != 0 || std::memcmp(out + 4, mr, 4) != 0) return 0;   /* headers */
    int codes = nSamples - 1, groups = (codes + 7) / 8;
    for (int g = 0; g < groups; ++g) {
        BYTE wantL[4] = { 0, 0, 0, 0 }, wantR[4] = { 0, 0, 0, 0 };
        int n = codes - 8 * g; if (n > 8) n = 8;
        for (int k = 0; k < (n + 1) / 2; ++k) { wantL[k] = ml[4 + 4 * g + k]; wantR[k] = mr[4 + 4 * g + k]; }
        if (std::memcmp(out + 8 + 8 * g, wantL, 4) != 0) return 0;
        if (std::memcmp(out + 12 + 8 * g, wantR, 4) != 0) return 0;
    }
    if (out[8 + 8 * groups] != 0xEE) return 0;                                       /* nothing past the block */
    return 1;
}
int main()
{
    int fails = 0;
    if (!check(17, 0, 0)) fails |= 1;          /* two full groups of eight */
    if (!check(12, 10, 60)) fails |= 2;        /* a full group plus three */
    if (!check(2, 88, 0)) fails |= 4;          /* one code per channel */
    if (!check(1, 5, 6)) fails |= 8;           /* header only */
    /* two blocks: the second starts nBlockAlignment later and continues both step indices */
    SHORT st[2 * 18]; for (int i = 0; i < 36; ++i) st[i] = (SHORT)((i * 1234) % 5000 - 2500);
    BYTE out[80]; std::memset(out, 0xEE, sizeof out);
    INT sL = 0, sR = 0;
    CImaAdpcmCodec::EncodeS16((LPBYTE)st, out, 2, 40, 9, &sL, &sR);
    LONG h2; std::memcpy(&h2, out + 40, 4);
    if ((SHORT)(h2 & 0xffff) != st[18]) fails |= 16;
    if (out[24] != 0xEE || out[39] != 0xEE) fails |= 32;                               /* block padding untouched */
    /* no blocks: indices pass through */
    sL = 42; sR = 43; CImaAdpcmCodec::EncodeS16((LPBYTE)st, out, 0, 40, 9, &sL, &sR);
    if (sL != 42 || sR != 43) fails |= 64;
    std::printf(fails ? "00c5f4e0_TEST FAIL %d\n" : "00c5f4e0_TEST PASS\n", fails);
    return fails != 0;
}