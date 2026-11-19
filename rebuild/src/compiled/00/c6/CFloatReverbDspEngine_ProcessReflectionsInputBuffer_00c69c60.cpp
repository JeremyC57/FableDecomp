// CFloatReverbDspEngine::ProcessReflectionsInputBuffer(float* pfInput, bool bInputValid, unsigned int uiSamples) -- feed one block into the engine's shared reflections input buffer: when the input has just switched off, fade out the last (up to the first reflection's first input delay of) samples already written and clear both input filters; write the block through the low-pass (+0x6c) and the in-place high-pass input filter (+0x80), fading current -> new parameters while the input filters' fade is active, in the tail run and the wrapped head run; when the input has just switched on, fade the new block in; then advance the write cursor
// Leaf: ?ProcessReflectionsInputBuffer@CFloatReverbDspEngine@@AAEXPAM_NI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C69C60, 837 bytes (+ int3 padding), `ret 0Ch`.
// Name (NF105, q11519; was the descriptive CReverbProcessor::ProcessInput over a local model): GameCODA 2.10 CodeView private
//   ?ProcessReflectionsInputBuffer@CFloatReverbDspEngine@@AAEXPAM_NI@Z. Two sources: (1) this body is byte-identical (837 bytes,
//   relocations masked) to that CODAApi.lib FloatReverbDspEngine.obj member; (2) the byte-identical SDK
//   CFloatReverbDspEngine::ProcessReverb 0x00C6A5D0 calls it at REL32 +0x35, the retail call site of 0x00C69C60. Layout: the CodeView
//   CReverbDspEngine (m_pvParams +4, m_pvParamsNew +8, m_bIsFading +0x14, m_iFaderToken +0x18, m_iSamplesToFade +0x1c) and
//   CFloatReverbDspEngine (m_reflectionsInputBuffer +0x54, m_inputFilterLowPass +0x6c, m_inputFilterHighPass +0x80,
//   m_bOldInputValid +0x28c, m_iInputFilterFaderToken +0x290); the parameter blocks are CFloatReverbParameters.
// Callees: CFloatFilter::Clear 0x00C744E0, CFloatFilter::Process 0x00C74CE0 (ret 14h) and its fading overload 0x00C74D30 (ret 1Ch),
//   and virtual slot 2 ScaleSamples(float*, float, float, unsigned int) of CReverbDspProcessor::m_pDSP 0x0143CCDC.
// Constants: 4294967296.0f @ 0x0122DCB4 (unsigned -> float), -1.0f @ 0x0122DEE0, 1.0f @ 0x0122DED8.
// Shape: the landed body with the CodeView names: both parameter pointers read first; each fade writes `(float)n` at every use
//   (VC7.1 shares the one conversion in a frame slot and keeps two `fdiv`s; a float local would become one reciprocal multiply);
//   the split-run length is recomputed in each arm from the members.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATREVERBDSPENGINE_PROCESSREVERB
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbDspEngine.h"

