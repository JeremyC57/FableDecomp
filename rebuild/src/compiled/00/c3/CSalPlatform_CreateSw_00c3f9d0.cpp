// 00c3f9d0 CSalPlatform::CreateSw (EH parent; its generated cleanup 0x01215280 lands as a separate funclet row that includes this file)
// Leaf: ?CreateSw@CSalPlatform@@AAE_NPBU_CRESOURCEDESC@@PBUCPLATFORMDESC@@@Z
// Flags: /EHsc /GS
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#define FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23
#include "engine/CSalPlatform.h"
#include "engine/CSalCpuInfo.h"
#include "engine/CSalMixer.h"
#include "engine/CSalOutputMM.h"
#include "engine/CSalOutputNull.h"
#include "engine/CDSoundAccelerationQuery.h"
#include "engine/CodaCaps.h"
#include "engine/CSalHrtf.h"
bool CSalPlatform::CreateSw(const CRESOURCEDESC* resources,const CPLATFORMDESC* params)
{
    CSalCpuInfo cpu;
    if (!CSalCpuInfo::HasMMX())
        return false;
    if (CSalCpuInfo::HasSSE2())
        m_pMixer = new CSalMixerLimiterSSE2;
    else
        m_pMixer = new CSalMixerLimiterMMX;
    if (!m_pMixer)
        return false;
    switch (params->SWOutputDevice) {
    case 1:
        m_pOutputStream = new CSalOutputNull;
    if (!m_pOutputStream)
        return false;
    if (!m_pOutputStream->Create(params->pGuid, (HWND)params->hWnd, false)) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        return false;
    }
        break;
    case 4:
        m_pOutputStream = new CSalOutputKS;
    if (!m_pOutputStream)
        return false;
    if (!m_pOutputStream->Create(params->pGuid, (HWND)params->hWnd, false)) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        return false;
    }
        break;
    case 2:
        if (CreateSwDirectSound(params))
            goto ready;
        return false;
    case 5:
        if (CreateSwBufferedDirectSound(params))
            goto ready;
        return false;
    case 0: {
        CDSoundAccelerationQuery devices;
        if (devices.GetAccelerationLevel(params->pGuid) > 0 && CreateSwDirectSound(params))
            goto ready;
    }
    // Fall through.
    case 3:
        m_pOutputStream = new CSalOutputMM;
    if (!m_pOutputStream)
        return false;
    if (!m_pOutputStream->Create(params->pGuid, (HWND)params->hWnd, false)) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        return false;
    }
        break;
    default:
        return false;
    }
ready:
    bool hasMixOut = m_pHw != 0;
    int noSecond = m_pReverbOutputStream == 0 ? 1 : 0;
    if (noSecond & static_cast<int>(hasMixOut)) {
        g_SalCaps.SW2DVoice.ffCaps &= ~0x80u;
        g_SalCaps.SW3DVoice.ffCaps &= ~0x80u;
        g_SalCaps.SW2DVoice.ffCaps &= ~0x2000u;
        g_SalCaps.SW3DVoice.ffCaps &= ~0x2000u;
    }
    unsigned int blockSize = m_pOutputStream->GetMinBufferSamples();
    g_SalCaps.SWMixer.nMinFrameSamples = blockSize;
    g_SalCaps.SWMixer.nFrameSamples = blockSize;
    if (g_HRTFMode == HRTF_NONE)
        g_SalCaps.SW3DVoice.ffCaps &= ~0x300000u;
    unsigned int start = params->nSW2DVoices;
    g_SalCaps.SW2DVoice.nVoices = start;
    g_SalCaps.SW2DVoice.nFreeVoices = params->nSW2DVoices;
    unsigned int length = params->nSW3DVoices;
    g_SalCaps.SW3DVoice.nVoices = length;
    g_SalCaps.SW3DVoice.nFreeVoices = params->nSW3DVoices;
    int end = (start != (unsigned int)-1 && length != (unsigned int)-1) ? (int)(start + length) : -1;
    g_SalCaps.nSWAllVoices = end;
    g_SalCaps.nSWFreeVoices = end;
    unsigned int size = params->nSWMixFrameSamples;
    if (size > blockSize) {
        if (size & 0x7f)
            size = (size / 128 + 1) * 128;
        g_SalCaps.SWMixer.nFrameSamples = size;
    }
    if (!CreateSwVoices())
        return false;
    m_bXtcProcessing = false;
    m_nSw2DVoicesPlaying = 0;
    m_nSw3DVoicesPlaying = 0;
    if (!SetSWOutputConfig()) {
        delete m_pOutputStream;
        m_pOutputStream = 0;
        delete m_pReverbOutputStream;
        m_pReverbOutputStream = 0;
        return false;
    }
    return true;
}