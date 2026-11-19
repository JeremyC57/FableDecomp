// CFloatReverbDspEngine::ProcessReverb(void* pvInput, bool bInputValid, void** apvOutputs, unsigned int uiSamples) -- the float reverb engine's per-block entry: false until the engine is created (m_bCreated); else feed the reflections input buffer, run the reflections into a 128-sample block, feed the short reverb input buffer from it, run the short and long reverbs, remember the input-valid flag and return true
// Leaf: ?ProcessReverb@CFloatReverbDspEngine@@MAE_NPAX_NPAPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A5D0, 125 bytes, thiscall `ret 10h`, `and esp,-16` frame.
// Name (NF99 draft, ported by NF105 with q11493 / q11519..q11522; was the descriptive CReverbProcessor::Process over a local model,
//   earlier the propagated CEditControlCentre::GetCurrentMask label): GameCODA 2.10 CodeView protected virtual
//   ?ProcessReverb@CFloatReverbDspEngine@@MAE_NPAX_NPAPAXI@Z (engine/CFloatReverbDspEngine.h declares it). Two sources: (1) this body is
//   byte-identical (125 bytes, relocations masked; NF99P2 / NF105 sdkcmp) to that CODAApi.lib FloatReverbDspEngine.obj member, whose
//   relocations name the retail callees (+0x35 ProcessReflectionsInputBuffer 0x00C69C60, +0x46 ProcessReflections 0x00C69FB0, +0x53
//   ProcessShortReverbInputBuffer 0x00C6A080, +0x5c ProcessShortReverb 0x00C6A180, +0x65 ProcessLongReverb 0x00C6A200); (2) the
//   function's only reference is slot 4 of the engine vtable (0x012B95A4 in 0x012B9594), the slot the base CReverbDspEngine leaves
//   pure (`_purecall` in 0x012BF0C8) as the header's `ProcessReverb(...) = 0`.
// Callees (SDK spellings; their self rows are ported in this hand-back): ProcessReflectionsInputBuffer 0x00C69C60,
//   ProcessReflections 0x00C69FB0, ProcessShortReverbInputBuffer 0x00C6A080, ProcessShortReverb 0x00C6A180 (ported with this row),
//   ProcessLongReverb 0x00C6A200.
// Members: m_bCreated +0x294, m_bOldInputValid +0x28c (the CodeView names on the shared header).
// Shape: the scratch block is a 16-aligned 128-float local (the landed char[0x1f4] gives the same frame) (`and esp,0FFFFFFF0h ; sub esp,204h`); the early `return false`
//   precedes the arguments' loads; `active` is re-read from its slot for the final byte store.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB
#include "engine/CFloatReverbDspEngine.h"

bool CFloatReverbDspEngine::ProcessReverb(void* pvInput, bool bInputValid, void** apvOutputs, unsigned int uiSamples)
{
    __declspec(align(16)) float afBuffer[128];           // one 128-sample block
    if (!m_bCreated)
        return false;
    ProcessReflectionsInputBuffer(static_cast<float*>(pvInput), bInputValid, uiSamples);
    ProcessReflections(reinterpret_cast<float**>(apvOutputs), uiSamples, afBuffer);
    ProcessShortReverbInputBuffer(uiSamples, afBuffer);
    ProcessShortReverb(reinterpret_cast<float**>(apvOutputs), uiSamples);
    ProcessLongReverb(reinterpret_cast<float**>(apvOutputs), uiSamples);
    m_bOldInputValid = bInputValid;
    return true;
}