void CFloatReverbDspEngine::ProcessReflectionsInputBuffer(float* pfInput, bool bInputValid, unsigned int uiSamples)
{
    CFloatReverbParameters* pParams = static_cast<CFloatReverbParameters*>(m_pvParams);
    CFloatReverbParameters* pNewParams = static_cast<CFloatReverbParameters*>(m_pvParamsNew);
    if (!bInputValid && m_bOldInputValid) {
        unsigned int uiFade = uiSamples;
        if (uiFade > pParams->m_Ref[0].m_auiInputDelay[0])
            uiFade = pParams->m_Ref[0].m_auiInputDelay[0];
        if (uiFade != 0) {
            unsigned int uiCursor = m_reflectionsInputBuffer.GetWriteCursor();
            unsigned int uiStart;
            if (uiCursor >= uiFade)
                uiStart = uiCursor - uiFade;
            else
                uiStart = m_reflectionsInputBuffer.GetLength() - uiFade + uiCursor;
            unsigned int uiFirst = uiFade;
            if (uiStart + uiFade > m_reflectionsInputBuffer.GetLength())
                uiFirst = m_reflectionsInputBuffer.GetLength() - uiStart;
            float fStep = -1.0f / (float)uiFade;
            m_pDSP->ScaleSamples(&m_reflectionsInputBuffer.m_pBase[uiStart], 1.0f, fStep, uiFirst);
            if (uiFirst < uiFade)
                m_pDSP->ScaleSamples(m_reflectionsInputBuffer.m_pBase, 1.0f - (float)uiFirst / (float)uiFade, fStep, uiFade - uiFirst);
        }
        m_inputFilterLowPass.Clear();
        m_inputFilterHighPass.Clear();
    }
    if (m_bIsFading && m_iFaderToken == m_iInputFilterFaderToken) {
        unsigned int uiFirst = uiSamples;
        if (m_reflectionsInputBuffer.GetWriteCursor() + uiSamples > m_reflectionsInputBuffer.GetLength())
            uiFirst = m_reflectionsInputBuffer.GetLength() - m_reflectionsInputBuffer.GetWriteCursor();
        m_inputFilterLowPass.Process(&pParams->m_inputFilterLowPass, &pNewParams->m_inputFilterLowPass, pfInput,
                                     &m_reflectionsInputBuffer.m_pBase[m_reflectionsInputBuffer.GetWriteCursor()], bInputValid, uiFirst,
                                     m_iSamplesToFade);
        m_inputFilterHighPass.Process(&pParams->m_inputFilterHighPass, &pNewParams->m_inputFilterHighPass,
                                      &m_reflectionsInputBuffer.m_pBase[m_reflectionsInputBuffer.GetWriteCursor()],
                                      &m_reflectionsInputBuffer.m_pBase[m_reflectionsInputBuffer.GetWriteCursor()], true, uiFirst,
                                      m_iSamplesToFade);
        if (uiFirst < uiSamples) {
            m_inputFilterLowPass.Process(&pParams->m_inputFilterLowPass, &pNewParams->m_inputFilterLowPass, pfInput + uiFirst,
                                         m_reflectionsInputBuffer.m_pBase, bInputValid, uiSamples - uiFirst, m_iSamplesToFade - uiFirst);
            m_inputFilterHighPass.Process(&pParams->m_inputFilterHighPass, &pNewParams->m_inputFilterHighPass,
                                          m_reflectionsInputBuffer.m_pBase, m_reflectionsInputBuffer.m_pBase, true, uiSamples - uiFirst,
                                          m_iSamplesToFade - uiFirst);
        }
    } else {
        unsigned int uiFirst = uiSamples;
        if (m_reflectionsInputBuffer.GetWriteCursor() + uiSamples > m_reflectionsInputBuffer.GetLength())
            uiFirst = m_reflectionsInputBuffer.GetLength() - m_reflectionsInputBuffer.GetWriteCursor();
        m_inputFilterLowPass.Process(&pParams->m_inputFilterLowPass, pfInput,
                                     &m_reflectionsInputBuffer.m_pBase[m_reflectionsInputBuffer.GetWriteCursor()], bInputValid, uiFirst);
        m_inputFilterHighPass.Process(&pParams->m_inputFilterHighPass,
                                      &m_reflectionsInputBuffer.m_pBase[m_reflectionsInputBuffer.GetWriteCursor()],
                                      &m_reflectionsInputBuffer.m_pBase[m_reflectionsInputBuffer.GetWriteCursor()], true, uiFirst);
        if (uiFirst < uiSamples) {
            m_inputFilterLowPass.Process(&pParams->m_inputFilterLowPass, pfInput + uiFirst, m_reflectionsInputBuffer.m_pBase,
                                         bInputValid, uiSamples - uiFirst);
            m_inputFilterHighPass.Process(&pParams->m_inputFilterHighPass, m_reflectionsInputBuffer.m_pBase,
                                          m_reflectionsInputBuffer.m_pBase, true, uiSamples - uiFirst);
        }
    }
    if (bInputValid && !m_bOldInputValid) {
        unsigned int uiFirst = uiSamples;
        if (m_reflectionsInputBuffer.GetWriteCursor() + uiSamples > m_reflectionsInputBuffer.GetLength())
            uiFirst = m_reflectionsInputBuffer.GetLength() - m_reflectionsInputBuffer.GetWriteCursor();
        float fStep = 1.0f / (float)uiSamples;
        m_pDSP->ScaleSamples(&m_reflectionsInputBuffer.m_pBase[m_reflectionsInputBuffer.GetWriteCursor()], 0.0f, fStep, uiFirst);
        if (uiFirst < uiSamples)
            m_pDSP->ScaleSamples(m_reflectionsInputBuffer.m_pBase, (float)uiFirst / (float)uiSamples, fStep, uiSamples - uiFirst);
    }
    m_reflectionsInputBuffer.AddWriteCursor(uiSamples);
}
