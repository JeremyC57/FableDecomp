// CImaAdpcmCodec::DecodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR) -- decode stereo IMA ADPCM
// Leaf: ?DecodeS16@CImaAdpcmCodec@@SIHPAE0IIIPAH1@Z
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef int            BOOL;
typedef int            INT;
typedef long           LONG;
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

    static BOOL __fastcall DecodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR);
    static INT __fastcall DecodeSample(INT nInputSample, INT nPredictedSample, INT nStepSize);   // 0x00C5F350
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

BOOL __fastcall CImaAdpcmCodec::DecodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR)
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
    DWORD  dwLeft;
    DWORD  dwRight;
    UINT   i;

    if (!cBlocks)
        return TRUE;

    do
    {
        cBlocks--;
        pbBlock = pbSrc;
        cSamples = cSamplesPerBlock - 1;

        dwLeft = *(LPDWORD)pbBlock;
        pbBlock += sizeof(DWORD);

        nPredSampleL = (SHORT)LOWORD(dwLeft);
        nStepIndexL = LOBYTE(HIWORD(dwLeft));

        if (!ValidStepIndex(nStepIndexL))
            return FALSE;

        dwRight = *(LPDWORD)pbBlock;
        pbBlock += sizeof(DWORD);

        nPredSampleR = (SHORT)LOWORD(dwRight);
        nStepIndexR = LOBYTE(HIWORD(dwRight));

        if (!ValidStepIndex(nStepIndexR))
            return FALSE;

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
    } while (cBlocks);

    return TRUE;
}
