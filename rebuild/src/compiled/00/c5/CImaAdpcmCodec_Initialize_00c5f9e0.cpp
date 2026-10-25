// CImaAdpcmCodec::Initialize(const IMAADPCMWAVEFORMAT* pwfxEncode, int fEncoder) -- validate the Xbox ADPCM format, keep a copy, pick the mono/stereo encode/decode converter and reset both step indices; FALSE for an invalid format
// Leaf: ?Initialize@CImaAdpcmCodec
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5F9E0, 103 bytes (`__fastcall`, `ret 8`).
// Callees: IsValidImaAdpcmFormat 0x00C5F110 (declaration only); data: the converter table 0x012B9274 (indexed
//   `[edx*4 + table - 4]`, the `- 1` of the channel count folded into the displacement).
// Identity: the manifest label is propagated. The audio library carries the DirectX/Xbox SDK IMA ADPCM codec
//   (imaadpcm.cpp, class CImaAdpcmCodec): CalculateEncodeAlignment 0x00C5F000, CreatePcmFormat 0x00C5F030,
//   CreateImaAdpcmFormat 0x00C5F070, IsValidImaAdpcmFormat 0x00C5F110, EncodeSample 0x00C5F2D0, DecodeSample 0x00C5F350,
//   Initialize 0x00C5F9E0 (its converter table 0x012B9274 = {DecodeM16 0x00C5F6A0, DecodeS16 0x00C5F7F0},
//   {EncodeM16 0x00C5F3B0, EncodeS16 0x00C5F4E0}); the encoder driver 0x00C425E0..0x00C4264F ("Step index chosen: %d")
//   calls CreateImaAdpcmFormat and Initialize. The format tag 0x69 is WAVE_FORMAT_XBOX_ADPCM. The static members are
//   `__fastcall` (the library is built /Gr). Local model: the SDK class and the pshpack1 mmsystem structs.
// Shape: the flag is stored as passed and normalised only for the index (`cmp eax,ecx ; setne dl`); the 20-byte format
//   copy goes through `mov eax,esi`; `m_nStepIndexL = m_nStepIndexR = 0` stores +0x1c before +0x18.
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef int            BOOL;
typedef int            INT;
typedef long           LONG;
typedef unsigned int   UINT;
typedef BYTE*          LPBYTE;
typedef INT*           LPINT;

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
typedef WAVEFORMATEX*             LPWAVEFORMATEX;
typedef IMAADPCMWAVEFORMAT*       LPIMAADPCMWAVEFORMAT;
typedef const IMAADPCMWAVEFORMAT* LPCIMAADPCMWAVEFORMAT;

typedef BOOL (__fastcall* LPFNIMAADPCMCONVERT)(LPBYTE, LPBYTE, UINT, UINT, LPINT, LPINT);

class CImaAdpcmCodec {                             // 0x24 bytes
protected:
    IMAADPCMWAVEFORMAT  m_wfxEncode;               // +0x00
    BOOL                m_fEncoder;                // +0x14
    INT                 m_nStepIndexL;             // +0x18
    INT                 m_nStepIndexR;             // +0x1c
    LPFNIMAADPCMCONVERT m_pfnConvert;              // +0x20
public:
    BOOL Initialize(LPCIMAADPCMWAVEFORMAT pwfxEncode, BOOL fEncoder);
    static void __fastcall CreatePcmFormat(WORD nChannels, DWORD nSamplesPerSec, LPWAVEFORMATEX pwfx);
    static void __fastcall CreateImaAdpcmFormat(WORD nChannels, DWORD nSamplesPerSec, WORD nSamplesPerBlock, LPIMAADPCMWAVEFORMAT pwfx);
    static BOOL __fastcall IsValidImaAdpcmFormat(LPCIMAADPCMWAVEFORMAT pwfx);
    static WORD __fastcall CalculateEncodeAlignment(WORD nChannels, WORD nSamplesPerBlock);   // 0x00C5F000
    static BOOL __fastcall EncodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, LPINT pnStepIndexL, LPINT pnStepIndexR);   // 0x00C5F3B0
    static BOOL __fastcall EncodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, LPINT pnStepIndexL, LPINT pnStepIndexR);   // 0x00C5F4E0
    static BOOL __fastcall DecodeM16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, LPINT pnStepIndexL, LPINT pnStepIndexR);   // 0x00C5F6A0
    static BOOL __fastcall DecodeS16(LPBYTE pbSrc, LPBYTE pbDst, UINT cBlocks, UINT nBlockAlignment, LPINT pnStepIndexL, LPINT pnStepIndexR);   // 0x00C5F7F0
    static INT __fastcall EncodeSample(INT nInputSample, LPINT pnPredictedSample, INT nStepSize);
    static INT __fastcall DecodeSample(INT nInputSample, INT nPredictedSample, INT nStepSize);
};

BOOL CImaAdpcmCodec::Initialize(LPCIMAADPCMWAVEFORMAT pwfxEncode, BOOL fEncoder)
{
    static const LPFNIMAADPCMCONVERT apfnConverter[2][2] =
    {
        { DecodeM16, DecodeS16 },
        { EncodeM16, EncodeS16 }
    };

    if (!IsValidImaAdpcmFormat(pwfxEncode))
        return 0;

    m_wfxEncode = *pwfxEncode;
    m_fEncoder = fEncoder;

    m_pfnConvert = apfnConverter[!!fEncoder][m_wfxEncode.wfx.nChannels - 1];

    m_nStepIndexL = m_nStepIndexR = 0;

    return 1;
}