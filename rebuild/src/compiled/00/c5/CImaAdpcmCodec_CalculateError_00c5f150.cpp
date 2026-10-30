// CImaAdpcmCodec::CalculateError(const BYTE* pbSrc, const BYTE* pbDecoded, UINT cBlocks, UINT cTotalBlocks, UINT cbBlock) -- the encoder's step-index search metric: for each of cBlocks PCM blocks (cbBlock bytes apart) sum the squared 16-bit sample differences between the source and its decode round trip over wSamplesPerBlock samples, normalise by wSamplesPerBlock * 2^32 and scale by ~0 / cTotalBlocks; returns the 64-bit total
// Leaf: ?CalculateError@CImaAdpcmCodec@@QAE_KPBE0III@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5F150, 381 bytes, thiscall, five stack arguments, `ret 14h`, result in edx:eax.
// Identity: the class is CImaAdpcmCodec (the DirectX/Xbox SDK IMA ADPCM codec, landed rows 0x00C5F000..0x00C5F9E0): the row
//   sits between IsValidImaAdpcmFormat 0x00C5F110 and EncodeSample 0x00C5F2D0, and its only callers (0x00C5FC52, 0x00C5FE27,
//   0x00C5FEF0) are inside the codec's mode-switched convert 0x00C5FA50 (`this` in ebx, m_pfnConvert +0x20, the
//   "Using normal / whole file / per-block encoding" step-index search), which passes the source PCM, the decoded PCM,
//   the block counts and the PCM block size wSamplesPerBlock * nChannels * 2. this+0x12 is IMAADPCMWAVEFORMAT::
//   wSamplesPerBlock. The SDK's public imaadpcm.cpp has no such method, so the name is descriptive (no PDB / RTTI);
//   the manifest label "Audio_ComputeEnergy" is propagated.
// Callees: the VC7.1 CRT helpers the compiler emits itself: _aulldiv 0x00BFEE00, _allmul 0x00BFEB50, _ftol2 0x00BFEA70.
// Shape: both block pointers are written as `base + i * cbBlock` (VC7.1 then keeps one induction pointer and the
//   decoded - source delta, as retail does; two `+= cbBlock` pointers give an extra register and a 0x34 frame). The
//   normaliser and scale are unsigned __int64 (the `and 7fffffffh` / `fchs` unsigned-to-double sequence, hoisted out of
//   the loop); the sample count is an int (`cdq` before _allmul).
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef int            BOOL;
typedef int            INT;
typedef unsigned int   UINT;
typedef BYTE*          LPBYTE;
typedef INT*           LPINT;
typedef unsigned __int64 ULONGLONG;
typedef __int64        LONGLONG;
#pragma pack(push, 1)
struct WAVEFORMATEX { WORD wFormatTag; WORD nChannels; DWORD nSamplesPerSec; DWORD nAvgBytesPerSec; WORD nBlockAlign; WORD wBitsPerSample; WORD cbSize; };
struct IMAADPCMWAVEFORMAT { WAVEFORMATEX wfx; WORD wSamplesPerBlock; };
#pragma pack(pop)
typedef BOOL (__fastcall* LPFNIMAADPCMCONVERT)(LPBYTE, LPBYTE, UINT, UINT, LPINT, LPINT);   // landed model
class CImaAdpcmCodec {
protected:
    IMAADPCMWAVEFORMAT  m_wfxEncode;
    BOOL                m_fEncoder;                // +0x14 (landed name; 0x00C5FA50 switches on it as an encode mode 0..3)
    INT                 m_nStepIndexL;
    INT                 m_nStepIndexR;
    LPFNIMAADPCMCONVERT m_pfnConvert;
public:
    ULONGLONG CalculateError(const BYTE* pbSrc, const BYTE* pbDecoded, UINT cBlocks, UINT cTotalBlocks, UINT cbBlock);
};
ULONGLONG CImaAdpcmCodec::CalculateError(const BYTE* pbSrc, const BYTE* pbDecoded, UINT cBlocks, UINT cTotalBlocks, UINT cbBlock)
{
    ULONGLONG nTotal = 0;
    ULONGLONG nScale = (ULONGLONG)-1 / cTotalBlocks;
    INT nSamples = m_wfxEncode.wSamplesPerBlock;
    ULONGLONG nNorm = (LONGLONG)nSamples * 0x100000000;
    for (UINT i = 0; i < cBlocks; i++)
    {
        const short* psSrc = (const short*)(pbSrc + i * cbBlock);
        const short* psDecoded = (const short*)(pbDecoded + i * cbBlock);
        ULONGLONG nSum = 0;
        for (UINT j = 0; j < (UINT)nSamples; j++)
        {
            LONGLONG nDiff = (LONGLONG)psDecoded[j] - (LONGLONG)psSrc[j];
            nSum += nDiff * nDiff;
        }
        nTotal += (ULONGLONG)((double)nSum / (double)nNorm * (double)nScale);
    }
    return nTotal;
}