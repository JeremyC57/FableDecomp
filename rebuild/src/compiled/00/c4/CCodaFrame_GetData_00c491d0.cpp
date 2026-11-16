// CCodaFrame::GetData() -- the frame's PCM sample pointer (+0x10)
// Leaf: ?GetData@CCodaFrame@@QAEPAFXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C491D0: `mov eax,[ecx+10h] ; ret`. Name-fix NF38: was the free stand-in
//   `int __fastcall DrawGetEnvironment(const unsigned char*)` (a propagated label naming another function). CCodaFrame is the
//   libpk frame of libpk/CCodaFrameStreamer.h (CCodaFrame.cpp trace; descriptive member names, no PDB or SDK source): the
//   constructor 0x00C491E0 stores the data pointer at +0x10, and the landed callers 0x00C47F10 / 0x00C49470 already bind
//   ?GetData@CCodaFrame@@QAEPAFXZ (one address, one name).
#define FABLE_RETAIL_CCODAFRAMESTREAMER_PASS23
#include "libpk/CCodaFrameStreamer.h"

short* CCodaFrame::GetData()
{
    return m_pData;
}
