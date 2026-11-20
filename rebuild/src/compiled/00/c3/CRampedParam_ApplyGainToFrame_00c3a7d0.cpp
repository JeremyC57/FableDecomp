// CRampedParam::ApplyGainToFrame(CCodaFrame& frame, unsigned long sampleRate) -- audio: take the ramped gain (wrapped into [-pi, pi) for an angle parameter) clamped to [0, 1], advance the ramp by the frame's duration in ms, then scale every 16-bit sample of the frame by the gain
// Leaf: ?ApplyGainToFrame@CRampedParam@@QAEXAAVCCodaFrame@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3A7D0, 273 bytes (+ int3 padding), __fastcall `ret 8`. Grade: RELOCATION_MATCH (273/273). No flag.
// Library: libpk audio (no PDB, no RTTI, no SDK source): descriptive names. `this` is libpk/CFader.h's 0x34-byte CRampedParam (the
//   landed SetImmediate 0x00C0B510, IsSettledAtZero 0x00C3A8F0); its only caller 0x00C39007 applies the voice's +0x138 gain parameter to
//   the frame its source returned. The frame is libpk/CCodaFrameStreamer.h's CCodaFrame (CCodaFrame.cpp trace; channels +0, valid
//   samples +8; GetData 0x00C491D0 = +0x10). The method name is descriptive: the catalogue's "Advance" on CRampedParam_00c3a8f0 and the
//   label AudioLib_ApplyGainEnvelope_00c3a7d0 are both made up; this one says what the body does (NAME_FIXES.tsv).
// Callees: CRampedParam::Update(float) 0x00C0B7B0 (the spelling the landed CFader::Update 0x00C25F40 / CAmbientCube::UpdateLevel
//   0x00C45980 bind), CCodaFrame::GetData 0x00C491D0, __ftol2 0x00BFEA70. Constants: pi 0x0128F604, 2*pi 0x0128F608, -pi 0x012B1EB8,
//   0.0f 0x0122DEDC, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (the unsigned-to-float fix-ups), 1000.0f 0x01252370, 32767.0f 0x0129C820.
// Shape:
//   - The gain is GetValue(), expanded as `m_bAngle ? WrapAngle(m_value) : m_value` (libpk/CFader.h opt-in FABLE_RETAIL_CRAMPEDPARAM_GETVALUE).
//     The wrap's two arithmetic paths join with the value in st(0), so the clamp stores it first (`fst`, in the dead `frame` argument
//     slot) and compares it popped against 0.0f (`fcomp`), reloading it for the 1.0f test. That header text is pinned by four retail copies
//     (WrapAngle 0x00C0B4C0, GetValue 0x00C0B730, GetTarget 0x00C0B770 out of line; IsSettledAtZero 0x00C3A8F0 / 0x00C38510 expanded).
//     The value-local GetValue spellings either reload the stored value (`fstp ; fld`, early returns) or compare before the store (`fcom ; fst`).
//   - The frame duration is a product in a named local, then divided by the rate in the call: retail finishes `fmul 1000.0f` before it
//     loads the rate for its sign fix-up. One expression schedules the rate load first.
//   - The samples loop counts frames down and channels down, scaling through the unsigned 16-bit gain and the signed divide by 32767.
#define FABLE_RETAIL_CCODAFRAMESTREAMER_PASS23
#define FABLE_RETAIL_CRAMPEDPARAM_GETVALUE
#define FABLE_RETAIL_CRAMPEDPARAM_APPLYGAINTOFRAME
#include "libpk/CCodaFrameStreamer.h"
#include "libpk/CFader.h"

void CRampedParam::ApplyGainToFrame(CCodaFrame& frame, unsigned long sampleRate)
{
    if (!frame.m_samples)
        return;
    float gain = GetValue();
    if (gain < 0.0f)
        gain = 0.0f;
    else if (gain > 1.0f)
        gain = 1.0f;
    float ms = (float)frame.m_samples * 1000.0f;
    Update(ms / (float)sampleRate);
    short* sample = frame.GetData();
    unsigned long frames = frame.m_samples;
    long scale = (long)(gain * 32767.0f);
    for (; frames; --frames) {
        for (unsigned long n = frame.m_channels; n; --n) {
            *sample = (short)(*sample * (unsigned short)scale / 32767);
            ++sample;
        }
    }
}