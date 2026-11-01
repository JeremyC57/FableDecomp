// Behaviour test for CImaAdpcmCodec::DecodeS16 0x00C5F7F0 (standalone copy of the SDK body with the SDK tables and DecodeSample).
#include <cstdio>
#include <cstring>
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef int            BOOL;
typedef int            INT;
typedef int            LONG;
typedef short          SHORT;
typedef unsigned int   UINT;
typedef BYTE*          LPBYTE;
typedef INT*           LPINT;
typedef SHORT*         LPSHORT;
typedef DWORD*         LPDWORD;
#define NUMELMS(a) (sizeof(a) / sizeof((a)[0]))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define MAKELONG(a, b) ((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define FALSE 0
#define TRUE 1

class CImaAdpcmCodec {
public:
    static const SHORT m_asNextStep[16];           // 0x012B91A0
    static const SHORT m_asStep[89];               // 0x012B91C0

    static BOOL DecodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR);
    static INT DecodeSample(INT nInputSample, INT nPredictedSample, INT nStepSize);   // 0x00C5F350
    static INT NextStepIndex(INT nEncodedSample, INT nStepIndex);
    static BOOL ValidStepIndex(INT nStepIndex);
};

inline INT CImaAdpcmCodec::NextStepIndex(INT nEncodedSample, INT nStepIndex)
{
    nStepIndex += m_asNextStep[nEncodedSample];

    if (nStepIndex < 0)
        nStepIndex = 0;
    else if (nStepIndex >= NUMELMS(m_asStep))
        nStepIndex = NUMELMS(m_asStep) - 1;

    return nStepIndex;
}

inline BOOL CImaAdpcmCodec::ValidStepIndex(INT nStepIndex)
{
    return (nStepIndex >= 0) && (nStepIndex < NUMELMS(m_asStep));
}

BOOL CImaAdpcmCodec::DecodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR)
{
    LPBYTE pbBlock;
    UINT   cSamples;
    UINT   cSubSamples;
    INT    nEncSampleL;
    INT    nPredSampleL;
    INT    nStepIndexL;
    INT    nEncSampleR;
    INT    nPredSampleR;
    INT    nStepIndexR;
    DWORD  dwHeader;
    DWORD  dwLeft;
    DWORD  dwRight;
    UINT   i;

    BOOL fSuccess = TRUE;

    while (cBlocks--)
    {
        pbBlock = pbSrc;
        cSamples = cSamplesPerBlock - 1;

        dwHeader = *(LPDWORD)pbBlock;
        pbBlock += sizeof(DWORD);

        nPredSampleL = (INT)(SHORT)LOWORD(dwHeader);
        nStepIndexL = (INT)(BYTE)HIWORD(dwHeader);

        if (!ValidStepIndex(nStepIndexL))
        {
            fSuccess = FALSE;
            break;
        }

        dwHeader = *(LPDWORD)pbBlock;
        pbBlock += sizeof(DWORD);

        nPredSampleR = (INT)(SHORT)LOWORD(dwHeader);
        nStepIndexR = (INT)(BYTE)HIWORD(dwHeader);

        if (!ValidStepIndex(nStepIndexR))
        {
            fSuccess = FALSE;
            break;
        }

        *(LPDWORD)pbDst = MAKELONG(nPredSampleL, nPredSampleR);
        pbDst += sizeof(DWORD);

        while (cSamples)
        {
            dwLeft = *(LPDWORD)pbBlock;
            pbBlock += sizeof(DWORD);

            dwRight = *(LPDWORD)pbBlock;
            pbBlock += sizeof(DWORD);

            cSubSamples = min(cSamples, 8);

            for (i = 0; i < cSubSamples; i++)
            {
                nEncSampleL = (dwLeft & 0x0F);
                nPredSampleL = DecodeSample(nEncSampleL, nPredSampleL, m_asStep[nStepIndexL]);
                nStepIndexL = NextStepIndex(nEncSampleL, nStepIndexL);

                nEncSampleR = (dwRight & 0x0F);
                nPredSampleR = DecodeSample(nEncSampleR, nPredSampleR, m_asStep[nStepIndexR]);
                nStepIndexR = NextStepIndex(nEncSampleR, nStepIndexR);

                *(LPDWORD)pbDst = MAKELONG(nPredSampleL, nPredSampleR);
                pbDst += sizeof(DWORD);

                dwLeft >>= 4;
                dwRight >>= 4;
            }

            cSamples -= cSubSamples;
        }

        pbSrc += nBlockAlignment;
    }

    return fSuccess;
}

const SHORT CImaAdpcmCodec::m_asNextStep[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8 };
const SHORT CImaAdpcmCodec::m_asStep[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060,
    1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
    7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 };
INT CImaAdpcmCodec::DecodeSample(INT nEncodedSample, INT nPredictedSample, INT nStepSize)
{
    LONG lDifference = nStepSize >> 3;
    if (nEncodedSample & 4) lDifference += nStepSize;
    if (nEncodedSample & 2) lDifference += nStepSize >> 1;
    if (nEncodedSample & 1) lDifference += nStepSize >> 2;
    if (nEncodedSample & 8) lDifference = -lDifference;
    LONG lNewSample = nPredictedSample + lDifference;
    if ((LONG)(SHORT)lNewSample != lNewSample) lNewSample = (lNewSample < -32768) ? -32768 : 32767;
    return (INT)lNewSample;
}
static DWORD hdr(int sample, int index) { return ((DWORD)(unsigned short)(short)sample) | ((DWORD)index << 16); }
int main()
{
    BYTE src[16]; DWORD out[4];
    DWORD w[4] = { hdr(100, 0), hdr(-50, 0), 0x04u, 0x1Cu };
    std::memcpy(src, w, 16);
    std::memset(out, 0, sizeof out);
    int a = 0, b = 0;
    bool ok = CImaAdpcmCodec::DecodeS16(src, (LPBYTE)out, 1, 16, 3, &a, &b) == TRUE;
    short o[6]; std::memcpy(o, out, 12);
    if (!ok || o[0] != 100 || o[1] != -50 || o[2] != 107 || o[3] != -57 || o[4] != 108 || o[5] != -54 || out[3] != 0) ok = false;
    if (CImaAdpcmCodec::DecodeS16(src, (LPBYTE)out, 0, 16, 3, &a, &b) != TRUE) ok = false;
    w[1] = hdr(5, 89); std::memcpy(src, w, 16);
    if (CImaAdpcmCodec::DecodeS16(src, (LPBYTE)out, 1, 16, 3, &a, &b) != FALSE) ok = false;
    if (!ok) { std::printf("00c5f7f0_TEST FAIL\n"); return 1; }
    std::printf("00c5f7f0_TEST PASS\n");
    return 0;
}