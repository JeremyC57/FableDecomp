// CSalOpenALBuffer::MakeStereoBuffer
// Leaf: ?MakeStereoBuffer@CSalOpenALBuffer@@AAEHXZ
// Flags: none
// Inferred: retail adds context-state preservation around generation and deletion. These explicit scopes model the native sequence.
// The SDK private GetALProcAddress supplies the shared loader.
#include "engine/CodaMemory.h"
#define FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
#include "engine/CSalOpenAL.h"
#include "engine/CSalOpenALBuffer.h"

int CSalOpenALBuffer::MakeStereoBuffer()
{
    void* pvStereoData = CodaMalloc(m_pDesc->cbSize * 2);
    if (!pvStereoData)
        return C_ERR_OUTOFMEMORY;

    signed short* pShortSrc;
    signed short* pShortDest;
    unsigned char* pByteSrc;
    unsigned char* pByteDest;
    int format;
    int cr;
    switch (m_pDesc->ffFormat) {
    case C_FORMAT_INT16: {
        pShortSrc = static_cast<signed short*>(m_pDesc->pvData);
        pShortDest = static_cast<signed short*>(pvStereoData);
        for (unsigned int i = 0; i < m_pDesc->cbSize / 2; ++i) {
            pShortDest[i * 2] = pShortSrc[i];
            pShortDest[i * 2 + 1] = pShortDest[i * 2];
        }
        format = AL_FORMAT_STEREO16;
        break;
    }
    case C_FORMAT_INT8: {
        pByteSrc = static_cast<unsigned char*>(m_pDesc->pvData);
        pByteDest = static_cast<unsigned char*>(pvStereoData);
        for (unsigned int i = 0; i < m_pDesc->cbSize; ++i) {
            pByteDest[i * 2] = pByteSrc[i];
            pByteDest[i * 2 + 1] = pByteDest[i * 2];
        }
        format = AL_FORMAT_STEREO8;
        break;
    }
    default:
        CodaFree(pvStereoData);
        return C_ERR_BUFFERTYPE;
    }

    g_OpenAL.GetError();
    if (!(m_ffStatus & 2)) {
        g_OpenAL.GetError();
        {
            const bool was_suspended = g_OpenAL.IsSuspended();
            g_OpenAL.Suspend();
            g_OpenAL.GenBuffers(1, &m_uiBuffer[1]);
            if (!was_suspended)
                g_OpenAL.Resume();
        }
        cr = g_OpenAL.CheckError();
        if (cr < C_OK) {
            CodaFree(pvStereoData);
            return cr;
        }
        m_ffStatus |= 2;
    }
    g_OpenAL.BufferData(m_uiBuffer[1], format, pvStereoData, m_pDesc->cbSize * 2, m_pDesc->nSamplesPerSec);
    cr = g_OpenAL.CheckError();
    CodaFree(pvStereoData);
    if (cr < C_OK) {
        {
            const bool was_suspended = g_OpenAL.IsSuspended();
            g_OpenAL.Suspend();
            g_OpenAL.DeleteBuffers(1, &m_uiBuffer[1]);
            if (!was_suspended)
                g_OpenAL.Resume();
        }
        m_ffStatus &= ~2;
        m_uiBuffer[1] = 0xC0DAF00D;
        return cr;
    }
    return C_OK;
}