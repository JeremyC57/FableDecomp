#include <cstdio>
#include <cstring>

// Behaviour test for 0x00C5F3B0 CImaAdpcmCodec::EncodeM16: per block a header dword (first sample low word, step index
// high word), then two codes per byte (first in the low nibble, a lone last sample pairs with 0), the step index
// following m_asNextStep clamped to 0..88; blocks start nBlockAlignment apart; the final step index is written back.
typedef unsigned char BYTE; typedef unsigned short WORD; typedef unsigned int DWORD; typedef int BOOL; typedef int INT;
typedef int LONG;   // 32-bit under both g++ (LP64) and VC7.1
 typedef short SHORT; typedef unsigned int UINT; typedef BYTE* LPBYTE; typedef INT* LPINT;
typedef SHORT* LPSHORT; typedef LONG* LPLONG;
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
};
const SHORT CImaAdpcmCodec::m_asNextStep[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8 };
const SHORT CImaAdpcmCodec::m_asStep[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060,
    1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
    7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 };

int main()
{
    int fails = 0;
    // two blocks of 4 samples (header + 2 bytes), alignment 8 (2 padding bytes)
    SHORT src[8] = { 100, 120, 80, 80, -5, 3000, 3000, -3000 };
    BYTE dst[16]; std::memset(dst, 0xEE, sizeof dst);
    INT stepL = 0, stepR = 77;
    if (CImaAdpcmCodec::EncodeM16((LPBYTE)src, dst, 2, 8, 4, &stepL, &stepR) != 1) ++fails;
    LONG h0; std::memcpy(&h0, dst, 4);
    if ((SHORT)(h0 & 0xffff) != 100 || (h0 >> 16) != 0) ++fails;               // first sample, initial index 0
    // block 0: 120 vs pred 100 step 7 -> code 7 (diff 20 >= 7,3,1), index 8; then 80 -> sign 8 etc.
    if ((dst[4] & 0x0f) != 7) ++fails;
    if (!(dst[4] & 0x80)) ++fails;                                              // second code negative
    if ((dst[5] >> 4) != 0) ++fails;                                            // odd sample count pairs with 0
    if (dst[6] != 0xEE || dst[7] != 0xEE) ++fails;                              // padding untouched
    LONG h1; std::memcpy(&h1, dst + 8, 4);
    if ((SHORT)(h1 & 0xffff) != -5) ++fails;
    INT idxAfter0 = h1 >> 16;
    if (idxAfter0 < 0 || idxAfter0 > 88) ++fails;
    if (stepL < 0 || stepL > 88 || stepR != 77) ++fails;
    // clamp at the top: a long run of large swings saturates at 88
    SHORT big[129]; for (int i = 0; i < 129; ++i) big[i] = (i & 1) ? 32767 : -32768;
    BYTE out[68]; INT s = 80;
    CImaAdpcmCodec::EncodeM16((LPBYTE)big, out, 1, 68, 129, &s, 0);
    if (s != 88) ++fails;
    // and at the bottom: silence walks the index down to 0
    SHORT quiet[9] = { 0 }; BYTE q[8]; s = 3;
    CImaAdpcmCodec::EncodeM16((LPBYTE)quiet, q, 1, 8, 9, &s, 0);
    if (s != 0 || (q[4] & 0x0f) != 0) ++fails;                              // first code: no difference
    // no blocks: index passes through
    s = 42; CImaAdpcmCodec::EncodeM16((LPBYTE)src, dst, 0, 8, 4, &s, 0);
    if (s != 42) ++fails;
    std::printf(fails ? "00c5f3b0_TEST FAIL %d\n" : "00c5f3b0_TEST PASS\n", fails);
    return fails != 0;
}