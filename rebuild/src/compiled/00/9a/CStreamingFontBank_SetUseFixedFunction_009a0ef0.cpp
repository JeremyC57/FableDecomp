// CStreamingFontBank::SetUseFixedFunction(bool value) -- store the static UseFixedFunction flag @ 0x013C5A7C (static, __fastcall)
// Leaf: ?SetUseFixedFunction@CStreamingFontBank@@SIX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A0EF0, 7 bytes.
// Retail bytes:  mov byte ptr [0x13c5a7c], cl ; ret
// Identity: Ego_r.pdb 0x00AEC210 / FableWin.pdb 0x02FB9AF0 ?SetUseFixedFunction@CStreamingFontBank@@SIX_N@Z; the stored byte is the
//   protected static ?UseFixedFunction@CStreamingFontBank@@1_NA (queue claim, 0x013C5A7C), which GetUseFixedFunction reads.
// Port (NF40, round 4): was the descriptive free function SetLeftAlignText over `g_flag`; now the static member on
//   engine/CStreamingFontBank.h (FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER for the member, overlay opt-in
//   FABLE_RETAIL_CSTREAMINGFONTBANK_SETUSEFIXEDFUNCTION for the setter declaration).
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER
#define FABLE_RETAIL_CSTREAMINGFONTBANK_SETUSEFIXEDFUNCTION
#include "engine/CStreamingFontBank.h"

void FABLE_FASTCALL CStreamingFontBank::SetUseFixedFunction(bool value)
{
    UseFixedFunction = value;
}
