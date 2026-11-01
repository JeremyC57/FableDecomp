// CImaAdpcmCodec::DecodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR) -- decode stereo IMA ADPCM to interleaved 16-bit PCM block by block: a dword header per channel (first sample, step index in the high word; FALSE on an invalid index), then groups of up to eight samples from one left and one right code dword; the source steps by nBlockAlignment
// Leaf: ?DecodeS16@CImaAdpcmCodec@@SIHPAE0IIIPAH1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5F7F0, 486 bytes (`__fastcall`: pbSrc ecx, pbDst edx, five stack arguments, `ret 14h`).
// Identity: the DirectX/Xbox SDK IMA ADPCM codec (imaadpcm.cpp, class CImaAdpcmCodec; no PDB names -- vendor class, the
//   landed CImaAdpcmCodec rows 0x00C5F000..0x00C5F9E0). CImaAdpcmCodec::Initialize's converter table 0x012B9274 holds
//   {DecodeM16 0x00C5F6A0, DecodeS16 0x00C5F7F0} in its decoder row (this address is referenced only from .rdata 0x012B9278
//   and 0x012B9318), and the body is the SDK's stereo decoder: per channel the header dword's (SHORT)LOWORD prediction and
//   (BYTE)HIWORD step index validated against the 89-entry m_asStep table, then two DecodeSample 0x00C5F350 calls per
//   sample pair with the inline NextStepIndex clamp; pnStepIndexL/R are unused (the decoder reads indices from the
//   headers), as retail never touches the last two argument slots. The catalog label "Audio_ExpandADPCM_Stereo" is
//   descriptive. Mangled spelling follows the landed EncodeS16 0x00C5F4E0 (same seven-parameter static __fastcall).
// Callees: CImaAdpcmCodec::DecodeSample 0x00C5F350 (`__fastcall`, twice). Data: m_asStep 0x012B91C0, m_asNextStep 0x012B91A0.
// Shape: the SDK error path `fSuccess = FALSE; break;` with `return fSuccess;` gives retail's shrink-wrapped zero-block exit
//   (`mov eax,1 ; je` into the shared TRUE tail before the callee-saved pushes); `return FALSE` / an explicit early
//   `return TRUE` do not (probe log). Header read as one DWORD (`mov eax,[edx] ; movsx ; shr 10h ; movzx`).
// Layout: only the two static tables of the class are needed here (no engine header: SDK class, not in the PDB).
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