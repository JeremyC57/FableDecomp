// CImaAdpcmCodec::EncodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR) -- encode mono 16-bit PCM to IMA ADPCM block by block: each block starts with a dword header (first sample as the prediction, step index in the high word), then packs two 4-bit codes per byte (first sample in the low nibble) for the remaining cSamplesPerBlock - 1 samples; the destination steps by nBlockAlignment; the running step index is written back
// Leaf: ?EncodeM16@CImaAdpcmCodec@@SIHPAE0IIIPAH1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5F3B0, 300 bytes (`__fastcall`: pbSrc ecx, pbDst edx, five stack arguments,
//   `ret 14h`; pnStepIndexR is unused by the mono path).
// Identity: the DirectX/Xbox SDK IMA ADPCM codec (imaadpcm.cpp, class CImaAdpcmCodec; see the landed EncodeSample
//   0x00C5F2D0 / Initialize 0x00C5F9E0): Initialize's converter table 0x012B9274 holds {EncodeM16 0x00C5F3B0, EncodeS16
//   0x00C5F4E0} in the encoder row. This is the SDK revision with the cSamplesPerBlock argument (five stack arguments,
//   the third read as the per-block sample count minus one; the landed model's six-argument converter type predates it).
//   The manifest label "Audio_DecodeADPCM_Mono" is wrong (it encodes: EncodeSample calls, nibble packing).
// Callees: CImaAdpcmCodec::EncodeSample 0x00C5F2D0 (`__fastcall`, twice). Data: m_asStep 0x012B91C0 (89 SHORTs),
//   m_asNextStep 0x012B91A0 (16 SHORTs).
// Shape: the SDK body with the inline NextStepIndex clamp; the upper bound is the unsigned NUMELMS(m_asStep) compare
//   (`cmp esi,59h ; jb`, a signed `> 88` gives `cmp 58h ; jle`); the step size is passed as a temporary
//   (`m_asStep[nStepIndex]` in the call), not a shared nStepSize local, which is what puts the first call's step in edx
//   and lets the two exits share one epilogue. The prediction lives in a stack slot for EncodeSample's pointer. Grades: 3.
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
typedef LONG*          LPLONG;
#define NUMELMS(a) (sizeof(a) / sizeof((a)[0]))
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

BOOL __fastcall CImaAdpcmCodec::EncodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR)
{
    LPBYTE pbBlock;
    UINT   cSamples;
    INT    nSample;
    INT    nEncSample1;
    INT    nEncSample2;
    INT    nPredSample;
    INT    nStepIndex;

    nStepIndex = *pnStepIndexL;

    while (cBlocks--)
    {
        pbBlock = pbDst;
        cSamples = cSamplesPerBlock - 1;

        nPredSample = *(LPSHORT)pbSrc;
        pbSrc += sizeof(SHORT);

        *(LPLONG)pbBlock = MAKELONG(nPredSample, nStepIndex);
        pbBlock += sizeof(LONG);

        while (cSamples)
        {
            nSample = *(LPSHORT)pbSrc;
            pbSrc += sizeof(SHORT);
            cSamples--;

            nEncSample1 = EncodeSample(nSample, &nPredSample, m_asStep[nStepIndex]);
            nStepIndex = NextStepIndex(nEncSample1, nStepIndex);

            if (cSamples)
            {
                nSample = *(LPSHORT)pbSrc;
                pbSrc += sizeof(SHORT);
                cSamples--;

                nEncSample2 = EncodeSample(nSample, &nPredSample, m_asStep[nStepIndex]);
                nStepIndex = NextStepIndex(nEncSample2, nStepIndex);
            }
            else
            {
                nEncSample2 = 0;
            }

            *pbBlock++ = (BYTE)(nEncSample1 | (nEncSample2 << 4));
        }

        pbDst += nBlockAlignment;
    }

    *pnStepIndexL = nStepIndex;

    return 1;
}