// CCodaFrame::Silence() -- fill the frame's whole 16-bit sample block (channels x samples) with zeroes and mark every sample valid
// Leaf: ?Silence@CCodaFrame@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49180, 40 bytes (+ int3 padding), __fastcall, plain `ret`.
// Library: libpk audio (no PDB/RTTI): descriptive names. The frame of .\CFrame.cpp: constructor 0x00C491E0
//   (channels +0, samples +4, "Expecting at least one channel!" / "Expecting frame to contain at least one sample!"),
//   GetData 0x00C491D0 (+0x10), FreeData 0x00C491B0 (`delete[]` of +0x10). Only caller: 0x00C38F4F, which silences
//   every frame of a streamer before handing it back. The manifest's "CTCActivationReceptor_ClearMemory" is propagated.
// Shape: `this` moves to edx for the intrinsic memset (`rep stosd` + `rep stosb`, the byte count kept in esi);
//   channels * samples * sizeof(short) gives `imul ; shl ecx,1`; the sample count is re-read after the fill.
#include <string.h>
#pragma intrinsic(memset)

struct CCodaFrame {
    unsigned long m_channels;                   // +0x00
    unsigned long m_samples;                    // +0x04 capacity in samples per channel
    unsigned long m_validSamples;               // +0x08
    unsigned long m_position;                   // +0x0c
    short*        m_pData;                      // +0x10 interleaved 16-bit PCM
    void Silence();
};

void CCodaFrame::Silence()
{
    memset(m_pData, 0, m_channels * m_samples * sizeof(short));
    m_validSamples = m_samples;
}