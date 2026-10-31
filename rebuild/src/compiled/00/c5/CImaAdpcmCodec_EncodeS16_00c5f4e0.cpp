// CImaAdpcmCodec::EncodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR) -- encode interleaved stereo 16-bit PCM to IMA ADPCM block by block: a dword header per channel (first sample as the prediction, step index in the high word), then the remaining samples in groups of up to eight, one dword of eight 4-bit left codes followed by one dword of right codes; the destination steps by nBlockAlignment; both running step indices are written back
// Leaf: ?EncodeS16@CImaAdpcmCodec@@SIHPAE0IIIPAH1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5F4E0, 438 bytes (`__fastcall`: pbSrc ecx, pbDst edx, five stack arguments, `ret 14h`).
// Identity: the DirectX/Xbox SDK IMA ADPCM codec (imaadpcm.cpp, class CImaAdpcmCodec), the stereo twin of the landed
//   EncodeM16 0x00C5F3B0: CImaAdpcmCodec::Initialize's converter table 0x012B9274 holds {EncodeM16 0x00C5F3B0, EncodeS16
//   0x00C5F4E0} in its encoder row (the only reference to this address, .rdata 0x012B9280), and the body is the SDK's
//   EncodeS16 (header per channel, groups of min(cSamples, 8) codes packed into one left and one right DWORD). The manifest
//   label "Audio_DecodeADPCM_Stereo" is wrong (it encodes: two EncodeSample calls per sample pair).
// Callees: CImaAdpcmCodec::EncodeSample 0x00C5F2D0 (`__fastcall`, twice). Data: m_asStep 0x012B91C0, m_asNextStep 0x012B91A0.
// Shape: the SDK text on the landed EncodeM16 model (same inline NextStepIndex with the unsigned NUMELMS clamp, step size
//   passed as `m_asStep[nStepIndex]`); VC7.1 homes dwLeft / dwRight in argument slots, as retail does. First grade.
// Layout: CImaAdpcmCodec / IMAADPCMWAVEFORMAT as the landed EncodeM16 / Initialize rows (no engine header: SDK class, not in the PDB).
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
typedef LONG*          LPLONG;
#define NUMELMS(a) (sizeof(a) / sizeof((a)[0]))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define MAKELONG(a, b) ((LONG)(((WORD)((DWORD)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))

#pragma pack(push, 1)                              // mmreg.h / mmsystem.h declare these under pshpack1.h
struct WAVEFORMATEX {                              // 18 bytes
    WORD  wFormatTag;                              // +0x00
    WORD  nChannels;                               // +0x02
    DWORD nSamplesPerSec;                          // +0x04
    DWORD nAvgBytesPerSec;                         // +0x08
    WORD  nBlockAlign;                             // +0x0c
    WORD  wBitsPerSample;                          // +0x0e
    WORD  cbSize;                                  // +0x10
};
struct IMAADPCMWAVEFORMAT {                        // 20 bytes
    WAVEFORMATEX wfx;                              // +0x00
    WORD         wSamplesPerBlock;                 // +0x12
};
#pragma pack(pop)

typedef BOOL (__fastcall* LPFNIMAADPCMCONVERT)(LPBYTE, LPBYTE, UINT, UINT, UINT, LPINT, LPINT);

class CImaAdpcmCodec {                             // 0x24 bytes
protected:
    IMAADPCMWAVEFORMAT  m_wfxEncode;               // +0x00
    BOOL                m_fEncoder;                // +0x14
    INT                 m_nStepIndexL;             // +0x18
    INT                 m_nStepIndexR;             // +0x1c
    LPFNIMAADPCMCONVERT m_pfnConvert;              // +0x20

    static const SHORT m_asNextStep[16];           // 0x012B91A0
    static const SHORT m_asStep[89];               // 0x012B91C0

public:
    static BOOL __fastcall EncodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR);   // 0x00C5F3B0
    static BOOL __fastcall EncodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR);   // 0x00C5F4E0
    static INT __fastcall EncodeSample(INT nInputSample, LPINT pnPredictedSample, INT nStepSize);                 // 0x00C5F2D0
    static INT NextStepIndex(INT nEncodedSample, INT nStepIndex);
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

BOOL __fastcall CImaAdpcmCodec::EncodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR)
{
    LPBYTE pbBlock;
    UINT   cSamples;
    UINT   cSubSamples;
    INT    nSample;
    INT    nEncSample;
    INT    nPredSampleL;
    INT    nPredSampleR;
    INT    nStepIndexL;
    INT    nStepIndexR;
    DWORD  dwLeft;
    DWORD  dwRight;
    UINT   i;

    nStepIndexL = *pnStepIndexL;
    nStepIndexR = *pnStepIndexR;

    while (cBlocks--)
    {
        pbBlock = pbDst;
        cSamples = cSamplesPerBlock - 1;

        nPredSampleL = *(LPSHORT)pbSrc;
        pbSrc += sizeof(SHORT);

        *(LPLONG)pbBlock = MAKELONG(nPredSampleL, nStepIndexL);
        pbBlock += sizeof(LONG);

        nPredSampleR = *(LPSHORT)pbSrc;
        pbSrc += sizeof(SHORT);

        *(LPLONG)pbBlock = MAKELONG(nPredSampleR, nStepIndexR);
        pbBlock += sizeof(LONG);

        while (cSamples)
        {
            dwLeft = 0;
            dwRight = 0;

            cSubSamples = min(cSamples, 8);

            for (i = 0; i < cSubSamples; i++)
            {
                nSample = *(LPSHORT)pbSrc;
                pbSrc += sizeof(SHORT);

                nEncSample = EncodeSample(nSample, &nPredSampleL, m_asStep[nStepIndexL]);
                nStepIndexL = NextStepIndex(nEncSample, nStepIndexL);
                dwLeft |= (DWORD)nEncSample << (4 * i);

                nSample = *(LPSHORT)pbSrc;
                pbSrc += sizeof(SHORT);

                nEncSample = EncodeSample(nSample, &nPredSampleR, m_asStep[nStepIndexR]);
                nStepIndexR = NextStepIndex(nEncSample, nStepIndexR);
                dwRight |= (DWORD)nEncSample << (4 * i);
            }

            *(LPDWORD)pbBlock = dwLeft;
            pbBlock += sizeof(DWORD);

            *(LPDWORD)pbBlock = dwRight;
            pbBlock += sizeof(DWORD);

            cSamples -= cSubSamples;
        }

        pbDst += nBlockAlignment;
    }

    *pnStepIndexL = nStepIndexL;
    *pnStepIndexR = nStepIndexR;

    return 1;
}