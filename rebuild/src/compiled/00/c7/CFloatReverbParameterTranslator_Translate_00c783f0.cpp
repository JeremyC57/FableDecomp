// CFloatReverbParameterTranslator::Translate(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams) -- apply the high-level reverb parameters' dirty update flags: run each flagged update (size, diffusion, input filters, echo, decay times, modulation, delays, pans, gains, level), then clear the flag word
// Leaf: ?Translate@CFloatReverbParameterTranslator@@QAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C783F0, 405 bytes, thiscall, `ret 8`.
// Name (NF99, q11477; was the descriptive CEffectPart260::Configure over local models): GameCODA 2.10 CODAApi.lib
//   FloatReverbParameterTranslator.obj public ?Translate@CFloatReverbParameterTranslator@@QAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
//   (405 bytes, byte-identical with relocations masked: hand4d b10) + the SDK FloatReverbEngine.obj ?Commit@CFloatReverbEngine@@
//   REL32 +0x11 to it, retail 0x00C6AF01 in 0x00C6AEF0 (92 bytes = the SDK Commit length). The landed CFloatReverbEngine::Create
//   0x00C6B050 already binds this spelling. Members: the CodeView names on the shared headers.
// Callees (the release REL32 targets, SDK spellings; hand4d NOTES addresses): UpdateSize 0x00C78150, UpdateDiffusion 0x00C780C0,
//   CFloatFilterParams::Set_HighPass 0x00C74BB0 / Set_LpButterworth2 0x00C74AB0, UpdateEchoTime 0x00C77FC0, UpdateDecayTimes
//   0x00C77800 (q11414), UpdateModulationDepth 0x00C78040, UpdateReverbDelay 0x00C77C80, UpdateReflectionsDelay 0x00C77B30,
//   UpdateReflectionsOutputGain 0x00C77AE0, UpdateReverbPan 0x00C783B0, UpdateReverbOutputGain 0x00C77C00, the private static PanToGains 0x00C77D90 (NF99P2 q11510: was
//   the free MixChannels_00c77d90, renamed with its self row and its other binder 0x00C783B0); and the free ?mBToLinear@@YAMJ@Z 0x00C77700
//   (NF99P2 q11511: was MillibelsToAmplitude, renamed with its self row).
// Inline members: the CODAApiDbg body calls UpdateInputHpFilters (flags 0x48), UpdateInputLpFilters (0x90), UpdateModulationTime
//   (0x80000), UpdateReflectionsPan (0x800) and UpdateLevel (0x20); the release expands them in place, so their bodies are written
//   here (no retail address).
// Shape: retail keeps an unreachable arm in the flag-2 step (`or eax,4 ; jne store`, fall-through `call UpdateDiffusion ; jmp`; the
//   Dbg body has the same arm): VC7.1 does not fold `(Flags | 4) == 0`, so the source tests the or'd word and calls UpdateDiffusion on
//   the dead arm (written first: retail's fall-through). The plain `Flags |= 4` is 13 bytes short (392v405).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#define FABLE_RETAIL_CFLOATFILTERPARAMS_SET_HIGHPASS
#define FABLE_RETAIL_CFLOATFILTERPARAMS_SET_LPBUTTERWORTH2
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_PANTOGAINS
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"
float __cdecl mBToLinear(long millibels);                                                // 0x00C77700 (NF99P2 q11511)

void CFloatReverbParameterTranslator::Translate(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams)
{
    if (pSrcParams->m_uiUpdateFlags == 0)
        return;
    if (pSrcParams->m_uiUpdateFlags & 1) {
        UpdateSize(pReverbParams, pSrcParams);
        pSrcParams->m_uiUpdateFlags |= 0x9106;
    }
    if (pSrcParams->m_uiUpdateFlags & 2) {
        unsigned int uiFlags = pSrcParams->m_uiUpdateFlags | 4;
        if (uiFlags == 0)
            UpdateDiffusion(pReverbParams, pSrcParams);
        else
            pSrcParams->m_uiUpdateFlags = uiFlags;
    }
    if (pSrcParams->m_uiUpdateFlags & 4) {
        UpdateDiffusion(pReverbParams, pSrcParams);
        pSrcParams->m_uiUpdateFlags |= 0x9000;
    }
    if (pSrcParams->m_uiUpdateFlags & 0x48) {                    // UpdateInputHpFilters
        pReverbParams->m_inputFilterHighPass.SetGain(1.0f);
        pReverbParams->m_inputFilterHighPass.Set_HighPass(pSrcParams->m_iLevelLF, pSrcParams->m_fRefFrequencyLF,
                                                          m_pReverbConstants->m_uiSamplesPerSecond);
    }
    if (pSrcParams->m_uiUpdateFlags & 0x90)                      // UpdateInputLpFilters
        pReverbParams->m_inputFilterLowPass.Set_LpButterworth2(pSrcParams->m_iLevelHF, pSrcParams->m_fRefFrequencyHF,
                                                               m_pReverbConstants->m_uiSamplesPerSecond);
    if (pSrcParams->m_uiUpdateFlags & 0x20000) {
        UpdateEchoTime(pReverbParams, pSrcParams);
        pSrcParams->m_uiUpdateFlags |= 0x100;
    }
    if (pSrcParams->m_uiUpdateFlags & 0x40000)
        pSrcParams->m_uiUpdateFlags |= 0x8000;
    if (pSrcParams->m_uiUpdateFlags & 0x718)
        UpdateDecayTimes(pReverbParams, pSrcParams);
    if (pSrcParams->m_uiUpdateFlags & 0x80000)                   // UpdateModulationTime
        pReverbParams->m_PitchShift.m_fModulationPeriodSamples =
            (float)m_pReverbConstants->m_uiSamplesPerSecond * pSrcParams->m_fModulationTime;
    if (pSrcParams->m_uiUpdateFlags & 0x180000)
        UpdateModulationDepth(pReverbParams, pSrcParams);
    if (pSrcParams->m_uiUpdateFlags & 0x10000)
        UpdateReverbDelay(pReverbParams, pSrcParams);
    if (pSrcParams->m_uiUpdateFlags & 0x12000) {
        UpdateReflectionsDelay(pReverbParams, pSrcParams);
        UpdateReverbDelay(pReverbParams, pSrcParams);
    }
    if (pSrcParams->m_uiUpdateFlags & 0x800)                     // UpdateReflectionsPan
        PanToGains(&pSrcParams->m_ReflectionsPan, m_fRefPanGain, 4, m_pReverbConstants->m_uiNumOutputs);
    if (pSrcParams->m_uiUpdateFlags & 0x1800)
        UpdateReflectionsOutputGain(pReverbParams, pSrcParams);
    if (pSrcParams->m_uiUpdateFlags & 0x4000)
        UpdateReverbPan(pSrcParams);
    if (pSrcParams->m_uiUpdateFlags & 0xc000)
        UpdateReverbOutputGain(pReverbParams, pSrcParams);
    if (pSrcParams->m_uiUpdateFlags & 0x20)                      // UpdateLevel
        pReverbParams->m_inputFilterLowPass.SetGain(mBToLinear(pSrcParams->m_iLevel));
    pSrcParams->m_uiUpdateFlags = 0;
}
