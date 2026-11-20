// CImaAdpcmCodec::CalcDifference(LPBYTE pbSrc, LPBYTE pbDecoded, UINT cBlocks, UINT cTotalBlocks, DWORD cbBlock) -- the encoder's step-index search metric: for each of cBlocks PCM blocks (cbBlock bytes apart) sum the squared 16-bit sample differences between the source and its decode round trip over wSamplesPerBlock samples, normalise by wSamplesPerBlock * 2^32 and scale by ~0 / cTotalBlocks; returns the 64-bit total
// Leaf: ?CalcDifference@CImaAdpcmCodec@@AAE_KPAE0IIK@Z
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
// NF176 (2026-11-20, q4181): renamed from the descriptive CalculateError onto the shared engine/CImaAdpcmCodec.h (Microsoft 2001
//   imaadpcm.h: private `ULONGLONG CalcDifference(LPBYTE, LPBYTE, UINT, UINT, DWORD)`); second source: the caller 0x00C5FA50 (landed
//   RELOCATION_MATCH on the same header) calls it at the three step-index-search sites with the SDK arguments and prints its result
//   with the SDK string "Difference between original and decoded streams: 0x%I64x". The local codec model is gone. Body unchanged.
#include "engine/CImaAdpcmCodec.h"
typedef __int64 LONGLONG;                                   // winnt.h (the SDK source gets it from windows.h)

ULONGLONG CImaAdpcmCodec::CalcDifference(LPBYTE pbSrc, LPBYTE pbDecoded, UINT cBlocks, UINT cTotalBlocks, DWORD cbBlock)
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