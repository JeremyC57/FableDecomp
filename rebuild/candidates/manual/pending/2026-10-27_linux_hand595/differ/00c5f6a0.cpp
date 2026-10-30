// CImaAdpcmCodec::DecodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR) -- decode mono IMA ADPCM blocks to 16-bit PCM: each block's dword header gives the first sample (low word) and the step index (low byte of the high word, rejected when out of range), then two 4-bit codes per byte (low nibble first) are expanded with DecodeSample; the source steps by nBlockAlignment
// Grade: DIFFER(327v327) -- block-head scheduling only: retail reads the dword header first and keeps the cBlocks-- temp in ebx with `lea edx,[esi+4]` before the `jl`; ours puts the cBlocks temp in edi and the lea after the step-index check
// Leaf: ?DecodeM16@CImaAdpcmCodec@@SIHPAE0IIIPAH1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5F6A0, 327 bytes (`__fastcall`: pbSrc ecx, pbDst edx, five stack arguments,
//   `ret 14h`; the step-index pointers are unused by the decoder).
// Identity: the DirectX/Xbox SDK IMA ADPCM codec (imaadpcm.cpp, class CImaAdpcmCodec). The landed Initialize 0x00C5F9E0
//   fills m_pfnConvert from the converter table 0x012B9274 = {DecodeM16 0x00C5F6A0, DecodeS16 0x00C5F7F0},
//   {EncodeM16 0x00C5F3B0, EncodeS16 0x00C5F4E0}; this row is the mono decoder (calls DecodeSample, writes SHORTs,
//   reads nibbles). Signature as the landed EncodeM16 0x00C5F3B0 (the SDK revision with cSamplesPerBlock).
//   The manifest label "Audio_ExpandADPCM_Mono" is descriptive and replaced.
// Callees: CImaAdpcmCodec::DecodeSample 0x00C5F350 (`__fastcall`, twice). Data: m_asStep 0x012B91C0 (89 SHORTs),
//   m_asNextStep 0x012B91A0 (16 SHORTs).
// Shape: the SDK body; ValidStepIndex is `>= 0 && < NUMELMS(m_asStep)` (`test edi,edi ; jl` then `cmp edi,59h ; jae`).
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
#define LOWORD(l) ((WORD)((DWORD)(l) & 0xffff))
#define HIWORD(l) ((WORD)((DWORD)(l) >> 16))

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
    static BOOL __fastcall DecodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR);   // 0x00C5F6A0
    static INT __fastcall DecodeSample(INT nInputSample, INT nPredictedSample, INT nStepSize);                    // 0x00C5F350
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

BOOL __fastcall CImaAdpcmCodec::DecodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, UINT cSamplesPerBlock, LPINT pnStepIndexL, LPINT pnStepIndexR)
{
    BOOL   fSuccess = 1;
    LPBYTE pbBlock;
    UINT   cSamples;
    BYTE   bSample;
    INT    nStepSize;
    INT    nEncSample;
    INT    nPredSample;
    INT    nStepIndex;
    DWORD  dwHeader;

    while (cBlocks--)
    {
        pbBlock = pbSrc;
        cSamples = cSamplesPerBlock - 1;

        dwHeader = *(LPDWORD)pbBlock;
        nPredSample = (SHORT)LOWORD(dwHeader);
        nStepIndex = (BYTE)HIWORD(dwHeader);

        if (!ValidStepIndex(nStepIndex))
        {
            fSuccess = 0;
            break;
        }

        pbBlock += sizeof(DWORD);

        *(LPSHORT)pbDst = (SHORT)nPredSample;
        pbDst += sizeof(SHORT);

        while (cSamples)
        {
            bSample = *pbBlock++;

            nEncSample = (bSample & (BYTE)0x0F);
            nStepSize = m_asStep[nStepIndex];
            nPredSample = DecodeSample(nEncSample, nPredSample, nStepSize);
            nStepIndex = NextStepIndex(nEncSample, nStepIndex);

            *(LPSHORT)pbDst = (SHORT)nPredSample;
            pbDst += sizeof(SHORT);

            cSamples--;

            if (cSamples)
            {
                nEncSample = (bSample >> 4);
                nPredSample = DecodeSample(nEncSample, nPredSample, m_asStep[nStepIndex]);
                nStepIndex = NextStepIndex(nEncSample, nStepIndex);

                *(LPSHORT)pbDst = (SHORT)nPredSample;
                pbDst += sizeof(SHORT);

                cSamples--;
            }
        }

        pbSrc += nBlockAlignment;
    }

    return fSuccess;
}
