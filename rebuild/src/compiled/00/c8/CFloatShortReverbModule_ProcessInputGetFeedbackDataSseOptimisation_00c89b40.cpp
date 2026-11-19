// CFloatShortReverbModule::ProcessInput_GetFeedbackData_SseOptimisation(float* pfOutput, unsigned int uiSamples) -- SSE-path first half of one short-reverb module block: copy uiSamples floats of the module's own cyclic buffer from its write cursor into pfOutput (the filter input of the quad filter)
// Leaf: ?ProcessInput_GetFeedbackData_SseOptimisation@CFloatShortReverbModule@@QAEXPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89B40, 32 bytes, thiscall `ret 8`: m_pDSP 0x0143CCDC, CReverbFns::CopySamples 0x00C763E0.
// Name (NF105, q11512; was the stand-in free function CTCActivationReceptorBase_AddTrigger over local models): GameCODA 2.10 CodeView
//   public ?ProcessInput_GetFeedbackData_SseOptimisation@CFloatShortReverbModule@@QAEXPAMI@Z. Two sources: (1) this body is
//   byte-identical (32 bytes, relocations masked) to that CODAApi.lib FloatShortReverbModule.obj member (+0x13 m_pDSP, +0x19
//   ?CopySamples@CReverbFns@@QAEXPAMPBVCFloatCyclicBuffer@@II@Z = 0x00C763E0); (2) the byte-identical SDK CFloatShortReverb::Process
//   0x00C75CE0 calls it at REL32 +0x5e, the retail call site. Layout: m_Buffer +0x18 (cursor +0x20).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
#include "engine/CFloatShortReverbModule.h"

void CFloatShortReverbModule::ProcessInput_GetFeedbackData_SseOptimisation(float* pfOutput, unsigned int uiSamples)
{
    m_pDSP->CopySamples(pfOutput, &m_Buffer, m_Buffer.GetWriteCursor(), uiSamples);
}
